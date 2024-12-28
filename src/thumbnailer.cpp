#include "thumbnailer.h"

K_PLUGIN_CLASS_WITH_JSON(NdsRomThumbnail, "thumbnailer.json")

NdsRomThumbnail::NdsRomThumbnail(QObject *parent, const QVariantList &args)
    : KIO::ThumbnailCreator(parent, args) {}

NdsRomThumbnail::~NdsRomThumbnail() = default;

KIO::ThumbnailResult
NdsRomThumbnail::create(const KIO::ThumbnailRequest &request) {
  const auto &size = request.targetSize();
  NdsRom nds(QFile(request.url().toLocalFile()));

  if (!nds.banner().has_value()) {
    qCDebug(LOG_NDSROMTHUMBNAILER) << "Returning fail. No banner found.";
    return KIO::ThumbnailResult::fail();
  } else {
    const auto &banner = nds.banner().value();
    const bool asked_for_animation = request.sequenceIndex() > 0;
    const bool banner_is_animated = banner.animated();

    if (banner_is_animated) {
      const uint32_t req_frame = uint32_t(request.sequenceIndex() * 60.0f);
      float animation_duration = float(banner.frame_count()) / 60.0f;
      QImage img = banner.frame(req_frame).scaled(
          size, Qt::KeepAspectRatioByExpanding,
          Qt::TransformationMode::FastTransformation);
      auto res = img.isNull() ? KIO::ThumbnailResult::fail()
                              : KIO::ThumbnailResult::pass(img);
      res.setSequenceIndexWraparoundPoint(ceil(animation_duration));
      return res;
    } else {
      QImage img =
          nds.icon().scaled(size, Qt::KeepAspectRatioByExpanding,
                            Qt::TransformationMode::FastTransformation);
      return img.isNull() ? KIO::ThumbnailResult::fail()
                          : KIO::ThumbnailResult::pass(img);
    }

    // if (banner_is_animated) {
    //   const uint32_t req_frame = uint32_t(request.sequenceIndex() * 60.0f);
    //   float animation_duration = float(banner.frame_count()) / 60.0f;
    //
    //   // log
    //   qCDebug(LOG_NDSROMTHUMBNAILER)
    //       << "Requested frame " << req_frame << " of " <<
    //       banner.frame_count();
    //
    //   QImage img;
    //   img = banner.frame(req_frame);
    //   auto res = img.isNull() ? KIO::ThumbnailResult::fail()
    //                           : KIO::ThumbnailResult::pass(img);
    //   res.setSequenceIndexWraparoundPoint(ceil(animation_duration));
    //   return res;
    // } else if (/* asked_for_animation &&  */!banner_is_animated) {
    //   QImage img =
    //       nds.icon().scaled(size, Qt::KeepAspectRatioByExpanding,
    //                         Qt::TransformationMode::FastTransformation);
    //   auto res = img.isNull() ? KIO::ThumbnailResult::fail()
    //                           : KIO::ThumbnailResult::pass(img);
    //   res.setSequenceIndexWraparoundPoint(0);
    //   return res;
    // } else if (!asked_for_animation) {
    //   QImage img =
    //       nds.icon().scaled(size, Qt::KeepAspectRatioByExpanding,
    //                         Qt::TransformationMode::FastTransformation);
    //   return img.isNull() ? KIO::ThumbnailResult::fail()
    //                       : KIO::ThumbnailResult::pass(img);
    // } else {
    //   qCDebug(LOG_NDSROMTHUMBNAILER) << "Returning fail. Unknown error.";
    //   return KIO::ThumbnailResult::fail();
    // }
  }
}

#include "thumbnailer.moc"
