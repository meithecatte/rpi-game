CFLAGS := -std=gnu11 `sdl2-config --cflags` -O2 -Wall -Wextra -Werror
LDFLAGS := `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lm

HDRS := $(wildcard *.h)
OBJS := $(patsubst %.c,build/%.o,$(wildcard *.c))

all: rpi-game | dirs

clean:
	@rm -rf build rpi-game

dirs:
	@mkdir -p build

rpi-game: $(OBJS) | dirs
	@gcc $(LDFLAGS) $(OBJS) -o $@
	@strip $@

build/%.o: %.c $(HDRS) | dirs
	@gcc $(CFLAGS) -c $< -o $@

.PHONY: all clean dirs
