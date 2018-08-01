all: transformations.cpp shader_s.h ../glad.c stb_image.h stb_image.cpp
	g++ -o transformations transformations.cpp shader_s.h ../glad.c -lglfw -ldl stb_image.h stb_image.cpp
	g++ -o matrixPlayground matrixPlayground.cpp
clean: 
	$(RM) transformations

