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
  return NATIVE_GETFILE(nullptr, tr("Find Pack3r executable"),
                        getDefaultPath(defaultPath), QString());
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
