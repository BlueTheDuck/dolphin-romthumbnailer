#include "nds.h"

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(LOG_ROMTHUMBNAILER_NDS)
Q_LOGGING_CATEGORY(LOG_ROMTHUMBNAILER_NDS, "romthumbnailer.nds")

template <typename T> std::string as_hex(T t) {
    std::stringstream ss;
    ss << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex
       << t;
    return ss.str();
}

NDS::NDS(std::unique_ptr<QFile> file) { this->_file = std::move(file); }

QString NDS::get_rom_code() const {
    qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Reading ROM code";
    assert(this->_file->seek(GAME_CODE_ADDR));
    QByteArray file_data = this->_file->read(4);
    QString    code(file_data);
    return code;
}

uint32_t NDS::get_banner_offset() const {
    qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Calculating banner offset";
    assert(this->_file->seek(BANNER_OFFSET_ADDR));
    auto data   = this->_file->read(4);
    auto offset = 0;
    for (int i = 3; i >= 0; i--) {
        uint8_t byte = data.at(i) & 0xFF;
        offset       = (offset << 8) | byte;
    }
    return offset;
}

void NDS::get_icon(QImage &img) const {
    // Shorthand for colors
    auto palette = this->get_icon_palette();
    // Width of a tile (in bytes)
    const uint64_t tile_width = 4;
    // Height of a tile (in bytes)
    const uint64_t tile_height = 8;
    // How many bytes is one tile
    const uint64_t tile_bytes = 32;

    img = QImage(QSize(32, 32), QImage::Format::Format_ARGB32);
    img.fill(QColorConstants::White);

    qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Generating icon";
    assert(this->_file->seek(ICON_BITMAP_ADDR + this->get_banner_offset()));
    auto data = this->_file->read(ICON_BITMAP_SIZE);
    assert(data.size() == ICON_BITMAP_SIZE);

    // Images are 4x4 tiles
    for (size_t tile_y = 0; tile_y < 4; tile_y++) {
        for (size_t tile_x = 0; tile_x < 4; tile_x++) {
            // Each tile is 8x8 pixels, but in each byte there are 2 pixels, so
            // the tile is 8x4 bytes
            for (size_t local_y = 0; local_y < tile_height; local_y++) {
                for (size_t local_x = 0; local_x < tile_width; local_x++) {
                    // Convert current position to index in the data stream
                    size_t idx = tile_y * tile_bytes * 4 + tile_x * tile_bytes +
                        local_y * tile_width + local_x;
                    size_t  x           = tile_x * 8 + local_x * 2;
                    size_t  y           = tile_y * 8 + local_y;
                    uint8_t pixel_data  = data.at(idx);
                    uint8_t right_pixel = (pixel_data >> 4) & 0x0F;
                    uint8_t left_pixel  = (pixel_data & 0x0F);

                    // If color idx is 0, then we substitute it with transparent (0x000000)
                    uint32_t left_pix_color =
                        left_pixel == 0 ? 0x00000000 : palette.at(left_pixel);
                    uint32_t right_pix_color =
                        right_pixel == 0 ? 0x00000000 : palette.at(right_pixel);

                    img.setPixel(x, y, left_pix_color);
                    img.setPixel(x + 1, y, right_pix_color);
                }
            }
        }
    }
    qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Done generating icon";
}

QVector<uint32_t> NDS::get_icon_palette() const {
    QVector<uint32_t> palette;
    palette.reserve(16);

    qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Reading icon palette";
    // Palette after bitmap
    assert(this->_file->seek(this->get_banner_offset() + ICON_BITMAP_ADDR +
                             ICON_BITMAP_SIZE));
    auto data = this->_file->read(0x20);
    for (size_t i = 0; i < 32; i += 2) {
        uint8_t hi = data.at(i + 1);
        uint8_t lo = data.at(i);
        // This is a 555 BGR color. No idea why, but NO$GBA calls it "setting"
        uint16_t setting = lo | uint16_t(hi) << 8;
        uint32_t red     = (setting & 0b0000'0000'0001'1111) << 3;
        uint32_t green   = (setting & 0b0000'0011'1110'0000) >> 5 << 3;
        uint32_t blue    = (setting & 0b0111'1100'0000'0000) >> 10 << 3;
        uint32_t color   = 0xFF000000 | ((red & 0xFF) << 16) |
            ((green & 0xFF) << 8) | (blue & 0xFF);
        palette.push_back(color);
    }

    return palette;
}
