# USBGuard Notifier

[![Build Status](https://travis-ci.org/Cropi/usbguard-notifier.svg?branch=master)](https://travis-ci.org/Cropi/usbguard-notifier)
[![License](https://img.shields.io/github/license/Cropi/usbguard-notifier.svg)](LICENSE)

### About

USBGuard Notifier is a software framework mainly for detecting usbguard policy modifications as well as device presence changes. In a nutshell, the purpose of this project is to create user-friendly notifications in form of quick pop-up messages.

### Dependencies
In order to compile the sources from a tarball/zip you will need the following development files:
* [usbguard](https://github.com/USBGuard/usbguard/)
* [libnotify](https://github.com/GNOME/libnotify)


### Instalation

It is pretty easy to install the USBGuard Notifier. There are two possibilities how to install it:
* per user (locally)
```
$  ./autogen.sh
```
Now you can choose if you would like to store the notifications temporarily(until reboot):
```
$  ./configure --prefix /home/$(id -un)
```
or permanently:
```
$ ./configure --prefix /home/$(id -un) --enable-permanent-notifications
```

```
$  make
$  make install SYSTEMD_UNIT_DIR=/home/$(id -un)/.config/systemd/user/
$  systemctl enable --now --user usbguard-notifier.service
```

### Note

You might get an **IPC connection** error while trying to execute the binary program. It means the usbguard.service stopped working or you have not started it yet.

If you want to compile the sources in a cloned repository, you’ll have to run the ./autogen.sh script. It will fetch the sources (via git submodule) of [CATCH](https://github.com/catchorg/Catch2/). The script will then initialize the autotools based build system. 

USBGuard Notifier has to use the same version of usbguard-devel package that USBGuard uses in your system.

### Contribution

You may want to extend the features of USBGuard Notifier. Contributions are welcome and will be fully credited. We accept contributions via Pull Requests on Github.

### License

GNU General Public License v2.0
