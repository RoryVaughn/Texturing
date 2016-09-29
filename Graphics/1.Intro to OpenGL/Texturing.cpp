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
	int imageWidth = 512, imageHeight = 512, imageFormat = 0;
	unsigned char* data = stbi_load("./textures/reform.jpg",
		&imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	// load normal map
	data = stbi_load("./textures/heart.png",
		&imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_normalmap);
	glBindTexture(GL_TEXTURE_2D, m_normalmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);
}
bool Texturing::generateGrid()
{
	texture();
	Vertex vertexData[] = {
		{ -5, 0, 5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
		{ 5, 0, 5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
		{ 5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ -5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },
	};
	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	// create and bind buffers to a vertex array object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Buffer Vertexes
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertexData, GL_STATIC_DRAW);


	//Buffer indicies
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), ((char*)0) + 48);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), ((char*)0) + 16);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), ((char*)0) + 32);



	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}
bool Texturing::CreateShader()
{
	// create shader
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec2 TexCoord; \
							layout(location=2) in vec4 Normal; \
							layout(location=3) in vec4 Tangent; \
							out vec2 vTexCoord; \
							out vec3 vNormal; \
							out vec3 vTangent; \
							out vec3 vBiTangent; \
							uniform mat4 ProjectionView; \
							void main() { vTexCoord = TexCoord; \
							vNormal = Normal.xyz; \
							vTangent = Tangent.xyz; \
							vBiTangent = cross(vNormal, vTangent); \
							gl_Position = ProjectionView * Position; }";
	const char* fsSource = "#version 410\n \
							in vec2 vTexCoord; \
							in vec3 vNormal; \
							in vec3 vTangent; \
							in vec3 vBiTangent; \
							out vec4 FragColor; \
							uniform vec3 LightDir; \
							uniform sampler2D diffuse; \
							uniform sampler2D normal; \
							void main() { \
							mat3 TBN = mat3( \
							normalize( vTangent ), \
							normalize( vBiTangent ), \
							normalize( vNormal )); \
							vec3 N = texture(normal, \
							vTexCoord).xyz * 2 - 1; \
							float d = max( 0, dot( \
							normalize( TBN * N ), \
							normalize( LightDir ))); \
							FragColor = texture(diffuse, vTexCoord); \
							FragColor.rgb = FragColor.rgb * d;}";

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
	int loc = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE,
		&(m_camera->getProjectionView()[0][0]));
	// set texture slots
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalmap);
	// tell the shader where it is
	loc = glGetUniformLocation(m_programID, "diffuse");
	glUniform1i(loc, 0);
	loc = glGetUniformLocation(m_programID, "normal");
	glUniform1i(loc, 1);
	// bind the light
	vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	loc = glGetUniformLocation(m_programID, "LightDir");
	glUniform3f(loc, light.x, light.y, light.z);
	// draw
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Texturing::inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	Texturing* myThis = (Texturing*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_SPACE)
		if (action == GLFW_PRESS)
			myThis->m_direction = true;
		else if (action == GLFW_RELEASE)
			myThis->m_direction = false;
}