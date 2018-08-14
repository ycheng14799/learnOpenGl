all: cameraExercises.cpp camera.cpp shader_s.h ../glad.c stb_image.h stb_image.cpp
	g++ -o camera camera.cpp shader_s.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp
	g++ -o cameraExercises cameraExercises.cpp shader_s.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp
clean: 
	$(RM) camera
