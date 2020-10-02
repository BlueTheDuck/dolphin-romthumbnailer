#include "nds.h"

void NDS::get_rom_code(QFile &in, char code[4]) {
    in.seek(NDS::GAME_CODE_ADDR);
    auto game_code = in.read(NDS::GAME_CODE_SIZE);
    for (size_t i = 0; i < NDS::GAME_CODE_SIZE; i++) {
        code[i] = game_code.at(i);
    }
}