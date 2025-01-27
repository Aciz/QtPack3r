<p align="center">
  <h1 align="center">QtPack3r</h1>
  <h3 align="center">A graphical user interface for <a href=https://github.com/ovska/Pack3r>Pack3r</a>, written in C++ and Qt.</h3>
</p>

&nbsp;

# Features
* Full support for all features of Pack3r
* Native, fast, cross-platform user interface with small installation size
* Extra safeguards for usage - ensures maps are processed from a valid mapping installation
* Persistent configuration for Pack3r and mapping install locations

# Installation
Pre-built binaries are available on the [releases page](https://github.com/Aciz/QtPack3r/releases).

Note that this repository only provides a user interface, it does **NOT** contain Pack3r itself. You must download it separately. The `Help` menu contains a link to Pack3r's latest release.

## Dependencies
### Windows
* Make sure [Visual C++ Redistributable 2022 64-bit](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-microsoft-visual-c-redistributable-version) is installed
### Linux
* Install Qt6 with your distributions package manager
    * Requires **Qt 6.2** or newer
 
# Reporting issues
If you encounter a bug while using QtPack3r, before making a bug report, please ensure that the issue is with QtPack3r itself and not Pack3r. If you're having trouble executing commands, or the results of an executed command is not what you expect, try running the command directly from the command line. The UI provides a copy button next to the command preview for a convenient way to copy the current command. If the issue you're experiencing persist while running the command directly, it's likely an issue with Pack3r itself, and you should [report the bug there](https://github.com/ovska/Pack3r/issues).

# Building
See [BUILDING.md](BUILDING.md).

# Screenshot
<p align="center">
  <img
    title="QtPack3r"
    src="https://github.com/user-attachments/assets/9256c921-8d51-4233-b9ea-c885ebb7f5eb" />
</p>
