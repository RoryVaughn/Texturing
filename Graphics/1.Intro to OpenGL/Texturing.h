#pragma once

#include "BaseApplication.h"
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <vector>
//#include <glm/mat4x2.hpp>
//#include "gl_core_4_4.h"

struct Vertex {
	glm::vec4 position;
	glm::vec2 texcoord;
};


// forward declared to reduce dependencies
class Camera;

class Texturing : public BaseApplication {
public:
	
	Texturing();
	virtual ~Texturing();
	void texture ();
	bool generateGrid();
	bool CreateShader();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update(float deltaTime);
	virtual void draw();

	static void inputCallback(GLFWwindow* window, int key, int scanline, int action, int mods);
	// our vertex and index buffers
	
private:
	
	Camera*		m_camera;
	
	// we can toggle the way the earth orbits the sun,
	// demonstating input callback
	bool		m_direction;
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_programID;
	unsigned int m_texture, m_perlin_texture;
	int m_count;
	int m_vertNum;
};