all: textures.cpp shader_s.h ../glad.c stb_image.h stb_image.cpp
	g++ -o textures textures.cpp shader_s.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp
clean: 
	$(RM) textures
