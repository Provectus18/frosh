frosh-game: main.c
	cc main.c -o frosh-game -I./include -L./lib -lraylib -lm -lpthread -ldl -lGL -lX11 -lrt

clean:
	rm frosh-game

run:
	cc main.c -o frosh-game -I./include -L./lib -lraylib -lm -lpthread -ldl -lGL -lX11 -lrt && LD_LIBRARY_PATH=./lib LIBGL_ALWAYS_SOFTWARE=1 ./frosh-game
