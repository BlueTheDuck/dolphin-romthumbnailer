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

NDS::NDS::NDS(std::unique_ptr<QFile> file) { this->_file = std::move(file); }
QString NDS::NDS::get_rom_code() const {
    qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Reading ROM code";
    assert(this->_file->seek(GAME_CODE_ADDR));
    QByteArray file_data = this->_file->read(4);
    QString    code(file_data);
    return code;
}

uint32_t NDS::NDS::get_banner_offset() const {
    qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Calculating banner offset";
    assert(this->_file->seek(BANNER_OFFSET_ADDR));
    auto data   = this->_file->read(4);
    auto offset = 0;
    for (int i = 0; i < 4; i++) {
        offset <<= 8;
        offset |= data.at(i);
    }
    return offset;
}

void NDS::NDS::get_icon(QImage &img) const {
    qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Generating icon";
    int colors[16] = {
        0x000000, 0x00007F, 0x007F00, 0x007F7F, 0x7F0000, 0x7F007F,
        0x7F7F00, 0x7F7F7F, 0x000000, 0x0000FF, 0x00FF00, 0x00FFFF,
        0xFF0000, 0xFF00FF, 0xFFFF00, 0xFFFFFF,
    };
    qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Reading image";
    assert(this->_file->seek(ICON_BITMAP_ADDR + this->get_banner_offset()));
    auto data = this->_file->read(ICON_BITMAP_SIZE);
    assert(data.size() % 4 * 8 == 0);
    for (size_t base = 0, i = 0; base < ICON_BITMAP_SIZE; base += 4 * 8, i++) {
        auto point_data = int(data.at(base));
        auto base_x = (i % 4) * 8;
        auto base_y = (i - (i % 4)) / 4 * 8;
        for(size_t x = 0; x < 8; x++)
            for(size_t y = 0; y < 8; y++)
                img.setPixel(base_x +x,base_y+y,colors[point_data]);
    }
    qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Done generating icon";

}

/*
size_t NDS::get_banner_offset(QFile &file) {
    file.seek(NDS::BANNER_OFFSET_ADDR);
    qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Pos: " << file.pos();
    uint32_t offset = 0;

    auto     data   = file.read(1);
    for (QByteArray::iterator i = data.begin(); i != data.end(); ++i) {
        // 00 00 0F 84
        qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Byte: " <<
as_hex(static_cast<uint64_t>(*i)&0xFF).c_str(); offset = (offset << 8) + *i;
    }
std::stringstream ss;
ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << offset;
qCDebug(LOG_ROMTHUMBNAILER_NDS) << "Offset: " << as_hex(offset).c_str();
return offset;
}

void NDS::get_icon(QFile &in, QImage &image) {
    uint8_t pixmap[32][32] = {0};
    auto    offset         = NDS::get_banner_offset(in);
}
*/