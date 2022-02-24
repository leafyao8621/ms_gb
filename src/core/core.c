#include <string.h>
#include <gb/gb.h>
#include <rand.h>
#include "core.h"

unsigned char board[64], mines, checked;

void core_initialize(void) {
    static unsigned char idx, *iter;
    static uint16_t seed;
    static char i, j, k, l;
    seed = LY_REG;
    seed |= (uint16_t)DIV_REG << 8;
    initrand(seed);
    checked = 0;
    mines = 0;
    memset(board, 0, 64);
    for (; mines < 10; ++mines) {
        idx = rand() & 0x3f;
        for (; board[idx] & MINE_FLAG; idx = rand() & 0x3f);
        board[idx] |= MINE_FLAG;
    }
    for (i = 0, iter = board; i < 8; ++i) {
        for (j = 0; j < 8; ++j, ++iter) {
            if (!(*iter & MINE_FLAG)) {
                for (k = -1; k < 2; ++k) {
                    for (l = -1; l < 2; ++l) {
                        if ((k || l) &&
                            i + k >= 0 &&
                            i + k < 8 &&
                            j + l >= 0 &&
                            j + l < 8) {
                            *iter +=
                                (
                                    (
                                        board[((i + k) << 3) + j + l] &
                                        MINE_FLAG
                                    ) !=
                                    0
                                );
                        }
                    }
                }
            }
        }
    }
}

void core_mark(unsigned char idx) {
    board[idx] ^= MARK_FLAG;
}

void core_check(unsigned char idx) {
    static unsigned char buf[64], *buf_ptr;
    if (board[idx] & (MARK_FLAG | CHECK_FLAG)) {
        return;
    }
    buf_ptr = buf;
}
