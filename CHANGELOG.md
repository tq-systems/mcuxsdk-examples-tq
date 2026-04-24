# Changelog

All notable changes to this project will be documented in this file.
Releases are named with the following scheme:

`<Yocto Project version name>.<TQ module family>.BSP.SW.<version number>`

**NOTE:** For details to the changes in a release see the CHANGELOG.md
files in git the submodule for meta-tq (meta layers meta-tq and meta-dumpling).

[[_TOC_]]

## Next Release

### Added

### Changed

* MCUXSDK: upgrade to v26.03.00
* MCUXSDK: upgrade to v25.12.00
* CI: split archiving from build
* CI: do not depend on pandas

### Fixed

* CI: fix tar creation
* CI: ensure to use .venv and error on failure
* CI: ensure fail on errors

### Removed

## Initial Version

### Added

* Initial board support for TQMa117xL on MBa117xL
* Initial board support for TQMa93xx on MBa93xxCA
* Initial CI support