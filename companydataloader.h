#ifndef COMPANYDATALOADER_H
#define COMPANYDATALOADER_H

#include <QFile>
#include <QObject>
#include <QString>

class CompanyDataLoader : public QObject
{
    Q_OBJECT
public:
    explicit CompanyDataLoader(QObject *parent = nullptr);

    bool parseFile(QFile *file);

signals:

    void newDepartment(const QString &name);
};

#endif // COMPANYDATALOADER_H
