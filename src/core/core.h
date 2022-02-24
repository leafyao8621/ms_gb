#ifndef _CORE_H_
#define _CORE_H_

#define MINE_FLAG 0x80
#define CHECK_FLAG 0x40
#define MARK_FLAG 0x20

#define CNT_FLAG 0xf

extern unsigned char board[64];

void core_initialize(void);
void core_mark(unsigned char idx);

#endif
