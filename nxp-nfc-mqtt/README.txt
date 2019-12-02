NFC Reader Library v4.010.03.001609 for PNEV512B
===========================

To extract the NFC Reader Library, please execute the installer and follow the instructions.

The installer for Windows is in the directory called Windows, for Linux the installer is in the directory called Linux.
On 64 bit Linux systems it is mandatory to install the mutliarch libraries first.
In Ubuntu Linux this can be done with the commands

sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install libc6:i386 libncurses5:i386 libstdc++6:i386

After installing the 32-bit libraries, please execute the installer with the command "./<name of the file>.run"