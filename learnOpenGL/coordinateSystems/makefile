all: coords.cpp shader_s.h ../glad.c stb_image.h stb_image.cpp
	g++ -o coords coords.cpp shader_s.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp
clean: 
	$(RM) coords
