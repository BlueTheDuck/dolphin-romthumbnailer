#ifndef DS_H
#define DS_H

#include <QFile>

namespace NDS {
const int GAME_CODE_ADDR = 0x000C;
const int GAME_CODE_SIZE = 4; // bytes
void    get_rom_code(QFile &in, char code[4]);
}; // namespace NDS

#endif