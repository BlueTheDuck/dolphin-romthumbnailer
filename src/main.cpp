#include "romthumbnailer.h"

extern "C" {
Q_DECL_EXPORT ThumbCreator *new_creator() { return new RomThumbnailer(); }
}