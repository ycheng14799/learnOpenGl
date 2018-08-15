all: instancing.cpp asteroidField.cpp ../glad.c camera.h shader_m.h model.h stb_image.cpp stb_image.h mesh.h
	g++ -o instancing instancing.cpp ../glad.c camera.h shader_m.h -lglfw -ldl -std=gnu++17
	g++ -o asteroidField asteroidField.cpp ../glad.c camera.h shader_m.h model.h -lglfw -ldl -lassimp -std=gnu++17 stb_image.cpp stb_image.h mesh.h
clean:
	$(RM) instancing
	$(RM) asteroidField
