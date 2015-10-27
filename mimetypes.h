#ifndef MIMETYPES_H
#define MIMETYPES_H
#include <QObject>
#include <QMap>
#include <QList>
#include <QStringList>
#include <QDir>

struct MimeTypeHandler {
    QString name;
    QString exec;
    QString icon;
    QStringList mime;

    static QList<MimeTypeHandler *> fromDesktopFile(const QString &path);
};

class MimeTypes : public QObject
{
    Q_OBJECT
public:
    explicit MimeTypes(QObject *parent = 0);
    ~MimeTypes();
    QList<MimeTypeHandler *> appsFor(QString mimeType);
    QStringList mimeTypes();

signals:

public slots:

private:
    void processFolder(const QDir &dir);
    void addApplication(MimeTypeHandler *app);

    QList<MimeTypeHandler*> apps;
    QMap<QString, QList<MimeTypeHandler*>> appsByMimetype;
};

#endif // MIMETYPES_H
