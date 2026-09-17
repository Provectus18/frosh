# Frosh
Frosh is a video game that I plan to design that will be very to "SpeedRunners", and "Super Mario Bros Run". It is the first graphical video game that I ever designed in C.

### How to build and run

To build it.
`gcc main.c -o game -I./include -L./lib -lraylib -lm -lpthread -ldl -lGL -lX11 -lrt`

To run it.
`LD_LIBRARY_PATH=./lib LIBGL_ALWAYS_SOFTWARE=1 ./game`
