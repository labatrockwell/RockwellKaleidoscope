#version 120

//layout(location = 0) in vec4 in_position;

void main()
{    
	vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    gl_Position = pos;
}