#include <string.h>
#include <gb/gb.h>
#include "controller.h"
#include "../core/core.h"
#include "../assets/tiles.h"

#define CNT_START 2

static unsigned char background[360];
static unsigned char field[64];

static void render_board(unsigned char reveal) {
    static unsigned char i, *iter_board, *iter_field, val;
    for (i = 0, iter_board = board, iter_field = field;
         i < 64;
         ++i, ++iter_board, ++iter_field) {
        val = *iter_board;
        if (reveal) {
            *iter_field = val & MINE_FLAG ? 12 : CNT_START + (val & CNT_FLAG);
        } else {
            if (val & CHECK_FLAG) {
                *iter_field =
                    val & MINE_FLAG ? 12 : CNT_START + (val & CNT_FLAG);
            } else if (val & MARK_FLAG) {
                *iter_field = 11;
            } else {
                *iter_field = 1;
            }
        }
    }
}

void controller_initialize(void) {
    set_bkg_data(0, 13, tiles);

    VBK_REG = 1;
    VBK_REG = 0;

    memset(background, 0, 360);
    set_bkg_tiles(0, 0, 20, 18, background);

    core_initialize();

    render_board(1);
    set_bkg_tiles(0, 0, 8, 8, field);

    SHOW_BKG;
    DISPLAY_ON;
}
