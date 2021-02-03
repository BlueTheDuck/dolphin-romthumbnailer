#include "nds.h"
#include "inttypes.h"
#include "gtest/gtest.h"
#include <QImage>
#include <QLoggingCategory>
#include <QRgb>
#include <QSize>
#include <QString>
#include <QVector>
#include <iostream>

Q_DECLARE_LOGGING_CATEGORY(LOG_TESTS_NDS)
Q_LOGGING_CATEGORY(LOG_TESTS_NDS, "tests.nds")

const char     rom_path[] = "../../Phoenix Wright - Ace Attorney (USA).nds";
const char     expexted_icon_path[]   = "expected_icon.png";
const char     expected_rom_code[]    = "AGYE";
const uint32_t expected_banner_offset = 0x000F8400;
const uint32_t expected_palette[16]   = {
    0xFF00f800, 0xFF000000, 0xFF101010, 0xFF202020, 0xFF303030, 0xFF484848,
    0xFF585858, 0xFF686868, 0xFF808080, 0xFF909090, 0xFFa0a0a0, 0xFFb0b0b0,
    0xFFc8c8c8, 0xFFd8d8d8, 0xFFe8e8e8, 0xFFf8f8f8};

TEST(NdsTests, RomCode) {

    std::unique_ptr<QFile> file(new QFile(rom_path));
    file->open(QIODevice::OpenModeFlag::ReadOnly);
    NDS  rom(std::move(file));
    auto code = rom.get_rom_code();
    qCDebug(LOG_TESTS_NDS) << QString("Rom code: %1").arg(code);
    ASSERT_EQ(code.compare(expected_rom_code), 0);
}

TEST(NdsTests, BannerOffset) {
    std::unique_ptr<QFile> file(new QFile(rom_path));
    file->open(QIODevice::OpenModeFlag::ReadOnly);
    NDS  rom(std::move(file));
    auto offset = rom.get_banner_offset();
    qCDebug(LOG_TESTS_NDS) << QString("Banner offset = %1").arg(offset, 8, 16);
    ASSERT_EQ(offset, expected_banner_offset);
}

TEST(NdsTests, Icon) {
    std::unique_ptr<QFile> file(new QFile(rom_path));
    file->open(QIODevice::OpenModeFlag::ReadOnly);
    NDS    rom(std::move(file));
    QImage icon;
    rom.get_icon(icon);
    // I don't have a way to test if the image is correct, so lets just save it here and check by eye. If it looks OK, the it's good enought
    icon.save("../../tests/icon.png");
}

TEST(NdsTests, IconPalette) {
    std::unique_ptr<QFile> file(new QFile(rom_path));
    file->open(QIODevice::OpenModeFlag::ReadOnly);
    NDS  rom(std::move(file));
    auto palette = rom.get_icon_palette();
    ASSERT_EQ(palette.length(), 16);
    bool are_both_equal = true;
    for (size_t i = 0; i < 16; i++) {
        qCDebug(LOG_TESTS_NDS) << QString("%1/16: %2 // %3")
                                      .arg(i + 1)
                                      .arg(palette.at(i), 8, 16)
                                      .arg(expected_palette[i], 8, 16);
        are_both_equal =
            are_both_equal && (palette.at(i) == expected_palette[i]);
    }
    ASSERT_TRUE(are_both_equal);
}