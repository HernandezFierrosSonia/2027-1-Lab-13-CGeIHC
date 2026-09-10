#version 330
layout (location = 0) in vec3 pos;
out vec4 vColor;
uniform mat4 model;
uniform mat4 projection;
void main()
{
	gl_Position = projection * model * vec4(pos, 1.0f);
	vColor = vec4(0.6f, 0.2f, 0.8f, 1.0f);
}
