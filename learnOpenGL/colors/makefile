all: color.cpp shader_m.h ../glad.c stb_image.h stb_image.cpp
	g++ -o colors color.cpp shader_m.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp
clean:
	$(RM) colors
