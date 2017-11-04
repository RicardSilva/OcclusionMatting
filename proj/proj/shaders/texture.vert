#version 330

uniform mat4 m_pvm;
uniform mat4 m_viewModel;

layout (location = 0)in vec4 position;
layout (location = 1)in vec3 normal; 
layout (location = 2)in vec2 texCoord; 

out Data {
	vec2 texC;
} DataOut;

void main () {

	//vec4 pos = m_viewModel * position;
	DataOut.texC = texCoord;

	gl_Position = m_pvm * position;	
}