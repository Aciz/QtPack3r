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

#include "pack3r_process_handler.h"
#include "dialog.h"

#include <QSaveFile>

Pack3rProcessHandler::Pack3rProcessHandler(
    QObject *parent, const QPointer<Pack3rOutputParser> &outputParser)
    : QObject(parent), process(new QProcess(this)), parser(outputParser) {}

void Pack3rProcessHandler::spawnProcess(
    const QPair<QString, QStringList> &command, const QString &outputFile) {
  process->setProgram(command.first);
  process->setArguments(command.second);

  // TODO: should probably refactor this function to take some sort of
  //  'type' argument on what workload we're running, this is kinda ugly
  isVersionCheck = process->arguments().join("") == "--version";
  currentOutputFile = outputFile;
  overWritePrompted = false;

  connect(process, &QProcess::readyReadStandardOutput, this,
          &Pack3rProcessHandler::readStdOut);
  connect(process, &QProcess::readyReadStandardError, this,
          &Pack3rProcessHandler::readStdErr);

  process->start();
}

void Pack3rProcessHandler::readStdOut() {
  const auto out = process->readAllStandardOutput();

  if (isVersionCheck) {
    parser->processVersion(out);
    return;
  }

  parser->processOutput(out);

  Q_ASSERT(!currentOutputFile.isEmpty());

  if (!overWritePrompted && out.contains("Overwrite? Y/N")) {
    overWritePrompted = true;

    QMessageBox dialog{};
    Dialog::setupMessageBox(dialog, Dialog::OVERWRITE);
    dialog.setText(tr("File '%1' already exists!").arg(currentOutputFile));

    const int ret = dialog.exec();

    if (ret == QMessageBox::Yes) {
      process->write("y\n");
    } else {
      process->write("n\n");
      parser->processOutput("Operation canceled\n");
    }
  }
}

// Pack3r at the moment doesn't actually send anything to stderr,
// but this is here for the future
void Pack3rProcessHandler::readStdErr() const {
  const auto err = process->readAllStandardError();
  parser->processOutput(err);
}
