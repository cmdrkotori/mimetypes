#include <QDir>
#include <QSettings>
#include "mimetypes.h"
#include <QDebug>

MimeTypes::MimeTypes(QObject *parent) : QObject(parent)
{
    processFolder(QDir("/usr/share/applications"));
    processFolder(QDir(QDir::homePath() + "/.local/share/applications"));
}

MimeTypes::~MimeTypes()
{
    foreach(MimeTypeHandler *app, apps)
        delete app;
    apps.clear();
    appsByMimetype.clear();
}

QList<MimeTypeHandler *> MimeTypes::appsFor(QString mimeType)
{
    if (!appsByMimetype.contains(mimeType))
        return QList<MimeTypeHandler *>();
    return appsByMimetype[mimeType];
}

QStringList MimeTypes::mimeTypes()
{
    return appsByMimetype.keys();
}

void MimeTypes::processFolder(const QDir &dir)
{
    foreach (QFileInfo info, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        processFolder(QDir(info.absoluteFilePath()));
    }
    foreach (QFileInfo info, dir.entryInfoList({"*.desktop"}, QDir::Files)) {
        QList<MimeTypeHandler *> parsedApps = MimeTypeHandler::fromDesktopFile(info.absoluteFilePath());
        foreach (MimeTypeHandler *app, parsedApps)
            addApplication(app);
    }
}

void MimeTypes::addApplication(MimeTypeHandler *app)
{
    if (app->mime.isEmpty()) {
        delete app;
        return;
    }
    apps.append(app);
    foreach (QString m, app->mime) {
        appsByMimetype[m].append(app);
    }
}

QList<MimeTypeHandler *> MimeTypeHandler::fromDesktopFile(const QString &path)
{
    // Process a .desktop file, taking careful steps to ensure we only grab
    // as much as we need.
    QList<MimeTypeHandler *> list;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return list;

    MimeTypeHandler *currentApp = NULL;
    QStringList text = QString::fromUtf8(f.readAll()).split('\n');
    QString line;
    while (!text.isEmpty()) {
        line = text.takeFirst();
        // restart parsing on each section, optionally store
        if (line.startsWith('[') && currentApp) {
            // store only if meaningful data gathered
            if (!currentApp->exec.isNull())
                list.append(currentApp);
            else
                delete currentApp;
            currentApp = NULL;
        }
        // create a new app per desktop entry
        if (line == "[Desktop Entry]" && !currentApp) {
            currentApp = new MimeTypeHandler;
            continue;
        }
        // if no reason to process, keep parsing
        if (!currentApp || !line.contains('='))
            continue;
        // process the line and search for stuff
        QString tag = line.section('=', 0, 0);
        QString data = line.section('=', 1);
        if (tag=="Name")
            currentApp->name = data;
        else if (tag=="Exec")
            currentApp->exec = data;
        else if (tag=="Icon")
            currentApp->icon = data;
        else if (tag=="MimeType")
            currentApp->mime = data.split(';');
    }
    // clean up
    if (currentApp) {
        if (!currentApp->exec.isNull())
            list.append(currentApp);
        else
            delete currentApp;
    }
    return list;
}
