#ifndef _RCGRANDDOGW32_H 
#define _RCGRANDDOGW32_H 
 
#include <windows.h> 
 
// constant definition 
#define RC_OPEN_FIRST_IN_LOCAL			1 
#define RC_OPEN_NEXT_IN_LOCAL			2 
#define RC_OPEN_IN_LAN					3 
#define RC_OPEN_LOCAL_FIRST				4 
#define RC_OPEN_LAN_FIRST				5 
 
#define RC_PASSWORDTYPE_USER			1 
#define RC_PASSWORDTYPE_DEVELOPER		2 
 
#define RC_DOGTYPE_LOCAL				1 
#define RC_DOGTYPE_NET					2 
 
#define RC_TYPEFILE_DATA				1 
#define RC_TYPEFILE_LICENSE				2 
#define RC_TYPEFILE_ALGORITHMS			3 
#define RC_TYPEFILE_DIR					4 
 
#define RC_KEY_AES						1 
#define RC_KEY_SIGN						2 
 
///////////////////////////////////////////////////////////////// 
// structure definitoin 
#pragma pack(1) 
 
typedef struct _RC_HARDWARE_INFO 
{ 
	ULONG 	ulSerialNumber;			//The Serial Number 
	ULONG 	ulCurrentNumber;   		//The Hardware Current Number 
	UCHAR 	ucDogType;        		//Hardware Dog Type, NetDog or LocalDog 
	UCHAR	ucDogModel[4];			//UGRA or PGRA, at this version ,it is equal to UGRA 
}RC_HARDWARE_INFO,*PRC_HARDWARE_INFO; 
 
typedef struct _RC_PRODUCT_INFO 
{ 
	CHAR	szProductName[16];       //Product Name 
	ULONG 	ulProductCurNumber;   	 //The Developer Current Number 
}RC_PRODUCT_INFO,*PRC_PRODUCT_INFO; 
 
#ifdef  __cplusplus 
extern "C" { 
#endif 
 
HRESULT WINAPI rc_OpenDog(ULONG ulFlag, CHAR * pszProductName, ULONG * pDogHandle); 
HRESULT WINAPI rc_CheckDog(ULONG DogHandle); 
HRESULT WINAPI rc_GetDogInfo(ULONG DogHandle, RC_HARDWARE_INFO * pHardwareInfo, ULONG * pulLen); 
HRESULT WINAPI rc_GetProductCurrentNo(ULONG DogHandle, ULONG * pulProductCurrentNo); 
HRESULT WINAPI rc_CloseDog(ULONG DogHandle); 
 
HRESULT WINAPI rc_VerifyPassword(ULONG DogHandle, UCHAR ucPasswordType, CHAR * pszPassword,  UCHAR * pucDegree); 
HRESULT WINAPI rc_ChangePassword(ULONG DogHandle, UCHAR ucPasswordType, CHAR * pszPassword); 
 
HRESULT WINAPI rc_GetUpgradeRequestString(ULONG DogHandle, UCHAR * pucBuf, ULONG * pulLen); 
HRESULT WINAPI rc_Upgrade(ULONG DogHandle, UCHAR * pucUpgrade, ULONG ulLen); 
 
HRESULT WINAPI rc_GetRandom(ULONG DogHandle, UCHAR * pucRandom, UCHAR ucLen); 
HRESULT WINAPI rc_EncryptData(ULONG DogHandle, UCHAR * pucIn, ULONG ulInLen, UCHAR * pucOut, ULONG * pulOutLen); 
HRESULT WINAPI rc_DecryptData(ULONG DogHandle, UCHAR * pucIn, ULONG ulInLen, UCHAR * pucOut, ULONG * pulOutLen); 
HRESULT WINAPI rc_ConvertData(ULONG DogHandle, UCHAR * pucIn, ULONG ulLen, ULONG * pulRet); 
HRESULT WINAPI rc_SetKey(ULONG DogHandle, UCHAR ucKeyType, UCHAR * pucIn, ULONG ulLen); 
HRESULT WINAPI rc_SignData(ULONG DogHandle, UCHAR * pucIn, ULONG ulLen, UCHAR * pucOut, ULONG * pulOutLen); 
 
HRESULT WINAPI rc_ExecuteFile(ULONG DogHandle, USHORT usDirID, USHORT usFileID, UCHAR * pucDataIn,  
							  ULONG ulInLen, UCHAR * pucDataOut, ULONG * pulOutLen); 
HRESULT WINAPI rc_ReadFile(ULONG DogHandle, USHORT usDirID, USHORT usFileID, ULONG ulPos, ULONG ulLen, UCHAR * pucbuf); 
HRESULT WINAPI rc_WriteFile(ULONG DogHandle, USHORT usDirID, USHORT usFileID, ULONG ulPos, ULONG ulLen, UCHAR * pucbuf); 
HRESULT WINAPI rc_VisitLicenseFile(ULONG DogHandle, USHORT usDirID, USHORT usFileID, ULONG ulReserved); 
HRESULT WINAPI rc_CreateFile(ULONG DogHandle, USHORT usDirID, USHORT usFileID, UCHAR ucFileType, ULONG ulFileLen); 
HRESULT WINAPI rc_DeleteFile(ULONG DogHandle, USHORT usDirID, USHORT usFileID); 
HRESULT WINAPI rc_CreateDir(ULONG DogHandle, USHORT usDirID, ULONG ulSize); 
HRESULT WINAPI rc_DeleteDir(ULONG DogHandle, USHORT usDirID); 
HRESULT WINAPI rc_DefragFileSystem(ULONG DogHandle, USHORT usDirID); 
 
#ifdef  __cplusplus 
} 
#endif 
 
 
	#define E_RC_ACCESSDENIED		_HRESULT_TYPEDEF_(0x80070005L)	//General access denied error 
	#define E_RC_INVALIDARG			_HRESULT_TYPEDEF_(0x80000003L)	//One or more arguments are invalid 
	#define E_RC_UNEXPECTED			_HRESULT_TYPEDEF_(0x8000FFFFL)	//Catastrophic failure 
 
//GrandDog error definition 
	#define E_RC_NOT_FIND_DRIVER			_HRESULT_TYPEDEF_(0xA8160001L)	//Not found the driver,or the driver is not installed correctly 
	#define E_RC_NOT_FIND_DOG				_HRESULT_TYPEDEF_(0xA8160002L)	//Not found the corresponding Serial Number Hardware Dog 
	#define E_RC_NOT_FIND_PRODUCT			_HRESULT_TYPEDEF_(0xA8160003L)	//Not found the corresponding Product Name Hardware Dog 
	#define E_RC_NOT_FIND_NEXT				_HRESULT_TYPEDEF_(0xA8160004L)	//Not found the next Hardware Dog 
	#define E_RC_NOT_FIND_KERNELDLL			_HRESULT_TYPEDEF_(0xA8160005L)	//Not found the Kernel DLL 
	#define E_RC_INTERN_AUTH_FAILED			_HRESULT_TYPEDEF_(0xA8160006L)	//Not passed inter authenication,maybe the dll and driver have been taken placed 
	#define E_RC_EXTERN_AUTH_FAILED			_HRESULT_TYPEDEF_(0xA8160007L)	//Not passed exter authenication,maybe the call is not legal 
	#define E_RC_INVALID_LIC				_HRESULT_TYPEDEF_(0xA8160008L)	//License is invalid 
	#define E_RC_NOT_FIND_FILE				_HRESULT_TYPEDEF_(0xA8160009L)	//Not found the corresponding file 
	#define E_RC_FILE_HAVEEXIST				_HRESULT_TYPEDEF_(0xA816000AL)	//The file has existed,can't be created again 
	#define E_RC_TIMEOUT					_HRESULT_TYPEDEF_(0xA816000BL)	//Time is out, can't find the server 
	#define E_RC_VERIFY_PASSWORD_FAILED		_HRESULT_TYPEDEF_(0xA816000CL)	//Verify the Password failed 
	#define E_RC_LOCK_PASSWORD				_HRESULT_TYPEDEF_(0xA816000DL)	//Password has been locked 
	#define E_RC_NOT_FIND_SERVER			_HRESULT_TYPEDEF_(0xA816000EL)	//Not found the server 
	#define E_RC_NOTMATCH					_HRESULT_TYPEDEF_(0xA816000FL)	//Dog ID is not matched 
	#define E_RC_NO_SUFFICENT_SPACE			_HRESULT_TYPEDEF_(0xA8160010L)	//While creating file or directory,if the free space is not enough ,this error will return 
	#define E_RC_NO_SUFFICENT_FREE_SPACE	_HRESULT_TYPEDEF_(0xA8160011L)	//While creating file or directory,if the useful free space is not enough ,this error will return 
	#define E_RC_NOT_PASS_EXTERN_AUTH		_HRESULT_TYPEDEF_(0xA8160012L)	//Not pass the extern authentication 
	#define E_RC_INVALID_COMMAND			_HRESULT_TYPEDEF_(0xA8160013L)	//Invalid command 
	#define E_RC_NOT_SUPPORT				_HRESULT_TYPEDEF_(0xA8160014L)	//not support the function,such as upgrade the netdog in the client machine 
	#define E_RC_NOT_CALL_OPEN_FIRST		_HRESULT_TYPEDEF_(0xA8160015L)	//Not call open first before open next 
	#define E_RC_SMALLBUFFER				_HRESULT_TYPEDEF_(0xA8160016L)	// Small buffer  
	#define E_RC_PASSWORD_NOT_VERIFIED		_HRESULT_TYPEDEF_(0xA8160017L)	// Password not verified  
	#define E_RC_INVALID_HANDLE				_HRESULT_TYPEDEF_(0xA8160018L)	// Handle to the Dog invalid  
	#define E_RC_CODE_DESTROYED				_HRESULT_TYPEDEF_(0xA8160019L)	// Api-Shell flag is wrong, code is cracked.  
 
 
	/******************************Shell tool error scope******************************/ 
	//from _HRESULT_TYPEDEF_(0xA8161000L) 
 
	/******************************Driver error scope******************************/ 
	//from HRESULT_TYPEDEF_(0xA8162000L) 
	#define E_RC_ADDRESS_OR_BYTES					_HRESULT_TYPEDEF_(0xA8162000L) 
	#define E_RC_MAPPING_SYSTEM_LIBRARY_FUNCTION	_HRESULT_TYPEDEF_(0xA8162001L)//Get the system function error 
	#define E_RC_LOAD_SYSTEM_LIBRARY				_HRESULT_TYPEDEF_(0xA8162002L)//Load system library error 
	#define E_RC_FIND_USB_DOG_DEVICE				_HRESULT_TYPEDEF_(0xA8162003L)//Not find the GrandDog USB device 
	#define E_RC_UPDATE_USB_DOG						_HRESULT_TYPEDEF_(0xA8162004L)//Update GrandDog USB driver error 
	#define E_RC_OPERATING_SYSTEM					_HRESULT_TYPEDEF_(0xA8162005L)//The operating system which can not be supported 
	#define E_RC_NO_PARALLEL_PORT_OR_DISABLED		_HRESULT_TYPEDEF_(0xA8162006L)//There is no Parallel port or the Parallel port is disabled. 
	#define E_RC_REMOVE_FILE_FAILED					_HRESULT_TYPEDEF_(0xA8162007L)//Delete driver file failed 
	#define E_RC_OPEN_DRIVER_SERVICE_FAILED			_HRESULT_TYPEDEF_(0xA8162008L)//Open driver service failed 
	#define E_RC_START_DRIVER_SERVICE_FAILED		_HRESULT_TYPEDEF_(0xA8162009L)//Start driver service failed 
	#define E_RC_COPY_FILE_FAILED					_HRESULT_TYPEDEF_(0xA816200AL)//Copy driver file failed 
	#define E_RC_OPERATE_REGISTRY_FAILED			_HRESULT_TYPEDEF_(0xA816200BL)//Access register failed 
	#define E_RC_NOT_ADMINISTRATOR					_HRESULT_TYPEDEF_(0xA816200CL)//The current user don't have adminstrator right 
	#define E_RC_TEST_FAILED						_HRESULT_TYPEDEF_(0xA816200DL)//Hardware test failed 
	#define E_RC_DEVICE_NOT_AVAILABLE				_HRESULT_TYPEDEF_(0xA816200EL)//The device can not response 
	#define E_RC_DOG_COUNT_IS_FULL					_HRESULT_TYPEDEF_(0xA816200FL)//The Dog's usage count is full 
 
	/******************************local machine dll error scope**************************/ 
	//from _HRESULT_TYPEDEF_(0xA8163000L) 
	#define E_RC_DATA_LENGTH						_HRESULT_TYPEDEF_(0xA8163000L)//Invalid data length 
	#define E_RC_DOG_CHECK							_HRESULT_TYPEDEF_(0xA8163001L)//Dog Check error 
	#define E_RC_FILE_TYPE							_HRESULT_TYPEDEF_(0xA8163002L)//Invalid file type 
	#define E_RC_GET_DEVICE_HANDLE					_HRESULT_TYPEDEF_(0xA8163003L)//Get Device handel error 
	#define E_RC_HOST_ID							_HRESULT_TYPEDEF_(0xA8163004L)//Host ID error 
	#define E_RC_DISK_ID							_HRESULT_TYPEDEF_(0xA8163005L)//Disk ID error 
	#define E_RC_PROCESS_ID							_HRESULT_TYPEDEF_(0xA8163006L)//Process ID error 
	#define E_RC_DOG_ID								_HRESULT_TYPEDEF_(0xA8163007L)//Dog ID error 
	#define E_RC_REMOVE_DOG_MSG						_HRESULT_TYPEDEF_(0xA8163008L)//Wrong remove dog message 
	#define E_RC_CLOSE_DOG							_HRESULT_TYPEDEF_(0xA8163009L)//Close Dog error 
	#define E_RC_KEY_TYPE							_HRESULT_TYPEDEF_(0xA816300AL)//Invalid key type 
	#define E_RC_BUFFER_LENGTH						_HRESULT_TYPEDEF_(0xA816300BL)//Buffer length error 
	#define E_RC_HACKER								_HRESULT_TYPEDEF_(0xA816300CL)//Hacker software error 
	#define E_RC_AGENT_ID							_HRESULT_TYPEDEF_(0xA816300DL)//Agent ID not match 
	#define	E_RC_NOT_FIND_NEXT_DOG					_HRESULT_TYPEDEF_(0xA816300EL)//Not found more corresponding Serial Number Hardware Dog 
	#define E_RC_NOT_FIND_NEXT_PRODUCT				_HRESULT_TYPEDEF_(0xA816300FL)//Not found more corresponding Product Number Hardware Dog 
	#define E_RC_NOT_SUPPORT_OPERATION_SYSTEM		_HRESULT_TYPEDEF_(0xA8163010L)//Not Support operation system type 
	#define E_RC_NOT_OPEN_DOG						_HRESULT_TYPEDEF_(0xA8163011L)//Not open dog 
	#define E_RC_UPGRADE_STRING						_HRESULT_TYPEDEF_(0xA8163012L)//Upgrade string is wrong 
	#define E_RC_UNKNOWN							_HRESULT_TYPEDEF_(0xA8162010L)//Unknown error 
	#define E_RC_DOG_RECV_INPUT_DATA				_HRESULT_TYPEDEF_(0xA8162011L)//Error happen when recieve the input data from dog 
	#define E_RC_DOG_SEND_OUTPUT_DATA				_HRESULT_TYPEDEF_(0xA8162012L)//Error happen when send the output data to dog 
 
	/******************************Developer API error scope**************************/ 
	//from _HRESULT_TYPEDEF_(0xA8164000L) 
	 
	// find more than one semi-dog  
	// 
 
	#define E_RC_MUTISEMIDOG						_HRESULT_TYPEDEF_(0xA8164000L) 
	 
#endif
