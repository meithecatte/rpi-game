CFLAGS = -O2 -Wall -Wextra -Werror

OBJS := main
OBJS := $(OBJS:%=build/%.o)

all: rpi-game | dirs

clean:
	@rm -rf build rpi-game

dirs:
	@mkdir -p build

rpi-game: $(OBJS) | dirs
	@gcc -o $@ $< `sdl2-config --libs`
	@strip $@

build/%.o: %.c | dirs
	@gcc $(CFLAGS) `sdl2-config --cflags` -c $< -o $@

.PHONY: all clean dirs
