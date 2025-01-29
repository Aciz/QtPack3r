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

#include "pack3r_output_parser.h"

Pack3rOutputParser::Pack3rOutputParser(QObject *parent)
    : QObject(parent), cursorPos(0) {}

/* This basically implements a terminal-style text processing,
 * which handles carriage returns correctly.
 *
 * Both QTextEdit and QPlainTextEdit simply insert a newline when
 * they encounter a carriage return, so we must manually handle it here
 * and overwrite whatever is currently being processed.
 *
 * Additionally, we hold the processed output here until a newline char
 * is encountered, at which point we emit the signal to the UI that output
 * is ready to be displayed. This is because Pack3r might split a single
 * line into multiple outputs, so we can't simply append a newline
 * after each chunk of output we receive.
 *
 * TODO: because we hold each line here until a newline character is sent,
 *  we do net get an actual "live" output with the progress indicators that
 *  Pack3r tries to output. This would require interacting directly with
 *  the text field, which is a bit complicated. Revisit this in the future.
 */
void Pack3rOutputParser::processOutput(const QByteArray &data) {
  for (const auto &c : data) {
    if (c == '\r') {
      cursorPos = 0;
      continue;
    }

    if (c == '\n') {
      emit pack3rOutputProcessed(currentLine);
      currentLine.clear();
      cursorPos = 0;
      continue;
    }

    if (cursorPos >= currentLine.length()) {
      currentLine += c;
    } else {
      currentLine[cursorPos] = c;
    }

    cursorPos++;
  }
}

void Pack3rOutputParser::processVersion(const QByteArray &data) {
  // there's seemingly an empty string sent at the end of --version command,
  // ignore that so we don't overwrite the version with an empty string
  if (data.isEmpty()) {
    return;
  }

  // version strings have git hash appended to them, strip that out
  // if the version string does not contain '+' for some reason,
  // the entire string is included
  emit pack3rVersionParsed(data.mid(0, data.indexOf("+")));
}
