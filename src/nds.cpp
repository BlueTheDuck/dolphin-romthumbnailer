#include "nds.h"

QVector<QRgb> decode_rgb555_palette(const QByteArrayView &data) {
  QVector<QRgb> palette;
  palette.reserve(data.size() / 2);
  for (size_t i = 0; i < 32; i += 2) {
    const auto rgb555 = qFromLittleEndian<uint16_t>(data.mid(i, 2).data());
    const uint32_t r = ((rgb555 >> 0) & 0x1F) * 255 / 31;
    const uint32_t g = ((rgb555 >> 5) & 0x1F) * 255 / 31;
    const uint32_t b = ((rgb555 >> 10) & 0x1F) * 255 / 31;
    palette.append(qRgb(r, g, b));
  }
  return palette;
}

void decode_tile(QImage &image, const QPoint &base, const QByteArrayView &data/* ,
                 const QVector<uint32_t> &palette */) {

  for (size_t i = 0; i < 32; i++) {
    QPoint coords = base + QPoint((i % 4) * 2, i / 4);
    uint8_t pixel_data = data.at(i);
    uint8_t right_pixel = (pixel_data >> 4) & 0x0F;
    uint8_t left_pixel = (pixel_data & 0x0F);
    image.setPixel(coords, left_pixel);
    image.setPixel(coords + QPoint(1, 0), right_pixel);
  }
}
/// @brief  Renders a 32x32 icon from a 4bpp bitmap
/// @param data
/// @param palette
/// @return
QImage render_icon(const QByteArrayView &data, const QVector<QRgb> &palette) {
  QImage icon(QSize(32, 32), QImage::Format::Format_Indexed8);
  icon.setColorTable(palette);
  icon.fill(QColorConstants::Transparent);

  for (size_t i = 0; i < 16; i++) {
    QPoint tile_base = QPoint((i % 4) * 8, (i / 4) * 8);
    decode_tile(icon, tile_base, data.mid(i * 32, 32) /* , palette */);
  }

  return icon;
}

NdsRom::NdsHeader::NdsHeader(QFile &file) {
  assert(file.open(QIODevice::ReadOnly));
  assert(file.seek(0));
  this->_data = file.read(0x4000);
}
std::optional<uint32_t> NdsRom::NdsHeader::banner_offset() const {
  uint32_t offset =
      qFromLittleEndian<uint32_t>(this->_data.mid(0x0068, 4).data());
  if (offset == 0) {
    return {};
  } else {
    return offset;
  }
}
uint32_t NdsRom::NdsHeader::banner_size() const {
  const auto possible_size =
      qFromLittleEndian<uint32_t>(this->_data.mid(0x0208, 4).data());
  if (possible_size == 0) {
    return 0x840;
  } else {
    return possible_size;
  }
}

NdsRom::NdsBanner::NdsBanner(QByteArray data) : _data(std::move(data)) {}
QImage NdsRom::NdsBanner::icon() const {
  const auto &raw_palette = this->_data.mid(0x0220, 32);
  QVector<QRgb> palette = decode_rgb555_palette(raw_palette);
  palette[0] &= !qAlpha(0); // Make the first color transparent
  const auto &raw_tiles = this->_data.mid(0x0020, 0x200);
  return render_icon(raw_tiles, palette);
}
uint16_t NdsRom::NdsBanner::version() const {
  return qFromLittleEndian<uint16_t>(this->_data.mid(0x01FC, 2).data());
}
bool NdsRom::NdsBanner::animated() const {
  // TODO: Implement this
  // do we have animation data?
  bool has_animation_data = this->_data.size() >= 0x23C0;
  bool is_correct_version = this->version() >= 0x0103;
  bool first_frame_not_empty =
      has_animation_data ? this->_data[0x2340] != 0 : false;
  return has_animation_data && is_correct_version && first_frame_not_empty;
}

uint32_t NdsRom::NdsBanner::frame_count() const {
  if (!this->animated()) {
    return 1;
  }

  uint32_t frame_count = 0;
  for (size_t i = 0; i < animation_tokens_count(); i++) {
    AnimationToken token = this->animation_token(i);
    frame_count += token.length;
  }
  return frame_count;
};
QImage NdsRom::NdsBanner::frame(uint32_t frame_i) const {
  if (!this->animated() || frame_i >= this->frame_count()) {
    return {};
  }

  AnimationToken token;
  for (size_t token_i = 0; token_i < animation_tokens_count(); token_i++) {
    token = this->animation_token(token_i);
    if (token.length > frame_i) {
      break;
    } else {
      frame_i -= token.length;
    }
  }
  const auto &raw_palette =
      this->_data.mid(0x2240 + token.palette_index * 32, 32);
  const auto &bitmap_data =
      this->_data.mid(0x1240 + token.image_index * 0x200, 0x200);
  QVector<QRgb> palette = decode_rgb555_palette(raw_palette);
  QImage frame(render_icon(bitmap_data, palette));
  frame = frame.mirrored(token.hflip, token.vflip);
  return frame;
};
uint32_t NdsRom::NdsBanner::animation_tokens_count() const {
  for (size_t i = 0; i < 64; i++) {
    AnimationToken token = this->animation_token(i);
    if (token.is_end()) {
      return i;
    }
  }
  return 64;
};
NdsRom::NdsBanner::AnimationToken
NdsRom::NdsBanner::animation_token(uint32_t i) const {
  QByteArrayView animation_tokens_view = this->_data.mid(0x2340, 0x80);
  QByteArrayView animation_token = animation_tokens_view.mid(
      i * sizeof(AnimationToken), sizeof(AnimationToken));
  return AnimationToken(qFromLittleEndian<uint16_t>(animation_token.data()));
};

NdsRom::NdsRom(QFile file) : _header(file) {
  std::optional<uint32_t> banner_offset = _header.banner_offset();
  if (banner_offset.has_value()) {
    file.seek(*banner_offset);
    this->_banner = NdsBanner(file.read(_header.banner_size()));
  } else {
    this->_banner = {};
  }
}

QImage NdsRom::icon() const {
  if (this->_banner.has_value()) {
    return this->_banner->icon().convertedTo(QImage::Format::Format_ARGB32);
  } else {
    return {};
  }
}
