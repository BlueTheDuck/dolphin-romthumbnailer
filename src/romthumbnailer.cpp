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

extern "C" {
Q_DECL_EXPORT ThumbCreator *new_creator() { return new RomThumbnailer(); }
}

RomThumbnailer::RomThumbnailer() {}

RomThumbnailer::~RomThumbnailer() {}

bool RomThumbnailer::create(const QString &path, int w, int h, QImage &img) {
    qCDebug(LOG_ROMTHUMBNAILER) << "Generating thumb for " << path << '\n';
    QImage out(QSize(w, h), QImage::Format::Format_ARGB32_Premultiplied);
    QFile  in(path);

    in.open(QIODevice::ReadOnly);

    if (path.endsWith(".nds")) {
        if (in.exists() || in.isReadable()) {
            QString path = ConfigData::dSCoversPath();

            qCDebug(LOG_ROMTHUMBNAILER) << "Thumbnails on: " << path << '\n';

            char game_code[4];
            NDS::get_rom_code(in, game_code);
            qCDebug(LOG_ROMTHUMBNAILER) << "ROM Code: " << game_code << '\n';

            QDir ds_covers(path);
            if (!ds_covers.exists()) {
                qCDebug(LOG_ROMTHUMBNAILER)
                    << "Path set on config  (" << ds_covers.path()
                    << ") does not exists" << '\n';
                return false;
            }
            ds_covers.setSorting(QDir::SortFlag::Name);
            ds_covers.setFilter(QDir::Filter::Files);

            QFileInfo cover_file;
            auto      file_list_info = ds_covers.entryInfoList();
            for (auto file : file_list_info) {
                if (file.baseName() == game_code) {
                    qCDebug(LOG_ROMTHUMBNAILER)
                        << "Found cover file: " << file.absolutePath() << '\n';
                    cover_file = file;
                    break;
                }
            }
            if (cover_file.exists()) {
                QImage cover_image(cover_file.absoluteFilePath());
                img = cover_image;
                return true;
            }
        } else {
            qCDebug(LOG_ROMTHUMBNAILER) << "Error!\n";
        }
    } else if (path.endsWith(".gba") || path.endsWith(".agb")) {
        out.fill(Qt::GlobalColor::blue);
        img = out;
        return true;
    }
    return false;
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