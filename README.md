dongletools
===========

dongletools is a simple collection of tools that allows you to back up and restore save data from a DJMax Trilogy Profile Key dongle.


OBLIGATORY DISCLAIMER
=====================

This tool has the power to brick your profile key if used incorrectly. I hold no responsibility for what might happen, am not liable, it's all provided without any express or implied warranties, blah blah blah. This has been tested by myself on 1.32 and nothing else, so tread carefully.
That aside, as long as you make an initial backup with it and keep it in a safe place your data should be fine. You can break the game if you let the tool write random data to your dongle. The format option should fix it, but no guarantees.


USAGE
=====

It's a commandline tool. Someone else can make a GUI for it if it matters. It works in the current directory, and doesn't do any sort of file management. So if you use it to dump a save it's your responsibility to move the files it generates to a more permanent backup folder - otherwise subsequent dumps will overwrite the old files.

Run it without arguments for a simple usage printout.

### Dongle Information
Run it with `dongletools.exe p` to have it print out some useless information - probably a good way to test that it can detect your dongle without actually doing anything dangerous.

### Dump Save
Run `dongletools.exe d` to dump the contents of your Profile Key Dongle to the current directory. It's now up to you to move them into their own backup folder for later restoration.

### Upload Save
Run `dongletools.exe u` to write the dumped files from the current directory to your dongle. That's about it - it will also upload partial dumps in case you only wanted to change one or two files for some reason rather than do a full restore.

### Format Save
Run `dongletools.exe f` to delete all files on your dongle and create an empty save on it. This is about the same as the Reset option in the Trilogy launcher, except it will work even when Trilogy starts rejecting the dongle outright if you've really messed it up. Also this way your MyDocuments/DJMaxTrilogy folder containing all of your highscores isn't deleted. But just in case, don't try this unless you have a backup.


PROFILE JUGGLING
----------------

I wrote this because I wanted to start a new game without losing my old data. The best way to do this is to make a dump of your current data, and keep it safe somewhere. Then use the format mode of dongletools to wipe the dongle and start a new game with it. Back up, restore, swap, etc. as you please.


CHEATING
========

While this isn't a save trainer tool, it does indeed enable savegame editing... C'est la vie. 3f00-6.bin can be edited to give yourself gold/experience/etc., and 3f00-3.bin seems to hold the majority of the game's data including unlockables, discs, emblems, etc. Just don't ask me about it.

