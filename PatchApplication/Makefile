all:
	gcc $(shell pkg-config --cflags gtk4 gstreamer-1.0) -o Main Main.c $(shell pkg-config --libs gtk4 gstreamer-1.0)

run:
	make
	./Main
