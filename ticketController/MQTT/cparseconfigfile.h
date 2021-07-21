#ifndef CPARSECONFIGFILE_H
#define CPARSECONFIGFILE_H

#include <QObject>
#include "cconnectinfo.h"

class cParseConfigFile : public QObject
{
    Q_OBJECT
public:
    explicit cParseConfigFile(QObject *parent = nullptr);
    ~cParseConfigFile();
    cConnectInfo getConfigurationData();
    void setConfigurationData(cConnectInfo conInfo);
};


#endif // CPARSECONFIGFILE_H
