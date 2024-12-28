#pragma once

#include "nds.h"
#include "romdata.h"
#include "debug.h"
#include <QDebug>
#include <QFile>
#include <KPluginFactory>
#include <QImage>
#include <KIO/ThumbnailCreator>

class NdsRomThumbnail : public KIO::ThumbnailCreator
{
    Q_OBJECT

private:
public:
    NdsRomThumbnail (QObject *parent, const QVariantList &args);
    ~NdsRomThumbnail ();
    KIO::ThumbnailResult create(const KIO::ThumbnailRequest &request) override;
};

