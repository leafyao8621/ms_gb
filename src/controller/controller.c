#include <string.h>
#include <gb/gb.h>
#include "../assets/tiles.h"

static unsigned char background[360];
static unsigned char field[64];

void controller_initialize(void) {
    set_bkg_data(0, 11, tiles);

    VBK_REG = 1;
    VBK_REG = 0;

    memset(background, 0, 360);
    set_bkg_tiles(0, 0, 20, 18, background);

    memset(field, 1, 64);
    set_bkg_tiles(0, 0, 8, 8, field);

    SHOW_BKG;
    DISPLAY_ON;
}
