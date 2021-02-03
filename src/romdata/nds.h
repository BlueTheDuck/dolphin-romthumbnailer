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
    /// The NDS boot menu shows a banner, an icon over the game's title. This
    /// function gets the icon and outputs an RGB32 image
    /// @param Image to overwrite with the ROM icon. Must be uninitialized
    void get_icon(QImage &) const;
    /// Reads the palette used by the icon and converts it into RGB32
    /// @return 16 RGB32 colors
    QVector<uint32_t> get_icon_palette() const;
};

#endif