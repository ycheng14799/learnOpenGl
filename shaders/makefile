all: shadersClassUse.cpp shader_s.h shadersSinGreen.cpp shadersColorful.cpp ../glad.c 
	g++ -o shadersSinGreen shadersSinGreen.cpp ../glad.c -lglfw -ldl
	g++ -o shadersColorful shadersColorful.cpp ../glad.c -lglfw -ldl
	g++ -o shadersClassUse shadersClassUse.cpp ../glad.c shader_s.h -lglfw -ldl
clean: 
	$(RM) shadersSinGreen
	$(RM) shadersColorful
