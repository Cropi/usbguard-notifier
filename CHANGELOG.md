### 0.1.0 - 2022-12-19

## Added

- Add "--with-usbguard-devel" configuration option to compile with usbguard-devel source files.
- Merge notifications when inserting devices. This reduces the amount of notifications by 1 when inserting a device, which is allowed by the policy.
- Introduce action buttons to allow or reject devices from notifications.

## Fixed

- Decrease spam when IPC connection fails. In case connection through the IPC can not be established, do not produce unnecessary warning messages.
- Change the injection of the usbguard-icon.
- Update Catch2 submodule to the latest stable 2.x.

### 0.0.6 - 2020-04-29

## Added

- Configuration option that disables building notifier-cli

## Fixed

- Fix segmentation fault when login can not be determined

## Removed

- Remove unnecessary dependency(libqb>=1.0.5), which does not exist in some distributions yet. We do not use libqb directly, so we let usbguard decide which version they do want as a requirement.

### 0.0.5 - 2020-03-02

## Added

- Pass FLAGS to configure script

### 0.0.4 - 2020-02-24

## Added

- Configuration file now has an additional option to use either systemd-system-unitdir or systemd-user-unitdir

### 0.0.3 - 2020-02-14

## Fixed

- Configure.ac has been updated to ensure that a recent version of Autoconf is being used
- Added missing dependencies to the documentation

### 0.0.2 - 2019-12-12

## Added

- There is no need to restart Notifier in case Usbguard stops working.
- Debug option in configure script

## Fixed

- Temporary notifications path can be set at the configuration phase
- README.md update
- Debug information when the connection is established successfully

## Removed

- Unnecessary dependencies have been removed from configuration

### 0.0.1 - 2019-11-07

- Initial pre-release
