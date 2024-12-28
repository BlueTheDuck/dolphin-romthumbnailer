#pragma once

#include "debug.h"
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QString>
#include <QVector>
#include <QtEndian>
#include <optional>

class NdsRom {
  class NdsHeader {
  private:
    QByteArray _data;

  public:
    NdsHeader() = delete;
    NdsHeader(QFile &);

    std::optional<uint32_t> banner_offset() const;
    uint32_t banner_size() const;
  };
  class NdsBanner {
  private:
    class AnimationToken {
    public:
      uint8_t vflip : 1;
      uint8_t hflip : 1;
      uint8_t palette_index : 3;
      uint8_t image_index : 3;
      uint8_t length;

      AnimationToken() = default;
      AnimationToken(uint16_t value) {
        this->vflip = (value & 0x8000) >> 15;
        this->hflip = (value & 0x4000) >> 14;
        this->palette_index = (value & 0x3800) >> 11;
        this->image_index = (value & 0x0700) >> 8;
        this->length = value & 0x00FF;
      }
      // TODO: verify this
      bool is_end() const { return length == 0; }
    };
    QByteArray _data;

  public:
    NdsBanner() = delete;
    NdsBanner(QByteArray);

    QImage icon() const;
    uint16_t version() const;
    bool animated() const;
    /// Returns the number of frames in the animation, or 1 if the banner is not
    /// animated
    uint32_t frame_count() const;
    /// Renders the frame at the given index. nullptr if the banner is not
    /// animated or the index is out of bounds
    QImage frame(uint32_t) const;

  private:
    /// Returns the number of animation tokens, or 0 if the banner is not
    /// animated
    uint32_t animation_tokens_count() const;
    AnimationToken animation_token(uint32_t) const;
  };

private:
  NdsHeader _header;
  std::optional<NdsBanner> _banner;

public:
  NdsRom(QFile file);
  const std::optional<NdsBanner> &banner() const { return _banner; }

  /// Renders the icon of the NDS ROM if it has one
  QImage icon() const;
};
