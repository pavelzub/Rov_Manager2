#include "settings.hpp"

Settings::Settings(QString fileName, Format format):
    QSettings(fileName, format)
{
}

void Settings::informUpdate()
{
    emit settingsUpdate();
}
