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

#pragma once

#include <QFileDialog>
#include <QObject>

#define NATIVE_GETFILE(parent, caption, dir, filter)                           \
  QDir::toNativeSeparators(                                                    \
      QFileDialog::getOpenFileName(parent, caption, dir, filter))
#define NATIVE_GETDIR(parent, caption, dir, filter)                            \
  QDir::toNativeSeparators(                                                    \
      QFileDialog::getExistingDirectory(parent, caption, dir, filter))

#define NATIVE_PATHSEP QDir::toNativeSeparators("/")

#ifdef Q_OS_WINDOWS
// QFileDialog returns forward slashes on Windows too,
// so we don't use native path separators here
#define PACK3R_EXECUTABLE "/Pack3r.exe"
#else
#define PACK3R_EXECUTABLE "/Pack3r"
#endif

class FileSystem : public QObject {
public:
  static QString getPack3rPath(const QString &defaultPath);
  static QString getMapPath(const QString &defaultPath);
  static QString getMappingPath(const QString &defaultPath);
  static QString getOutputPath(const QString &defaultPath);

  static QString getDefaultPath(const QString &defaultPath);
};
