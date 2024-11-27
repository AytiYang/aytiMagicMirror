#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QMap>

class ApiManager : public QObject {
    Q_OBJECT

public:
    explicit ApiManager(QObject *parent = nullptr);
    void sendRequest(const QString &url, const QMap<QString, QString> &params);

signals:
    void jsonReceived(const QJsonObject &json);

private slots:
    void handleReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *createNetworkAccessManager();
};

#endif // APIMANAGER_H
