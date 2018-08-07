all: flashLight.cpp pointLights.cpp directionalLight.cpp  shader_m.h ../glad.c stb_image.h stb_image.cpp camera.h
	g++ -o directionalLight directionalLight.cpp shader_m.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp camera.h -std=gnu++0x
	g++ -o pointLights pointLights.cpp shader_m.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp camera.h -std=gnu++0x
	g++ -o flashlight flashLight.cpp shader_m.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp camera.h -std=gnu++0x
clean:
	$(RM) lightCasters
