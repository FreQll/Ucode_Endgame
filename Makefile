flags := -std=c11 -Wall -Wextra -Werror -Wpedantic 

name := endgame

all: MINILIBMX

MINILIBMX:
	clang $(flags) src/*.c -o $(name) -lSDL2 -lSDL2_mixer -lSDL2main -lSDL2_ttf -lSDL2_image -Wno-switch

clean:
	rm -f $(name)

uninstall:
	rm -f $(name)

reinstall:
	make uninstall
	make
