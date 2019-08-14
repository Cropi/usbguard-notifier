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
  ./autogen.sh
  ./configure
  make
```

### Note

You might get an **IPC connection** error while trying to execute the binary program. It means the usbguard.service stopped working or you have not started it yet.


### Contribution

You may want to extend the features of USBGuard Notifier. Contributions are welcome and will be fully credited. We accept contributions via Pull Requests on Github.

### License

GNU General Public License v2.0
