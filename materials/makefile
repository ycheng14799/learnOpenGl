all: materials.cpp shader_m.h ../glad.c stb_image.h stb_image.cpp camera.h
	g++ -o materials materials.cpp shader_m.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp camera.h -std=gnu++0x
clean:
	$(RM) basicLighting
