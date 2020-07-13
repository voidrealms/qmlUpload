#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "fileupload.h"

void testGet(FileUpload &fileupload)
{
    fileupload.getData("https://postman-echo.com/get?foo1=bar1&foo2=bar2");
}

void testPost(FileUpload &fileupload)
{
    QByteArray data;
    data.append("Hello world");

    fileupload.setHeader("content-type","text/html; charset=UTF-8");
    fileupload.postData("https://postman-echo.com/post",data);
}

void testPostFile(FileUpload &fileupload)
{
    QString path = "/home/rootshell/Code/Qt/test.txt";

    fileupload.setHeader("user-agent","Mozilla/5.0 (X11; Linux x86_64) Ubuntu Chromium/83 Chrome/83 "); //fake a specific user-agent
    fileupload.postFile("https://postman-echo.com/post",path,"form-data; name=\"notes\""); //part of a form, in the "notes" form field
}

void testPostPNG(FileUpload &fileupload)
{
    QString path = "/home/rootshell/Code/Qt/test.png";

    fileupload.setHeader("user-agent","Mozilla/5.0 (X11; Linux x86_64) Ubuntu Chromium/83 Chrome/83 "); //fake a specific user-agent
    fileupload.postFile("https://postman-echo.com/post",path,"form-data; name=\"notes\""); //part of a form, in the "notes" form field
}



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setOrganizationName("My Company");
    app.setOrganizationDomain("http://www.company.com");

    //Test in C++ if you are so inclined
    //Uncomment the tests you want to try in C++

    FileUpload fileupload;
    testGet(fileupload); //HTTP GET
    //testPost(fileupload); //HTTP POST Raw Data no encoding
    //testPostFile(fileupload);
    //testPostPNG(fileupload);

    //Register the file upload class
     qmlRegisterType<FileUpload>("com.company.fileupload",1,0,"FileUpload");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
