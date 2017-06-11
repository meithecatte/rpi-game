#ifndef _JOYPAD_H
#define _JOYPAD_H
#define JOY_SELECT 0x200
#define JOY_START 0x100
#define JOY_UP 0x080
#define JOY_DOWN 0x040
#define JOY_LEFT 0x020
#define JOY_RIGHT 0x010
#define JOY_A 0x008
#define JOY_B 0x800
#define JOY_X 0x004
#define JOY_Y 0x400
#define JOY_L 0x002
#define JOY_R 0x001

extern uint16_t gJoypadHeld, gJoypadPressed;

void Init_Joypad(void);
void Read_Joypad(void);
#endif
