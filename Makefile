
default:
	gcc -o game util.c scene.c object.c objectlist.c camera.c input.c hud.c texture.c tga.c main.c -lglut -lGL -lGLU
clean:
	rm game
