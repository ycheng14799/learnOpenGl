all: modelLoading.cpp shader_m.h ../glad.c stb_image.h stb_image.cpp camera.h model.h
	g++ -o modelLoading modelLoading.cpp shader_m.h ../glad.c -lglfw -ldl -lassimp stb_image.h stb_image.cpp camera.h model.h -std=gnu++17
clean:	
	$(RM) modelLoading 
