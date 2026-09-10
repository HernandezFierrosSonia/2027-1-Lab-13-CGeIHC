#version 330
layout (location =0) in vec3 pos; //s posición 0. Coincide exactamente con glVertexAttribPointer(0, 3, GL_FLOAT, ...) de Mesh.cpp
out vec4 vColor; //viaja al fragment shader shaderrojo.frag
uniform mat4 model;
uniform mat4 projection;
void main()
{
	gl_Position=projection*model*vec4(pos,1.0f);//toma la posición local, le aplica model (T/R/S) y luego projection (perspectiva o orto)
	vColor=vec4(1.0f,0.0,0.0f,1.0f);//el color rojo está fijo, escrito a mano
}