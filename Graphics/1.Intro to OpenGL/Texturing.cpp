#include "Texturing.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include <iostream>
#include <stb_image.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;

Texturing::Texturing()
	: m_camera(nullptr),
	m_direction(false) {

}

Texturing::~Texturing() {

}

void Texturing::texture()
{
	/*int dims = 64;
	float *perlin_data = new float[dims * dims];
	float scale = (1.0f / dims) * 3;
	int octaves = 26;
	for (int x = 0; x < dims; ++x)
	{
		for (int y = 0; y < dims; ++y)
		{
			float amplitude = 1.8f;
			float persistence = 0.1f;
			perlin_data[y * x + x] = 0;
			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample =
					glm::perlin(glm::vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				perlin_data[y * x + x] += perlin_sample * amplitude;
				amplitude *= persistence;
			}
		}
	}
	glGenTextures(1, &m_perlin_texture);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, perlin_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/


	/*int imageWidth = 512, imageHeight = 512, imageFormat = 0;
	unsigned char* data = stbi_load("./textures/reform.jpg",
		&imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);*/

	//data = stbi_load("./textures/lightening.png",
	//	&imageWidth, &imageHeight, &imageFormat, STBI_default);

	//glGenTextures(1, &m_texture2);
	//glBindTexture(GL_TEXTURE_2D, m_texture2);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//stbi_image_free(data);

	//// load normal map
	//data = stbi_load("./textures/heart.png",
	//	&imageWidth, &imageHeight, &imageFormat, STBI_default);

	//glGenTextures(1, &m_normalmap);
	//glBindTexture(GL_TEXTURE_2D, m_normalmap);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//stbi_image_free(data);
}



bool Texturing::generateGrid()
{
	texture();
	/*struct Vertex {
		float x, y, z, w;
		float nx, ny, nz, nw;
		float tx, ty, tz, tw;
		float s, t;
	};*/

	/*Vertex vertexData[] = {
		{ -5, 0, 5, 1,
		0, 1, 0, 0,
		1, 0, 0, 0,
		0, 1 },


		{ 5, 0, 5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
		{ 5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ -5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },
	};
	unsigned int indexData[] = {
		0, 1, 2,
		2, 3, 0,
	};*/
	unsigned int vrow = 8;
	unsigned int vcol = 8;
	m_vertNum = vrow * vcol;
		Vertex* vertexData = new Vertex[m_vertNum];
		int p = 0;
		for (float row = 0; row < vrow; ++row)
		{
			for (float column = 0; column < vcol; ++column)
			{
				//x  y   z  w  u  v
				vertexData[p].position = vec4(column - vcol * 10, 0, row - vrow * 10, 1);//tl
				vertexData[p].texcoord = glm::vec2(column * (1.0f / vcol), row * (1.0f / vrow));
				p++;

			}
		}
		m_count = (vrow - 1) * (vcol - 1) * 6;
		unsigned int* indexData = new unsigned int[m_count];
		unsigned int l = 0;
		for (unsigned int j = 0; j < (vrow-1) * (vcol-1); ++j)
		{
			indexData[l] =  l;
			l++;
			indexData[l] =  l;
			l++;
			indexData[l] =  l;
			l++;
			indexData[l] =  l - 1;
			l++;
			indexData[l] =  l - 1;
			l++;
			indexData[l] =  l - 5;
			l++;
		}
		
			
		

	
		

	// create and bind buffers to a vertex array object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Buffer Vertexes
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertNum, vertexData, GL_STATIC_DRAW);


	//Buffer indicies
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_count, indexData, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,		sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,		sizeof(Vertex), ((char*)0) +  16);
	
	
	/*glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,		sizeof(Vertex), ((char*)0) + 16);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE,		sizeof(Vertex), ((char*)0) + 32);*/



	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}
bool Texturing::CreateShader()
{
	// create shader
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 position; \
							layout(location = 1) in vec2 texcoord; \
							uniform mat4 view_proj; \
							out vec2 frag_texcoord; \
							uniform sampler2D perlin_texture; \
							void main() {vec4 pos = position; \
							frag_texcoord = texcoord; \
							gl_Position = view_proj * pos; \
							pos.y += texture(perlin_texture, texcoord).r * 5;}";
							

	const char* fsSource = "#version 410\n \
							in vec2 frag_texcoord; \
							out vec4 out_color; \
							uniform sampler2D perlin_texture; \
							void main() { \
							out_color = texture(perlin_texture, frag_texcoord).rrrr;\
							out_color.a = 1;}";
						
							

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}
bool Texturing::startup() {

	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);
	// setup camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));
	vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	generateGrid();
	CreateShader();
	// set input callbacksetInputCallback(inputCallback);
	return true;
}

void Texturing::shutdown() {
	// cleanup camera and gizmos
	delete m_camera;
	// destroy our window properly
	destroyWindow();
}

bool Texturing::update(float deltaTime) {

	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;
	// update the camera's movement
	m_camera->update(deltaTime);
	// return true, else the application closes
	return true;
}

void Texturing::draw() {
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// use our texture program
	glUseProgram(m_programID);
	// bind the camera
	int loc0 = glGetUniformLocation(m_programID, "view_proj");
	glUniformMatrix4fv(loc0, 1, GL_FALSE, &(m_camera->getProjectionView()[0][0]));
	//// set texture slots
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);
	// tell the shader where it is
	//int loc1 = glGetUniformLocation(m_programID, "texture");
	//glUniform1i(loc1, 0);
	int loc2 = glGetUniformLocation(m_programID, "perlin_texture");
	glUniform1i(loc2, 0);
	// bind the light
	// draw
	glPointSize(5.0f);
	glBindVertexArray(m_VAO);
	glDrawElements(GL_POINTS, 64, GL_UNSIGNED_INT, nullptr);
}

void Texturing::inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	Texturing* myThis = (Texturing*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_SPACE)
		if (action == GLFW_PRESS)
			myThis->m_direction = true;
		else if (action == GLFW_RELEASE)
			myThis->m_direction = false;
}