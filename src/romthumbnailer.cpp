#include "romthumbnailer.h"
#include <QDir>
#include <QFile>
#include <QImage>
#include <qbuffer.h>
#include <qcompilerdetection.h>

#include <QtWidgets/QLabel>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(LOG_ROMTHUMBNAILER)
Q_LOGGING_CATEGORY(LOG_ROMTHUMBNAILER, "romthumbnailer")


#include <KConfigCore/KConfig>

#include "config/configui.h"
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
            char game_code[4];
            NDS::get_rom_code(in, game_code);
            qCDebug(LOG_ROMTHUMBNAILER) << "ROM Code: " << game_code << '\n';
            auto ds_covers = QDir::home();
            ds_covers.cd(".cache/RomThumbnailer/covers/ds/coverS/US/");
            ds_covers.setFilter(QDir::Files);
            ds_covers.setSorting(QDir::SortFlags::enum_type::Name);
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
    return new ConfigUi();
}


void RomThumbnailer::writeConfiguration(const QWidget *configurationWidget) {
    
}