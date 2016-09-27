#include "SolarSystemApplication.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include <iostream>
#include <vector>
#include <stb_image.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;


SolarSystemApplication::SolarSystemApplication()
	: m_camera(nullptr),
	m_direction(false) {

}

SolarSystemApplication::~SolarSystemApplication() {

}

Vertex* SolarSystemApplication::genSemiCircle(const int points)
{
	Vertex* Vertices = new Vertex[points];

	for (int Vert = 0; Vert < points / m_slices; Vert++)
	{
		float theta = (glm::pi<float>() * Vert) / (m_slices - 1);
		Vertices[Vert].position = vec4(r * sin(theta), r * cos(theta), 0, 1);
		Vertices[Vert].color = vec4(0, 0, 255, 1);
	}
	return Vertices;
}

Vertex * SolarSystemApplication::latheSphere(Vertex* Verts, int meridians)
{
	Verts = new Vertex[m_points];
	for (int currentSlice = 0; currentSlice < meridians; currentSlice++)
	{
		float theta = glm::pi<float>() * m_points / (m_slices - 1);
		float oldX = (sin(theta) * r);
		float oldZ = 0;
		float phi = ((glm::pi<float>() * 2 * currentSlice) / meridians);
		float newX = oldX * cos(phi) + oldZ * sin(phi);
		float newZ = oldZ * cos(phi) - oldX * sin(phi);
		for (int currentPoint = 0; currentPoint < m_points; currentPoint++)
		{

			Verts[currentPoint].position = vec4(newX, (cos(theta) * r), newZ, 1);
			Verts[currentPoint].position = vec4(newX, (cos(theta) * r), newZ, 1);
		}
		phi = ((glm::pi<float>() * 2 * currentSlice) / meridians);
		newX = oldX * cos(phi) + oldZ * sin(phi);
		newZ = oldZ * cos(phi) - oldX * sin(phi);
		oldX = newX;
		oldZ = newZ;
	}
	return Verts;
}
void SolarSystemApplication::texture()
{
	int imageWidth = 512, imageHeight = 512, imageFormat = 0;
	unsigned char* data = stbi_load("./textures/crate.png",
		&imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);
}
bool SolarSystemApplication::generateGrid()
{

	texture();
	float vertexData[] = {
		-5, 0, 5, 1, 0, 1,
		5, 0, 5, 1, 1, 1,
		5, 0, -5, 1, 1, 0,
		-5, 0, -5, 1, 0, 0,
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);


	//Buffer indicies
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}
bool SolarSystemApplication::CreateShader()
{

	// create shader
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec2 TexCoord; \
							out vec2 vTexCoord; \
							uniform mat4 ProjectionView; \
							void main() { \
							vTexCoord = TexCoord; \
							gl_Position= ProjectionView * Position;\
							}";
	const char* fsSource = "#version 410\n \
							in vec2 vTexCoord; \
							out vec4 FragColor; \
							uniform sampler2D diffuse; \
							void main() { \
							FragColor = texture(diffuse,vTexCoord);\
							}";
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
bool SolarSystemApplication::startup() {

	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);

	// start the gizmo system that can draw basic immediate-mode shapes


	// setup camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));


	//
	generateGrid();
	CreateShader();
	// set input callbacksetInputCallback(inputCallback);
	return true;
}

void SolarSystemApplication::shutdown() {

	// cleanup camera and gizmos
	delete m_camera;


	// destroy our window properly
	destroyWindow();
}

bool SolarSystemApplication::update(float deltaTime) {

	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);


	// return true, else the application closes
	return true;
}

void SolarSystemApplication::draw() {
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// use our texture program
	glUseProgram(m_programID);
	// bind the camera
	int loc = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE,
		&(m_camera->getProjectionView()[0][0]));
	// set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	// tell the shader where it is
	loc = glGetUniformLocation(m_programID, "diffuse");
	glUniform1i(loc, 0);
	// draw
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void SolarSystemApplication::inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	SolarSystemApplication* myThis = (SolarSystemApplication*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_SPACE)
		if (action == GLFW_PRESS)
			myThis->m_direction = true;
		else if (action == GLFW_RELEASE)
			myThis->m_direction = false;
}