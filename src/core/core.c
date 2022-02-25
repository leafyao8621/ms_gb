#include <string.h>
#include <gb/gb.h>
#include <rand.h>
#include "core.h"

unsigned char board[64], mines, checked, death;

void core_initialize(void) {
    static uint16_t seed;
    seed = LY_REG;
    seed |= (uint16_t)DIV_REG << 8;
    initrand(seed);
}

void core_reset(void) {
    static unsigned char idx, *iter;
    static char i, j, k, l;
    death = 0;
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
    static char i, j, val, r, c, cur;
    if (board[idx] & (MARK_FLAG | CHECK_FLAG)) {
        return;
    }
    if (board[idx] & MINE_FLAG) {
        death = 1;
        return;
    }
    buf_ptr = buf;
    board[idx] |= CHECK_FLAG;
    *(buf_ptr++) = idx;
    for (; buf_ptr != buf;) {
        --buf_ptr;
        val = *buf_ptr;
        if (!(board[val] & CNT_FLAG)) {
            r = val >> 3;
            c = val & 0x7;
            for (i = -1; i < 2; ++i) {
                for (j = -1; j < 2; ++j) {
                    cur = ((i + r) << 3) + j + c;
                    if ((i || j) &&
                        i + r >= 0 &&
                        i + r < 8 &&
                        j + c >= 0 &&
                        j + c < 8 &&
                        !(board[cur] & CHECK_FLAG)) {
                        board[cur] |= CHECK_FLAG;
                        *(buf_ptr++) = cur;
                    }
                }
            }
        }
    }
}
