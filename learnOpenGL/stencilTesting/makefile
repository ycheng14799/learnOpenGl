all: basicScene.cpp shader_m.h ../glad.c stb_image.h stb_image.cpp camera.h
	g++ -o basicScene basicScene.cpp shader_m.h ../glad.c stb_image.h stb_image.cpp camera.h -lglfw -ldl -std=gnu++0x 

clean: 
	$(RM) basicScene
