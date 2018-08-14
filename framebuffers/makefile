all: framebuffers.cpp shader_m.h ../glad.c stb_image.h stb_image.cpp camera.h
	g++ -o framebuffers framebuffers.cpp shader_m.h ../glad.c stb_image.h stb_image.cpp camera.h -lglfw -ldl -std=gnu++0x 
clean: 
	$(RM) basicScene
