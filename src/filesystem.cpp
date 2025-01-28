/*
 * MIT License
 *
 * Copyright (c) 2025 Aciz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "filesystem.h"
#include "preferences.h"

// TODO: support PATH env variable for discovering Pack3r executable?
QString FileSystem::getPack3rPath(const QString &defaultPath) {
  QFileDialog fileDialog{};
  fileDialog.setWindowTitle(tr("Find Pack3r executable"));
  fileDialog.setFileMode(QFileDialog::ExistingFile);

  // Because the input here is the executable file, what we get is not actually
  // a path, but a filename. On Linux, due to a lack of file extension,
  // this means that the dialog tries to open a directory called Pack3r
  // from the directory where the executable is, which usually breaks
  // the file picker state initially, before a valid directory is selected.
  // Windows handles this gracefully and gives us the directory containing
  // Pack3r.exe even if we end the path with the filename, but let's properly
  // strip the executable name from the end and use the actual directory.

  // Note: use 'toNativeSeparators' here, because the input is the path
  // saved into settings, which DOES use native path separators
  if (defaultPath.endsWith(QDir::toNativeSeparators(PACK3R_EXECUTABLE),
                           Qt::CaseInsensitive)) {
    auto splits = defaultPath.split(NATIVE_PATHSEP);
    splits.removeLast();
    fileDialog.setDirectory(splits.join(NATIVE_PATHSEP));
  } else {
    fileDialog.setDirectory(QDir::homePath());
  }

  // on Windows, we can apply a .exe filter, but Linux needs to allow
  // all files, and later check for an executable flag
#ifdef Q_OS_WINDOWS
  fileDialog.setNameFilter(tr("Executable files (*.exe)"));
#else
  fileDialog.setNameFilter(tr("All files (*)"));
#endif

  const int ret = fileDialog.exec();

  if (ret == QDialog::Accepted) {
    const QString selectedFile = fileDialog.selectedFiles().first();

    if (!isValidPack3rBinary(selectedFile)) {
      return {};
    }

#ifdef Q_OS_WINDOWS
    return QDir::toNativeSeparators(selectedFile);
#else
    const QFileInfo fileInfo(selectedFile);

    if (fileInfo.isExecutable()) {
      return QDir::toNativeSeparators(selectedFile);
    } else {
      QMessageBox::critical(nullptr, "Invalid file",
                            "The selected file is not an executable file.");
    }
#endif
  }

  return {};
}

QString FileSystem::getMapPath(const QString &defaultPath) {
  return NATIVE_GETFILE(nullptr, tr("Open map file"),
                        getDefaultPath(defaultPath),
                        tr("Radiant map files (*.map *.reg)"));
}

QString FileSystem::getMappingPath(const QString &defaultPath) {
  return NATIVE_GETDIR(nullptr, tr("Choose mapping install location"),
                       getDefaultPath(defaultPath), QFileDialog::ShowDirsOnly);
}

QString FileSystem::getOutputPath(const QString &defaultPath) {
  return NATIVE_GETDIR(nullptr, tr("Choose output directory"),
                       getDefaultPath(defaultPath), QFileDialog::ShowDirsOnly);
}

QString FileSystem::getDefaultPath(const QString &defaultPath) {
  return defaultPath.isEmpty() ? QDir::homePath() : defaultPath;
}

bool FileSystem::isValidPack3rBinary(const QString &file, const bool silent) {
  if (!file.endsWith(PACK3R_EXECUTABLE, Qt::CaseInsensitive)) {
    if (!silent) {
      QMessageBox dialog{};
      Dialog::setupMessageBox(dialog,
                              Dialog::MessageBox::INVALID_PACK3R_BINARY);
      dialog.exec();
    }

    return false;
  }

  return true;
}
