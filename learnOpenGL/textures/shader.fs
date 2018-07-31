#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue; 

void main()
{
    FragColor = mix(mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue), vec4(ourColor,1.0), 0.3);
}
