all: multLights.cpp shader_m.h ../glad.c stb_image.h stb_image.cpp camera.h
	g++ -o multLights multLights.cpp shader_m.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp camera.h -std=gnu++0x
clean:
	$(RM) lightCasters
