#pragma once
#include <string>
#include <GL\glew.h>
#include <vector>

class Texture
{
public:
	Texture();

	void Bind(unsigned int unit); // bind upto 32 textures
	void init(const std::string& fileName);

	GLuint loadCubemap(std::vector<std::string> faces);


	~Texture();
	GLint getTexHandler() { return textureHandler; }

protected:
private:

	GLuint textureHandler;
};

