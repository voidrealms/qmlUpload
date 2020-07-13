#include "fileupload.h"

FileUpload::FileUpload(QObject *parent) : QObject(parent)
{
    connect(&m_manager, &QNetworkAccessManager::finished, this, &FileUpload::finished);
    connect(&m_manager, &QNetworkAccessManager::sslErrors, this, &FileUpload::sslErrors);
    connect(&m_manager, &QNetworkAccessManager::encrypted, this, &FileUpload::encrypted);
}

void FileUpload::setHeader(QString key, QString value)
{
    m_headers.insert(key,value);
}

void FileUpload::prepairRequest(QNetworkRequest *request)
{
    //You will need to change this to meet your specific situation
    qWarning() << "To Do:" << Q_FUNC_INFO;

    foreach(QString key, m_headers.keys())
    {
        request->setRawHeader(key.toUtf8(), m_headers.value(key).toUtf8());
    }
}

void FileUpload::getData(QString url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    prepairRequest(&request);

    QNetworkReply *reply = m_manager.get(request);
    Q_UNUSED(reply); // Do something with the reply if you want to
}

void FileUpload::postData(QString url, QByteArray data)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    prepairRequest(&request);

    QNetworkReply *reply = m_manager.post(request,data);
    Q_UNUSED(reply); // Do something with the reply if you want to
}

void FileUpload::postFile(QString url, QString path,  QString disposition)
{
    //use the QHttpMultiPart class
    //https://doc.qt.io/archives/qt-4.8/qhttpmultipart.html#details

    if(path.startsWith("file://")) path = path.remove(0,7);

    QFileInfo fi(path);
    if(!fi.exists())
    {
        qCritical() << "FILE NOTE FOUND: " << path << " in " << Q_FUNC_INFO;
        return;
    }

    QMimeDatabase db;
    QMimeType type = db.mimeTypeForFile(path); //change this if you don't agree with the mime type returned from QT
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(type.name()));
    part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(disposition));
    QFile *file = new QFile(path);
    file->open(QIODevice::ReadOnly);
    part.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

    multiPart->append(part);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    prepairRequest(&request);

    QNetworkReply *reply = m_manager.post(request,multiPart);
    multiPart->setParent(reply); // delete the multiPart with the reply

    m_files.insert(reply,file); //cleanup later
}

void FileUpload::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    //You will need to change this to meet your specific situation
    Q_UNUSED(reply); // Do something with the reply if you want to
    Q_UNUSED(errors); // Do something with the reply if you want to
    qWarning() << "To Do:" << Q_FUNC_INFO;
}

void FileUpload::encrypted(QNetworkReply *reply)
{
    //You will need to change this to meet your specific situation
    Q_UNUSED(reply); // Do something with the reply if you want to
    qWarning() << "To Do:" << Q_FUNC_INFO;
}

void FileUpload::finished(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );



    qWarning() << "If you have issues, this is where to look at the servers status back" << Q_FUNC_INFO;

    emit complete(statusCode.toString());

    QByteArray data = reply->readAll();
    qInfo() << "-----------------------";
    qInfo() << "Status:" << statusCode.toString();

    //Anything other that a Status 200 and you have a config issue to sort out between your app and your server code

    if(statusCode.toString() != "200")
    {
       QString reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
       qCritical() << "Looks like you have some troubleshooting to do :(" << Q_FUNC_INFO;
       qCritical() << reason;
    }

    qInfo() << "Length:" << data.length();
    qInfo() << "Data: " << data;
    qInfo() << "-----------------------";


    qWarning() << "To Do:" << Q_FUNC_INFO;

    if(m_files.contains(reply))
    {
        QFile *file = m_files.value(reply);
        qInfo() << "Removing pointer to file:" << file->fileName();
        file->close();
        file->deleteLater();
    }


    reply->deleteLater();
}


