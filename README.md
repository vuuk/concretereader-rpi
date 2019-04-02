# Development environment setup guide
This guide is for cross-compiling Qt programs using Boost. The host computer is running Ubuntu and the target is a Raspberry Pi 3 running Raspbian Stretch Lite.
### Guides used
* [mechatronicsblog - cross compiling qt for rpi](https://mechatronicsblog.com/cross-compile-and-deploy-qt-5-12-for-raspberry-pi/)
* [wiki.qt.io - cross compiling qt for rpi](https://wiki.qt.io/RaspberryPi2EGLFS)
* [yadoms - cross-compiling for rpi with boost](https://github.com/Yadoms/yadoms/wiki/Cross-compile-for-raspberry-PI)
* [Updating the compiler included in raspberrypi/tools](https://mechatronicsblog.com/cross-compile-and-deploy-qt-5-12-for-raspberry-pi/#comment-21)

# Setting up the Rpi
Download [Raspbian Strecth Lite](https://www.raspberrypi.org/downloads/raspbian/)

## Setting up ssh and the Rpi firmware
Enter the following console commands.
```console
pi@raspberry:~ $ sudo rpi-update
pi@raspberry:~ $ reboot
pi@raspberry:~ $ sudo raspi-config
```
* Set the available GPU memory to 256
* Enable ssh
* Enable console without login
```console
pi@raspberry:~ $ reboot
```
> Follow [this guide](https://www.raspberrypi.org/documentation/remote-access/ssh/passwordless.md) to set up passwordless ssh

Uncomment the line beginning with: *deb-src*
```console
pi@raspberry:~ $ nano /etc/apt/sources.list
```
## Building the required libraries on the Rpi
Run the following commands.
```console
pi@raspberry:~ $ sudo apt-get update
pi@raspberry:~ $ sudo apt-get build-dep qt4-x11
pi@raspberry:~ $ sudo apt-get build-dep libqt5gui5
pi@raspberry:~ $ sudo apt-get install libudev-dev libinput-dev libts-dev libxcb-xinerama0-dev libxcb-xinerama0
pi@raspberry:~ $ sudo apt-get install gdbserver /*For debugging.*/
```
## Setting up the target directories on the Rpi
Run the following commands. This will create the directories needed later
```console
pi@raspberry:~ $ sudo mkdir /usr/local/qt5pi  /*Directory where the Qt libraries will be put*/
pi@raspberry:~ $ sudo chown pi:pi /usr/local/qt5pi  /*Makes it so the pi user can write to it*/
pi@raspberry:~ $ mkdir /home/pi/lib_tmp  /*Temporary directory where the boost libs will be put*/
```

# Setting up the Host Computer
## Setting up the build environment
Start out by making all the environment directories. The sysroot folder will be used to mimic the Rpi. It is therefore important that, you make sure that all object-files that are copied onto the Rpi, are compiled with a compiler that is compatible with the Rpi. Use the compilers in the raspberrypi toolset we will clone in this section.
```console
user@hostpc:~$ mkdir ~/raspi ~/raspi/sysroot
user@hostpc:~$ cd ~/raspi
user@hostpc:~$ git clone https://github.com/raspberrypi/tools
user@hostpc:~$ mkdir sysroot/usr sysroot/opt
```
## Updating the compiler in ~/raspi/tools
Download the compiler archive namned: *gcc-linaro-7.4.1-2019.02-x86_64_arm-linux-gnueabihf* from [this page](https://releases.linaro.org/components/toolchain/binaries/latest-7/).\
\
Now move the contents of the directory ~/raspi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/ to a different directory and
copy the contents of the extracted archive into ~/raspi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/
## Compiling boost
For more information, please check out [this guide](https://github.com/Yadoms/yadoms/wiki/Cross-compile-for-raspberry-PI).
1. Add the following line to your .bashrc-file in the $HOME directory.
```console
PATH=$PATH:$HOME/raspi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin
```
2. Restart the terminal
3. Download boost and extract it
```console
user@hostpc:~$ wget https://sourceforge.net/projects/boost/files/boost/desired_boost_version.bz2
```
4. Go into the extracted directory and run the following command
```console
user@hostpc:~/boost_dir$ ./bootstrap.sh
```
5. Create a user-config.jam file and add the line below to it.
```console
user@hostpc:~/boost_dir$ nano ~/user-config.jam
``` 
  > using gcc : arm : arm-linux-gnueabihf-g++ ;
6. Now compile boost
```console
user@hostpc:~/boost_dir$ ./b2 toolset=gcc-arm link=static cxxflags=-fPIC
``` 
7. Remove the user-config.jam
```console
user@hostpc:~/boost_dir$ rm ~/user-config.jam
``` 
8. Done! For now.. Later we will move the compiled object-files onto the Rpi.

## Compiling Qt
### Setting up the local Rpi-enviornment
First set up the local rpi-environment issuing these commands.
```console
user@hostpc:~$ cd ~/raspi
user@hostpc:~/raspi$ rsync -avz pi@your.rpis.ip.address:/lib sysroot
user@hostpc:~/raspi$ rsync -avz pi@your.rpis.ip.address:/usr/include sysroot/usr
user@hostpc:~/raspi$ rsync -avz pi@your.rpis.ip.address:/usr/lib sysroot/usr
user@hostpc:~/raspi$ rsync -avz pi@your.rpis.ip.address:/opt/vc sysroot/opt
```
This will copy all the libraries needed to simulate the rpi-environment to the host pc.\
Now we just have to set-up the symlinks using a python-script.
```console
user@hostpc:~/raspi$ wget https://raw.githubusercontent.com/riscv/riscv-poky/master/scripts/sysroot-relativelinks.py
user@hostpc:~/raspi$ chmod +x sysroot-relativelinks.py
user@hostpc:~/raspi$ ./sysroot-relativelinks.py sysroot
```
### Download qt
The only Qt version I've tried to setup is 5.9 since it offers all the features I need. I'm sure later version can be made to work as well, but I haven't tested it.\
Download qt into the directory *~/raspi*
```console
user@hostpc:~/raspi$ git clone git://code.qt.io/qt/qtbase.git -b <5.9 or your desired version>
```
When the download is finished edit the file: *~/raspi/qtbase/mkspecs/devices/**your Rpi version**/qmake.conf*\
and replace every occurence of **-lEGL** and **-LGLESv2** with **-lbrcmEGL** and **-lbrcmGLESv2** respectively.\
From the directory ~/raspi/qtbase run the following command:
```console
user@hostpc:~/raspi/qtbase$ ./configure -release -opengl es2 -device linux-rasp-pi3-g++ -device-option CROSS_COMPILE=~/raspi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf- -sysroot ~/raspi/sysroot -opensource -confirm-license -make libs -prefix /usr/local/qt5pi -extprefix ~/raspi/qt5pi -hostprefix ~/raspi/qt5 -no-use-gold-linker -v -no-gbm
user@hostpc:~/raspi/qtbase$ make -j 4
user@hostpc:~/raspi/qtbase$ make install
```
Note that the compiler specified in the previous command is the same used to compile boost.\
If the compiling fails you can reset the directory by issuing:
```console
user@hostpc:~/raspi/qtbase$ git clean -dfx
```
### Deploying Qt and Boost on the Rpi
```console
user@hostpc:~/raspi$ rsync -avz qt5pi pi@raspberrypi_ip:/usr/local
user@hostpc:~/raspi$ scp full/path/to/boost_1_XX_X/stage/lib/* pi@your.rpis.ip.address:/home/pi/lib_tmp
```
Now on the Rpi issue the following commands:
```console
pi@raspberry:~$ sudo mv lib_tmp/* /usr/local/lib
pi@raspberry:~$ rmdir lib_tmp
pi@raspberry:~$ sudo ldconfig
pi@raspberry:~$ reboot
```
All compilations are done, now it's time to setup the gdbserver on the Rpi and Qt Creator.
# Setting everything up
## Setting Qt Creator up
Download and install Qt Creator.
### Now we'll set up the Rpi development kit.
  1. **Tools -> Options -> Devices -> Devices Tab -> Add (a new Generic Linux Device)**\
Enter the Rpi:s details.

  2. **Tools -> Options -> Kits -> Compilers tab -> Add (GCC->C and GCC->C++)**\
  Add the GCC and G++ compilers from the directory we used when compiling Qt. From this guide it would be:\
  \
  */home/user/raspi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc*\
   */home/user/raspi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-g++*
   
 3. **Tools -> Options -> Kits -> Debuggers tab -> Add**\
  Select the GDB from the directory used when compiling qt. From this guide it would be:\
 \
 */home/user/raspi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gdb*
 
 4. **Tools -> Options -> Kits -> Qt Versions -> Add**\
 Select the qmake located in the recently compiled qtdirectory. From this guide it would be\
 \
 */home/user/raspi/qt5/bin/qmake*
 
 5. **Tools -> Options -> Kits -> Kits tab -> Add (Enter the following settings)**
  > Device type: Generic Linux Device\
  > Device: *The name of the Raspberry Pi device we set up previously*\
  > Sysroot: ~/raspi/sysroot\
  > Compiler C: *the GCC-compiler we added previously*\
  > Compiler C++: *the G++-compiler we added previously*\
  > Debugger: *the GDB-debugger we added previously*\
  > Qt version: *the Qt version we added previously*
  
## Setting up debugging with gdbserver running on the Rpi
### Setting up the gdbserver
```console
pi@raspberry:~$ gdbserver :<PORT NR> <PROGRAM TO BE DEBUGGED>
```
### Making Qt Creator connect to the server
In Qt Creator do the following:\
**Debug -> Start Debugging -> Attach to running debug server**
> Kit: the Rpi kit we setup.\
> Server Port: Your chosen server port\
> Local executable: The executable of the program on the Host PC

## Editing the qmake file of the project
Now editing the qmake-file of the project you're cross-compiling. You have to add the following lines to make sure the boost libraries are linked properly and that Qt Creator recognizes the boost-libraries. 
```qmake
INCLUDEPATH += /path/on/host/pc/to/boost_1_XX_0/

LIBS += -L/path/on/host/pc/to/lib/boost_1_XX_0/stage/lib -lboost_system (for example)

target.path = /home/pi
INSTALLS += target
```
# Problems that might occur
## The editor in Qt Creator does not recognize the Qt libraries in the code
Go to **Help->About\ Plugins** and deactivate ClangCodeModel och ClangFormat.

## Set physical width and height of the Rpi screen.
```console
pi@raspberry:~$ sudo nano ~/.profile
```
Add the following two lines to the file.
 > export QT_QPA_EGLFS_PHYSICAL_WIDTH=<YOUR SCREEN'S WIDTH IN MILLIMETERS>\
 > export QT_QPA_EGLFS_PHYSICAL_HEIGHT=<YOUR SCREEN'S HEIGHT IN MILLIMETERS>
 
Write out and run the command:
```console
pi@raspberry:~$ source ~/.profile
```
## Installing additional modules for Qt
As explained in the guide [wiki.qt.io - cross compiling qt for rpi](https://wiki.qt.io/RaspberryPi2EGLFS).\
Do the same as when you installed the qtbase.
```console
user@hostpc:~$ git clone git://code.qt.io/qt/<qt-module>.git -b <qt-version>
user@hostpc:~$ cd <qt-module>
user@hostpc:~$ ~/raspi/qt5/bin/qmake
user@hostpc:~$ make
user@hostpc:~$ make install
```
