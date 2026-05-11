# Changelog

All notable changes to this project will be documented in this file.
Releases are named with the following scheme:

`<Yocto Project version name>.<TQ module family>.BSP.SW.<version number>`

**NOTE:** For details to the changes in a release see the CHANGELOG.md
files in git the submodule for meta-tq (meta layers meta-tq and meta-dumpling).

[[_TOC_]]

## Next Release

### Added

* TQMa93xx: add RPMSG demo
* Initial board support for TQMa8MPxS on MB-SMARC-2
* Initial board support for TQMa8MPxL on MBa8MPxL

### Changed

* TQMa93xx: add debugger configuration and templates

* TQMa93xx: add and use board common pin_mux.c

  A common file can be used when only debug UART is used.
* TQMa93xx: cleanup board code

  Remove unused I2C support code, remove outdated *.mex file
* MCUXSDK: upgrade to v26.03.00
* MCUXSDK: upgrade to v25.12.00
* CI: split archiving from build
* CI: do not depend on pandas

### Fixed

* TQMa93xx: use generic SDK linker files instead of local copy of old LPA demo linker file
* CI: fix tar creation
* CI: ensure to use .venv and error on failure
* CI: ensure fail on errors

### Removed

## Initial Version

### Added

* Initial board support for TQMa117xL on MBa117xL
* Initial board support for TQMa93xx on MBa93xxCA
* Initial CI support