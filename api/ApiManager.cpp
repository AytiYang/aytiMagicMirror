#include "ApiManager.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QDebug>

ApiManager::ApiManager(QObject *parent) : QObject(parent) {
}

void ApiManager::sendRequest(const QString &url, const QMap<QString, QString> &params) {
    QNetworkAccessManager *manager = createNetworkAccessManager();

    QUrl qurl(url);
    QUrlQuery query;
    for (auto it = params.begin(); it != params.end(); ++it) {
        query.addQueryItem(it.key(), it.value());
    }
    qurl.setQuery(query);

    QNetworkRequest request(qurl);

    connect(manager, &QNetworkAccessManager::finished, this, &ApiManager::handleReply);

    manager->get(request);
}

void ApiManager::handleReply(QNetworkReply *reply) {
    QNetworkAccessManager *manager = qobject_cast<QNetworkAccessManager *>(sender());
    if (manager) {
        manager->deleteLater();
    }

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QString responseString = QString::fromUtf8(response);
        qDebug() << "Response:" << responseString;

        // 解析JSON数据
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        if (!jsonResponse.isNull() && jsonResponse.isObject()) {
            QJsonObject jsonObject = jsonResponse.object();
            emit jsonReceived(jsonObject);
        } else {
            qDebug() << "Failed to parse JSON response.";
        }
    } else {
        qDebug() << "Error:" << reply->errorString();
    }
    reply->deleteLater();
}

QNetworkAccessManager *ApiManager::createNetworkAccessManager() {
    return new QNetworkAccessManager(this);
}
