#ifndef DS_H
#define DS_H

#include <QFile>
#include <QImage>
#include <iomanip>
#include <memory>
#include <sstream>
#include <stdint.h>

class NDS {
    const int      GAME_CODE_ADDR     = 0x000C;
    const int      GAME_CODE_SIZE     = 4; // bytes
    const uint16_t BANNER_OFFSET_ADDR = 0x0068;
    // Relative to this::get_banner_offset()
    const uint32_t ICON_BITMAP_ADDR = 0x0020;
    const uint32_t ICON_BITMAP_SIZE = 0x0200;

  private:
    std::unique_ptr<QFile> _file;

  public:
    NDS(std::unique_ptr<QFile> file);
    uint32_t get_banner_offset() const;
    QString  get_rom_code() const;
    void     get_icon(QImage &) const;
};

#endif