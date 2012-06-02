#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include "RCGrandDogW32.h"

#define DOG_EXE_NAME "dongletools.exe"
#define DOG_NAME "GrandDog"
#define DOG_PASSWORD "did81rnd"
#define DOG_DIR 0x3F00
#define DOG_FILES 0x10
#define DOG_FILE_SIZE 0x80
#define DOG_MAX_FILE_SIZE 0x2000
#define DOG_SIZES_FILE 4

#define DOG_FORMAT_FILE 10
UCHAR DOG_FORMAT_DATA[] = {
	0x34, 0x12, 0x01, 0x00, 0x1e, 0x41, 0x2b, 0x2a, 0xa1, 0x62, 0x99, 0x41, 0x8e, 0x74, 0x36, 0x88, 
	0x1f, 0x5b, 0xca, 0xfa, 0x0d, 0x2f, 0xd5, 0x82, 0x04, 0x0c, 0x00, 0x0e, 0x0e, 0x45, 0x70, 0x0f, 
	0x05, 0x12, 0x30, 0xf0, 0x6d, 0xbe, 0x64, 0x63, 0xa9, 0x4d, 0x8d, 0xfd, 0xf1, 0x00, 0x04, 0x45, 
	0x76, 0x0f, 0xfa, 0xdf, 0x3e, 0x1c, 0xb3, 0x55, 0x0f, 0xa8, 0x80, 0x39, 0x01, 0x46, 0x30, 0x40, 
	0x07, 0x0c, 0xfe, 0x00, 0x75, 0x8c, 0x88, 0x50, 0x15, 0xde, 0xcb, 0x39, 0xe1, 0x04, 0x45, 0xa0, 
	0x08, 0xad, 0x9f, 0x72, 0x64, 0xff, 0x30, 0x4d, 0x4b, 0x3c, 0xd5, 0x39, 0x01, 0xe4, 0x64, 0xf0, 
	0x09, 0x0a, 0xf0, 0x90, 0x7f, 0x14, 0xef, 0x1e, 0xa1, 0x9a, 0xd6, 0x39, 0x01, 0xa0, 0x05, 0x50, 
	0x0a, 0x0f, 0xdf, 0x09, 0xe1, 0x91, 0xf4, 0x43, 0x6f, 0x8e, 0xda, 0x39, 0x45, 0x56, 0xa6, 0xe0
};

//#define DOG_LOG(...) printf("\tLOG: " __VA_ARGS__)
#define DOG_LOG(...)

typedef struct _ReadSuggestion { int dir; int file; int size; } ReadSuggestion;
static bool IsSuggestion(ReadSuggestion* suggestion) { return suggestion->dir || suggestion->file || suggestion->size; }
static ReadSuggestion readSuggestions[] = {
	{ 0x3F00, 1, 0x97 },
	{ 0x3F00, 2, 0x70 },
	{ 0x3F00, 3, 0xB57 },
	{ 0x3F00, 4, 0x0C },
	{ 0x3F00, 5, 0x40 },
	{ 0x3F00, 6, 0x70 },
	{ 0x3F00, 10, 0x80 },
	{ 0, 0, 0 }
};
static ReadSuggestion* SuggestionAt(int dir, int file)
{
	for (ReadSuggestion* suggestion = readSuggestions; IsSuggestion(suggestion); suggestion++) {
		if (suggestion->dir == dir && suggestion->file == file)
			return suggestion;
	}
	return NULL;
}

static ULONG dogHandle = 0;

static void DogOpen()
{
	if (!dogHandle) {
		HRESULT code = rc_OpenDog(RC_OPEN_FIRST_IN_LOCAL, DOG_NAME, &dogHandle);
		if (code || !dogHandle) {
			printf("Open dongle failed with error code %08x\n", code);
			exit(1);
		}
		printf("Dongle opened\n");
	}
}

static void DogClose()
{
	if (dogHandle) {
		HRESULT code = rc_CloseDog(dogHandle);
		if (code != S_OK) {
			printf("Error closing dongle: %08x\n", code);
			exit(1);
		}
		printf("Dongle closed\n");
	}
}

static void DogVerify()
{
	if (dogHandle) {
		UCHAR verifyCount = 0;
		HRESULT code = rc_VerifyPassword(dogHandle, RC_PASSWORDTYPE_USER, DOG_PASSWORD, &verifyCount);
		if (code) {
			printf("Password verification failed with error %08x - %d verifications left\n", code, (int)verifyCount);
			DogClose();
			exit(1);
		}
		printf("Password verified and %d verifications left\n", (int)verifyCount);
	}
}

static int DogGetFileSize(int dir, int file, int size)
{
	size--;
	int readoffset = 0;
	DOG_LOG("Checking size of file %04x/%d\n", dir, file);
	while (size < DOG_MAX_FILE_SIZE) { // Condition is to prevent looping forever
		DOG_LOG("Reading at %08x\n", size);
		char temp = 0;
		HRESULT code = rc_ReadFile(dogHandle, dir, file, size, 1, &temp);
		if (code == E_RC_INVALIDARG) {
			if (!readoffset) {
				size--;
				DOG_LOG("Sizing down to %08x\n", size);
				continue;
			} else {
				DOG_LOG("Done at size %08x\n", size);
				break;
			}
		} else if (code == E_RC_NOT_FIND_FILE) {
			printf("\tFile does not exist\n");
			return 0;
		} else if (code) {
			printf("\tFailed to read file with error %08x\n", code);
			return 0;
		}
		size++;
		readoffset++;
	}
	if (size > DOG_MAX_FILE_SIZE)
		printf("\tFile %04x/%d much larger than expected, INCORRECT SIZE REPORTING\n", dir, file);
	return size;
}

static void DogLoadFile(int dir, int file, int size)
{
	char* buffer = NULL;
	printf("Reading file %04x/%d\n", dir, file);
	size = DogGetFileSize(dir, file, size);
	if (size <= 0) {
		printf("\tFilesize could not be determined\n");
		return;
	}
	buffer = malloc(size);
	HRESULT code = rc_ReadFile(dogHandle, dir, file, 0, size, buffer);
	if (code == E_RC_NOT_FIND_FILE) {
		printf("\tFile does not exist\n");
		free(buffer);
		return;
	} else if (code) {
		printf("\tFailed to read file with error %08x\n", code);
		free(buffer);
		return;
	}
	char outname[0x20];
	sprintf(outname, "%04x-%d.bin", dir, file);
	int fd = open(outname, O_TRUNC | O_WRONLY | O_CREAT | O_BINARY, 0666);
	if (fd <= 0) {
		printf("\tError creating file \"%s\" on disk to backup! FILE NOT SAVED\n", outname);
		free(buffer);
		return;
	}
	if (write(fd, buffer, size) != size)
		printf("\tError writing file \"%s\" on disk to backup! INCOMPLETE FILE\n", outname);
	close(fd);
	free(buffer);
}

static bool DogSaveFile(int dir, int file)
{
	printf("Writing file %04x/%d\n", dir, file);

	char name[0x20];
	sprintf(name, "%04x-%d.bin", dir, file);
	int fd = open(name, O_RDONLY | O_BINARY);
	if (fd <= 0) {
		printf("\tFile does not exist\n");
		return false;
	}
	int size = lseek(fd, 0, SEEK_END);
	if (size <= 0) {
		printf("\tUnable to determine file size, aborting\n");
		close(fd);
		return false;
	}
	lseek(fd, 0, SEEK_SET);
	char* buffer = malloc(size);
	int readbytes = size;
	while (readbytes > 0) {
		int ret = read(fd, buffer + size - readbytes, readbytes);
		if (ret <= 0) {
			printf("\tIO error when reading file at %08x, got %08x, aborting\n", readbytes, ret);
			close(fd);
			return false;
		}
		readbytes -= ret;
	}
	close(fd);

	ReadSuggestion* suggestion = SuggestionAt(dir, file);
	if (DogGetFileSize(dir, file, suggestion ? suggestion->size : size) != size) {
		printf("\tFile not on dongle or incorrect size, recreating...\n");
		rc_DeleteFile(dogHandle, dir, file);
		HRESULT code = rc_CreateFile(dogHandle, dir, file, RC_TYPEFILE_DATA, size);
		if (code)
			printf("\tUnable to create file on dongle with error %08x\n", code);
	}

	HRESULT code = rc_WriteFile(dogHandle, dir, file, 0, size, buffer);
	free(buffer);
	if (code) {
		printf("\tFailed to write file to dongle with error %08x\n", code);
		return false;
	} else if (suggestion)
		suggestion->size = size;

	return true;
}

static bool DogFileExists(int dir, int file)
{
	char temp = 0;
	HRESULT code = rc_ReadFile(dogHandle, dir, file, 0, 1, &temp);
	return !code;
}

static void DogDeleteFile(int dir, int file)
{
	printf("Deleting file %04x/%d\n", dir, file);
	if (DogFileExists(dir, file)) {
		HRESULT code = rc_DeleteFile(dogHandle, dir, file);
		if (code) {
			printf("\tUnable to delete file with error %08x\n", code);
			return;
		}
		printf("\tDeleted file\n");
	} else
		printf("\tFile does not exist\n");
}

static void DogInfo()
{
	RC_HARDWARE_INFO info;
	ULONG length = sizeof(info);
	HRESULT code = rc_GetDogInfo(dogHandle, &info, &length);
	if (code) {
		printf("Failed to get dongle info with error %08x\n", code);
		return;
	}
	printf("Dongle info retrieved\n");
	printf("\tSerial Number: %08x\n", info.ulSerialNumber);
	printf("\tCurrent Number: %08x\n", info.ulCurrentNumber);
	printf("\tDongle Type: %08x\n", (int)info.ucDogType);
	printf("\tDongle Model: %.4s\n", info.ucDogModel);

	ULONG productNumber = 0;
	code = rc_GetProductCurrentNo(dogHandle, &productNumber);
	if (code) {
		printf("Failed to get dongle product number with error %08x\n", code);
		return;
	}
	printf("\tProduct number: %08x\n", productNumber);
}

static int sizeFiles[] = { 1, 6, 3 };
static void DogReadSizes(int dir, int file)
{
	int buffer[3];

	printf("Reading sizes file %04x/%d\n", dir, file);
	if (DogGetFileSize(dir, file, sizeof(buffer)) != sizeof(buffer)) {
		printf("\tUnable to use size file\n");
	}
	HRESULT code = rc_ReadFile(dogHandle, dir, file, 0, sizeof(buffer), (UCHAR*)buffer);
	if (code) {
		printf("\tFailed to read sizes file with error %08x\n", code);
		return;
	}

	for (int i = 0; i < 3; i++) {
		int sizeFile = sizeFiles[i];
		int size = buffer[i];
		if (size > DOG_MAX_FILE_SIZE) {
			printf("\tFile %04x/%d size is larger than expected, ignoring\n", dir, sizeFile);
			continue;
		}
		SuggestionAt(dir, sizeFile)->size = size;
		if (sizeFile == 6)
			SuggestionAt(dir, 2)->size = size;
	}

	printf("\tRead file sizes successfully\n");
}

static void DogSaveSizes(int dir, int file)
{
	printf("Writing sizes file %04x/%d\n", dir, file);

	int buffer[3] = { 0, 0, 0 };
	for (int i = 0; i < 3; i++) {
		int sizeFile = sizeFiles[i];
		int size = DogGetFileSize(dir, sizeFile, SuggestionAt(dir, sizeFile)->size);
		if (sizeFile == 6 && !size)
			size = DogGetFileSize(dir, sizeFile, SuggestionAt(dir, 2)->size);
		if (!size)
			size = SuggestionAt(dir, sizeFile)->size ?: (sizeFile == 6 ? SuggestionAt(dir, 2)->size : 0);
		buffer[i] = size;
	}

	if (DogGetFileSize(dir, file, sizeof(buffer)) != sizeof(buffer)) {
		rc_DeleteFile(dogHandle, dir, file);
		HRESULT code = rc_CreateFile(dogHandle, dir, file, RC_TYPEFILE_DATA, sizeof(buffer));
		if (code)
			printf("\tError creating sizes file, write may not succeed. %08x\n", code);
	}

	HRESULT code = rc_WriteFile(dogHandle, dir, file, 0, sizeof(buffer), (UCHAR*)buffer);
	if (code)
		printf("\tUnable to write sizes file with error %08x\n", code);
}

static void DogBackup()
{
	DogReadSizes(DOG_DIR, DOG_SIZES_FILE);

	int dir = DOG_DIR;
	for (int file = 0; file < DOG_FILES; file++) {
		int size = DOG_FILE_SIZE;
		ReadSuggestion* suggestion = SuggestionAt(dir, file);
		if (suggestion)
			size = suggestion->size;
		DogLoadFile(dir, file, size);
	}
}

static void DogSave()
{
	DogReadSizes(DOG_DIR, DOG_SIZES_FILE);

	bool saveSizes = true;
	int dir = DOG_DIR;
	for (int file = 0; file < DOG_FILES; file++) {
		if (DogSaveFile(dir, file) && file == DOG_SIZES_FILE)
			saveSizes = false;
	}

	if (saveSizes)
		DogSaveSizes(DOG_DIR, DOG_SIZES_FILE);
}

static void DogFormat()
{
	int dir = DOG_DIR;
	for (int file = 0; file < DOG_FILES; file++) {
		DogDeleteFile(dir, file);
	}

	printf("Writing template data\n");
	HRESULT code = rc_CreateFile(dogHandle, dir, DOG_FORMAT_FILE, RC_TYPEFILE_DATA, sizeof(DOG_FORMAT_DATA));
	if (code) {
		printf("Unable to create template file due to error %08x\nYOUR DONGLE IS NOW COMPLETELY EMPTY AND WILL NOT LOAD THE GAME, GOOD LUCK\n", code);
		return;
	}
	code = rc_WriteFile(dogHandle, dir, DOG_FORMAT_FILE, 0, sizeof(DOG_FORMAT_DATA), DOG_FORMAT_DATA);
	if (code)
		printf("Unable to write template data due to error %08x\nYOUR DONGLE IS NOW PROBABLY EMPTY, GOOD LUCK\n", code);
}

static void DogUsage(const char* arg)
{
	printf("Usage:\n"
		"`%s p` to print out dongle information.\n"
		"`%s d` to dump files from dongle.\n"
		"`%s u` to upload files to dongle.\n"
		"`%s f` to format the dongle / delete all data.\n",
		arg, arg, arg, arg
	);
}

int main(int argc, const char* argv[])
{
	if (argc != 2 || strlen(argv[1]) != 1) {
		DogUsage(argc ? argv[0] : DOG_EXE_NAME);
		return 1;
	}

	bool verify = false;
	bool format = false;
	bool backup = false;
	bool save = false;
	bool info = false;
	switch (*argv[1]) {
		case 'p': info = true; break;
		case 'd': verify = backup = true; break;
		case 'u': verify = save = true; break;
		case 'f': verify = format = true; break;
		default: DogUsage(argv[0]); return 1;
	}

	DogOpen();

	if (verify)
		DogVerify();

	if (format)
		DogFormat();

	if (backup)
		DogBackup();

	if (save)
		DogSave();

	if (info)
		DogInfo();

	DogClose();

	return 0;
}
