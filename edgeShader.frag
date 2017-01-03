#version 330 core
in vec4 vertexColor; // Specify a color output to the fragment shade
in vec3 vertPosition;
out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform vec4 ourColor; // We set this variable in the OpenGL code.
void main()
{
       color = vertexColor;
	   if(vertPosition.x 

}