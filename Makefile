
default:
	gcc -o game scene.c object.c objectlist.c camera.c input.c hud.c texture.c tga.c main.c -lglut -lGL -lGLU -lXext -lX11
clean:
	rm game
