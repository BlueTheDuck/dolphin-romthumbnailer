#include "../src/nds.h"
#include "gtest/gtest.h"
#include <iostream>
#include <QString>

const char rom_path[] = "/home/pedro/Projects/kde/RomThumbnailer/Phoenix "
                        "Wright - Ace Attorney (USA).nds";

TEST(Tests, Open) {

    std::unique_ptr<QFile> file(new QFile(rom_path));
    file->open(QIODevice::OpenModeFlag::ReadOnly);
    NDS   rom(std::move(file));
    std::cout << rom.get_rom_code().toStdString() << std::endl;
}
