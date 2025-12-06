source = src/main.c src/player.c src/scene.c
libs = -lraylib -lm
flags = -ggdb -fsanitize=address

ray_caster: $(source) build
	gcc $(source) $(libs) -o build/ray_caster

build: 
	mkdir build

main.c: player.h window.h scene.h

player.c: player.h window.h scene.h 

scene.c: scene.h 

.PHONY: clean run

clean:
	rm -r build

run: ray_caster
	./build/ray_caster
