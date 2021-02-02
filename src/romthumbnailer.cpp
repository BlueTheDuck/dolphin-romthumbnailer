#include "romthumbnailer.h"

#include <QDir>
#include <QFile>
#include <QImage>
#include <QLoggingCategory>
#include <qbuffer.h>
#include <qcompilerdetection.h>
Q_DECLARE_LOGGING_CATEGORY(LOG_ROMTHUMBNAILER)
Q_LOGGING_CATEGORY(LOG_ROMTHUMBNAILER, "romthumbnailer")

#include "configdata.h"
#include "configui.h"
#include "nds.h"

RomThumbnailer::RomThumbnailer() {}

RomThumbnailer::~RomThumbnailer() {}

bool RomThumbnailer::create(const QString &path, int w, int h, QImage &icon) {
    qCDebug(LOG_ROMTHUMBNAILER)
        << "Generating thumb for " << path << " Size: " << QSize(w, h);

    std::unique_ptr<QFile> file(new QFile(path));
    bool                   return_status = false;

    file->open(QIODevice::OpenModeFlag::ReadOnly);

    if (path.endsWith(".nds")) {
        if (file->exists() || file->isReadable()) {
            NDS  nds(std::move(file));
            auto code = nds.get_rom_code();
            qCDebug(LOG_ROMTHUMBNAILER) << "ROM Code: " << code << '\n';
            nds.get_icon(icon);
            icon =
                icon.scaled(QSize(w, h), Qt::AspectRatioMode::KeepAspectRatio,
                            Qt::TransformationMode::SmoothTransformation);
            return_status = true;
        }
    } else if (path.endsWith(".gba") || path.endsWith(".agb")) {
        icon.fill(Qt::GlobalColor::blue);
        return_status = true;
    }
    if (!return_status) {
        qCCritical(LOG_ROMTHUMBNAILER) << "An error has ocurred.";
        icon.fill(Qt::GlobalColor::red);
        return_status = true;
    }
    return return_status;
}

QWidget *RomThumbnailer::createConfigurationWidget() {
    auto ui = new ConfigUi();
    ui->setUseFrames(ConfigData::useFrames());
    ui->setDSCoversPath(ConfigData::dSCoversPath());
    return ui;
}

void RomThumbnailer::writeConfiguration(const QWidget *configurationWidget) {
    ConfigData *    config_data = ConfigData::self();
    const ConfigUi *configui =
        qobject_cast<const ConfigUi *>(configurationWidget);
    config_data->setUseFrames(configui->getUseFrames());
    config_data->setDSCoversPath(configui->getDSCoversPath().url(
        QUrl::UrlFormattingOption::RemoveScheme));
    config_data->save();
}

ThumbCreator::Flags RomThumbnailer::flags() const { return ThumbCreator::None; }
