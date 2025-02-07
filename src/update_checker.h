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

#include <QNetworkAccessManager>

class UpdateChecker : public QObject {
  Q_OBJECT

public:
  explicit UpdateChecker(QObject *parent = nullptr);

  enum ReleaseType {
    QTPACK3R_RELEASE,
    PACKER_RELEASE,
  };

  struct ReleaseInfo {
    ReleaseType type;
    QString tag;
    QDateTime releaseDate;
  };

  void init();
  void check();

#ifdef MOCK_UPDATES
  void mockCheck();
#endif

signals:
  void releasesReceived(const QList<ReleaseInfo> &releases);

private:
  void sendRequest(const QUrl &url) const;
  void onRequestFinished(QNetworkReply *reply);

#ifdef MOCK_UPDATES
  void mockOnRequestFinished(const QString &input, ReleaseType type);
#endif

  const QString userAgentHeader = QString("QtPack3r %1 update checker").arg(PROJECT_VERSION);
  const QUrl qtPack3rRepoAPI = QString("https://api.github.com/repos/Aciz/QtPack3r/releases");
  const QUrl pack3rRepoAPI = QString("https://api.github.com/repos/ovska/Pack3r/releases");

  QNetworkAccessManager *manager{};
};
