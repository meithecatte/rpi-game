#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "global.h"

#define PERI_BASE 0x20000000
#define GPIO_BASE (PERI_BASE + 0x200000)
#define INP_GPIO(g) *(gGPIO+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) INP_GPIO(g); *(gGPIO+((g)/10)) |=  (1<<(((g)%10)*3))
#define GET_GPIO(g) (*(gGPIO+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH
#define GPIO_LO(g) *(gGPIO + 10 + (g / 32)) = 1 << (g % 32)
#define GPIO_HI(g) *(gGPIO + 7 + (g / 32)) = 1 << (g % 32)

#define GPIO_JOY_CLOCK 2
#define GPIO_JOY_LATCH 3
#define GPIO_JOY_DATA 4

volatile u32 * gGPIO;
u16 gJoypadHeld, gJoypadPressed;

void Init_Joypad(void){
	int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);

	ERROR_ON_SYS(mem_fd < 0, "GPIO open");

	gGPIO = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, GPIO_BASE);

	ERROR_ON_SYS(gGPIO == MAP_FAILED, "GPIO mmap");

	close(mem_fd);

	OUT_GPIO(GPIO_JOY_CLOCK);
	OUT_GPIO(GPIO_JOY_LATCH);
	INP_GPIO(GPIO_JOY_DATA);

	GPIO_LO(GPIO_JOY_LATCH);
	GPIO_HI(GPIO_JOY_CLOCK);
}

void Read_Joypad(void){
	u16 oldJoypad = gJoypadHeld;
	gJoypadHeld = 0;

	GPIO_HI(GPIO_JOY_LATCH); usleep(15);
	GPIO_LO(GPIO_JOY_LATCH); usleep(15);

	for(u8 i = 0;i < 12;i++){
		gJoypadHeld <<= 1;

		if(!GET_GPIO(GPIO_JOY_DATA)){
			gJoypadHeld |= 1;
		}

		GPIO_LO(GPIO_JOY_CLOCK); usleep(15);
		GPIO_HI(GPIO_JOY_CLOCK); usleep(15);
	}

	gJoypadPressed = gJoypadHeld & ~oldJoypad;

	if(gJoypadHeld == (JOY_L | JOY_R | JOY_SELECT | JOY_START)) gExit = 1;
}
