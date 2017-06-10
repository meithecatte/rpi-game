CFLAGS := -I. -std=gnu11 `sdl2-config --cflags` -O2 -Wall -Wextra -Werror
LDFLAGS := `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lm

GAMES := ekans
HDRS := $(wildcard *.h)
MAIN := $(wildcard *.c)
SRCS := $(MAIN) $(wildcard $(patsubst %,%/*.c,$(GAMES)))
OBJS := $(patsubst %.c,build/%.o,$(SRCS))

all: rpi-game | dirs
	@[ `git grep '[^_]\(malloc\|free\)' | grep -v 'TEST_EXCEPTION' | wc -l` -eq 0 ]
	@echo Checking heap wrapper usage...
	@git grep '[^_]\(malloc\|free\)' | grep -v 'TEST_EXCEPTION' || true

clean:
	@rm -rf build rpi-game

dirs:
	@mkdir -p build $(patsubst %,build/%,$(GAMES))

rpi-game: $(OBJS) | dirs
	@echo " LD rpi-game"
	@gcc $(LDFLAGS) $(OBJS) -o $@
	@strip $@

build/%.o: %.c $(HDRS) $(subst ./..h,,$(dir $<)$(patsubst %/,%.h,$(dir $<))) Makefile | dirs
	@echo " CC $<"
	@gcc $(CFLAGS) -c $< -o $@

.PHONY: all clean dirs
