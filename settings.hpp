#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QObject>
#include <QSettings>

class Settings : public QSettings
{
    Q_OBJECT
public:
    Settings(QString fileName, QSettings::Format format);
    void informUpdate();

signals:
    void settingsUpdate();

};

#endif // SETTINGS_HPP
