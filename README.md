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
Pre-built binaries are available on the releases page (soontm)

## Dependencies
### Windows
* Make sure [Visual C++ Redistributable 2022 64-bit](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-microsoft-visual-c-redistributable-version) is installed
### Linux
* Install Qt6 with your distributions package manager
  * Requires **Qt 6.2** or newer

 # Planned features in the future
 * Drag & drop importing for maps
 * Proper theme support - currently Windows follows system light/dark preference, Linux uses `QT_QPA_PLATFORMTHEME`
   * Can be overridden with startup options or environmental variables, see [QStyle](https://doc.qt.io/qt-6/qstyle.html#details) and [QGuiApplication](https://doc.qt.io/qt-6/qguiapplication.html#supported-command-line-options) documentation
 * Ability to save option combinations as presets
 * Colored output for Pack3r's output

# Building
TODO

# Screenshot
<p align="center">
  <img
    title="QtPack3r"
    src="https://github.com/user-attachments/assets/9256c921-8d51-4233-b9ea-c885ebb7f5eb" />
</p>
