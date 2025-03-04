all:
	mkdir -p build
	gcc -I inc $(shell pkg-config --cflags gtk4 gstreamer-1.0) -o build/Main src/Main.c $(shell pkg-config --libs gtk4 gstreamer-1.0)

run:
	make
	./build/Main

clean:
	rm -r build
