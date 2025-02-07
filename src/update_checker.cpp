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

#include "update_checker.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <qfile.h>

UpdateChecker::UpdateChecker(QObject *parent) : QObject(parent) {}

void UpdateChecker::init() {
  manager = new QNetworkAccessManager(this);

  connect(manager, &QNetworkAccessManager::finished, this,
          &UpdateChecker::onRequestFinished);
}

void UpdateChecker::check() {
  // TODO: check update frequency from preferences
  sendRequest(qtPack3rRepoAPI);
  sendRequest(pack3rRepoAPI);
}

void UpdateChecker::sendRequest(const QUrl &url) const {
  QNetworkRequest request(url);

  // GitHub API requires a user agent header
  request.setHeader(QNetworkRequest::UserAgentHeader, userAgentHeader);
  manager->get(request);
}

void UpdateChecker::onRequestFinished(QNetworkReply *reply) {
  if (reply->error() == QNetworkReply::NoError) {
    const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    const QJsonObject release = doc.object();

    QList<ReleaseInfo> releases{};
    releases.reserve(doc.array().size());

    for (const auto &value : doc.array()) {
      QJsonObject obj = value.toObject();

      // ignore drafts and pre-releases
      // TODO: support pre-releases with a different update "channel"?
      //  Pack3r might not have pre-release updates ever, but QtPack3r might
      if (obj["draft"].toBool() || obj["prerelease"].toBool()) {
        continue;
      }

      ReleaseInfo info{};
      info.tag = obj["tag_name"].toString();
      info.releaseDate = QDateTime::fromString(obj["published_at"].toString(), Qt::ISODate);

      if (reply->url() == qtPack3rRepoAPI) {
        releases.append(info);
      } else if (reply->url() == pack3rRepoAPI) {
        releases.append(info);
      } else {
        qDebug() << QString(tr("%1: unable to get latest version: unknown URL").arg(__func__));
      }
    }

    emit releasesReceived(releases);
  } else {
    qDebug() << QString(tr("%1: request returned error: ").arg(__func__))
             << reply->errorString();
  }
}

#ifdef MOCK_UPDATES
void UpdateChecker::mockCheck() {
  mockOnRequestFinished("../misc/qtpack3r_release_api_response.json", QTPACK3R_RELEASE);
  mockOnRequestFinished("../misc/pack3r_release_api_response.json", PACKER_RELEASE);
}

// TODO: ideally this should be the exact same function as the real deal
//  to ensure correctness in parsing the response, refactor!
void UpdateChecker::mockOnRequestFinished(const QString &input, const ReleaseType type) {
  QFile file = input;
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  const auto contents = file.readAll();
  const QJsonDocument doc = QJsonDocument::fromJson(contents);
  QList<ReleaseInfo> releases{};
  releases.reserve(doc.array().size());

  for (const auto &value : doc.array()) {
    QJsonObject obj = value.toObject();

    if (obj["prerelease"].toBool() || obj["draft"].toBool()) {
      continue;
    }

    ReleaseInfo info{};
    info.tag = obj["tag_name"].toString();
    info.releaseDate = QDateTime::fromString(obj["published_at"].toString(), Qt::ISODate);
    info.type = type;
    releases.append(info);
  }

  emit releasesReceived(releases);
  file.close();
}
#endif
