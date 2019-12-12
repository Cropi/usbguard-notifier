# USBGuard Notifier Release Process

1. Clean up any artifact from your local repository.

```
$ git clean -x -f -d
```

2. Update the AC_INIT field in `configure.ac` and `CHANGELOG.md`. Commit your changes.

3. Build the final release tarball.

```
$ ./autogen.sh
$ ./configure
$ make dist
```

4. Tag the release with a signed tag.

```
$ git tag -s -m "usbguard-notifier-0.x.y" usbguard-notifier-0.x.y
$ git push origin usbguard-notifier-0.x.x
```

5. Hash and sign the release.

```
$ sha256sum usbguard-notifier-0.x.y.tar.gz > usbguard-notifier-0.x.y.tar.gz.sum
$ gpg --armor --detach-sign usbguard-notifier-0.x.y.tar.gz
$ gpg --clearsign usbguard-notifier-0.x.y.tar.gz.sum
```

6. Create a new GitHub release using the associated tag; add the relevant section from CHANGELOG.md. Upload:

    * usbguard-notifier-0.x.y.tar.gz
    * usbguard-notifier-0.x.y.tar.gz.asc
    * usbguard-notifier-0.x.y.tar.gz.sum
    * usbguard-notifier-0.x.y.tar.gz.sum.asc

7. Edit(if needed) the new Github release.


