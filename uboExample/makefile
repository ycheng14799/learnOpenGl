all: uboExample.cpp shader_m.h ../glad.c stb_image.h stb_image.cpp camera.h
	g++ -o uboExample uboExample.cpp shader_m.h ../glad.c -lglfw -ldl -lassimp stb_image.h stb_image.cpp camera.h -std=gnu++17
clean:	
	$(RM) modelLoading 
