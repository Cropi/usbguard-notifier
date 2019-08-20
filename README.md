# USBGuard Notifier

### About

USBGuard Notifier is a software framework mainly for detecting usbguard policy modifications as well as device presence changes. In a nutshell, the purpose of this project is to create user-friendly notifications in form of quick pop-up messages.

### Dependencies
In order to compile the sources from a tarball/zip you will need the following development files:
* [usbguard](https://github.com/USBGuard/usbguard/)
* [libnotify](https://github.com/GNOME/libnotify)

### Compile & configure

USBGuard Notifier utilizies the features of autools. The following commands are required to build the project.

```
$  ./autogen.sh
$  ./configure
$  make
```

### Instalation

It is pretty easy to install the USBGuard Notifier. There are two possibilities how to install it:
* per user (locally)
```
$  ./autogen.sh
$  ./configure --prefix /home/$(id -un)
$  make
$  make install SYSTEMD_UNIT_DIR=/home/$(id -un)/.config/systemd/user/
$  systemctl enable --now --user usbguard_notifier.service
```
* system wide
```
$  ./autogen.sh
$  ./configure --prefix /usr
$  make
$  sudo make install
$  systemctl enable --now --user usbguard_notifier.service
```

### Note

You might get an **IPC connection** error while trying to execute the binary program. It means the usbguard.service stopped working or you have not started it yet.


### Contribution

You may want to extend the features of USBGuard Notifier. Contributions are welcome and will be fully credited. We accept contributions via Pull Requests on Github.

### License

GNU General Public License v2.0
