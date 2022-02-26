#include <string.h>
#include <gb/gb.h>
#include "controller.h"
#include "../core/core.h"
#include "../assets/tiles.h"
#include "../assets/sprites.h"

#define CNT_START 2

static unsigned char background[360];
static unsigned char field[64];
static unsigned idx;

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
    set_bkg_tiles(6, 5, 8, 8, field);
}

static void reset_game(void) {
    idx = 0;

    core_initialize();
    core_reset();

    render_board(0);
    move_sprite(0, (7 + (idx & 0x7)) << 3, (7 + (idx >> 3)) << 3);
}

void controller_initialize(void) {
    set_bkg_data(0, 13, tiles);

    memset(background, 0, 360);
    set_bkg_tiles(0, 0, 20, 18, background);

    SPRITES_8x8;
    set_sprite_data(0, 1, sprites);
    set_sprite_tile(0, 0);

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    reset_game();
    wait_vbl_done();
}

void controller_main_loop(void) {
    static unsigned char keys, prev, finished;
    prev = 0;
    for (;;) {
        keys = joypad();
        if ((keys & J_LEFT) && !(prev & J_LEFT)) {
            idx = (idx & 0xf8) | (((idx & 0x7) - 1) & 0x7);
            move_sprite(0, (7 + (idx & 0x7)) << 3, (7 + (idx >> 3)) << 3);
        }
        if ((keys & J_RIGHT) && !(prev & J_RIGHT)) {
            idx = (idx & 0xf8) | (((idx & 0x7) + 1) & 0x7);
            move_sprite(0, (7 + (idx & 0x7)) << 3, (7 + (idx >> 3)) << 3);
        }
        if ((keys & J_UP) && !(prev & J_UP)) {
            idx = (idx & 0x7) | (((idx & 0x38) - 8) & 0x38);
            move_sprite(0, (7 + (idx & 0x7)) << 3, (7 + (idx >> 3)) << 3);
        }
        if ((keys & J_DOWN) && !(prev & J_DOWN)) {
            idx = (idx & 0x7) | (((idx & 0x38) + 8) & 0x38);
            move_sprite(0, (7 + (idx & 0x7)) << 3, (7 + (idx >> 3)) << 3);
        }
        if ((keys & J_A) && !(prev & J_A)) {
            finished = checked == 54 || death;
            if (!finished) {
                core_check(idx);
            }
            render_board(checked == 54 || death);
        }
        if ((keys & J_B) && !(prev & J_B)) {
            finished = checked == 54 || death;
            if (!finished) {
                core_mark(idx);
            }
            render_board(finished);
        }
        if ((keys & J_START) && !(prev & J_START)) {
            reset_game();
        }
        prev = keys;
        wait_vbl_done();
    }
}
