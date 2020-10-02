
#ifndef ROMTHUMBNAILER_H
#define ROMTHUMBNAILER_H

#include <QPixmap>
#include <kio/thumbcreator.h>

class RomThumbnailer : public ThumbCreator {
  public:
    /**
     * Default constructor
     */
    RomThumbnailer();

    /**
     * Destructor
     */
    ~RomThumbnailer();

    bool create(const QString &path, int w, int h, QImage &img) override;

    QWidget* createConfigurationWidget() override;
};

#endif // ROMTHUMBNAILER_H
