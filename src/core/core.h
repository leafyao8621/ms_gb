#ifndef _CORE_H_
#define _CORE_H_

#define MINE_FLAG 0x80
#define CHECK_FLAG 0x40
#define MARK_FLAG 0x20

#define CNT_FLAG 0xf

extern unsigned char board[64], mines, checked, death;

void core_initialize(void);
void core_reset(void);
void core_mark(unsigned char idx);
void core_check(unsigned char idx);

#endif
