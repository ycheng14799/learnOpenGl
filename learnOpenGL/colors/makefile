all: color_learn.cpp color.cpp shader_m.h ../glad.c stb_image.h stb_image.cpp camera.h
	g++ -o colors color.cpp shader_m.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp camera.h -std=gnu++0x
	g++ -o colorsLearn color_learn.cpp shader_m.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp camera.h -std=gnu++0x
clean:
	$(RM) colors
