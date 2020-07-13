#ifndef FILEUPLOAD_H
#define FILEUPLOAD_H

/*
 * ABANDON HOPE ALL YE WHO ENTER!!!
 * Will not magically solve your problems, you will need to change this to your specific use case
 *
 * File uploader
 * Uses QNetWorkAccess manager to HTTP POST a file
 * Can be used by C++ / QML

 * Author: Bryan Cairns
 * Web: http://ww.voidrealms.com
 */

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFile>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QVariant>

class FileUpload : public QObject
{
    Q_OBJECT
public:
    explicit FileUpload(QObject *parent = nullptr);

public slots:
    void setHeader(QString key, QString value);
    void getData(QString url);
    void postData(QString url, QByteArray data);
    void postFile(QString url, QString path, QString disposition);

private slots:
    void encrypted(QNetworkReply *reply);
    void finished(QNetworkReply *reply);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
signals:

    void complete(QString status);

private:
    QNetworkAccessManager m_manager;
    QMap<QString,QString> m_headers;
    QMap<QNetworkReply*,QFile*> m_files;
    void prepairRequest(QNetworkRequest *request);


};

#endif // FILEUPLOAD_H
