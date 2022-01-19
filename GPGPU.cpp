//OPTION A WAVE SIMULATION

#include "GPGPU.h"
#include <iostream>
#include <fstream>

GPGPU::GPGPU(int w, int h) : _initialized(0), _width(w), _height(h)
{
    // Create a texture to store the framebuffer
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGB, GL_FLOAT, 0);

    _programId = glCreateProgram();

	// Load fragment shader which will be used as computational kernel
	std::string edgeFragSource2;
	loadShader("fragment.glsl", edgeFragSource2);

    // Create the fragment program
    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* source = edgeFragSource2.c_str();
    glShaderSource(_fragmentShader, 1, &source, NULL);
    glCompileShader(_fragmentShader);
    glAttachShader(_programId, _fragmentShader);

    // Link the shader into a complete GLSL program.
    glLinkProgram(_programId);

	int infologLength1 = 0;
	glGetShaderiv(_fragmentShader, GL_INFO_LOG_LENGTH, &infologLength1);
	if (infologLength1 > 0) {
		char *infoLog1 = (char *)malloc(infologLength1);
		glGetShaderInfoLog(_fragmentShader, infologLength1, NULL, infoLog1);
		printf("%s\n", infoLog1);
		free(infoLog1);
	}
	// Check program
	{
		int infologLength = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 0) {
			char *infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(_programId, infologLength, NULL, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}

    // Get location of the uniform variables
    _texUnitLoc = glGetUniformLocation(_programId, "texUnit");
	_initializedLoc = glGetUniformLocation(_programId, "initialized");
    _timeLoc = glGetUniformLocation(_programId, "time");
    _ampLoc = glGetUniformLocation(_programId, "amp");
	_speedLoc = glGetUniformLocation(_programId, "speed");
	_ampLoc1 = glGetUniformLocation(_programId, "amp1");
	_speedLoc1 = glGetUniformLocation(_programId, "speed1");
	_ampLoc2 = glGetUniformLocation(_programId, "amp2");
	_speedLoc2 = glGetUniformLocation(_programId, "speed2");
	_ampLoc3 = glGetUniformLocation(_programId, "amp3");
	_speedLoc3 = glGetUniformLocation(_programId, "speed3");
	_dirxLoc = glGetUniformLocation(_programId, "dirx");
	_diryLoc = glGetUniformLocation(_programId, "diry");
	_dirxLoc1 = glGetUniformLocation(_programId, "dirx1");
	_diryLoc1 = glGetUniformLocation(_programId, "diry1");
	_dirxLoc2 = glGetUniformLocation(_programId, "dirx2");
	_diryLoc2 = glGetUniformLocation(_programId, "diry2");
	_dirxLoc3 = glGetUniformLocation(_programId, "dirx3");
	_diryLoc3 = glGetUniformLocation(_programId, "diry3");
	_lenLoc = glGetUniformLocation(_programId, "length");
	_lenLoc1 = glGetUniformLocation(_programId, "length1");
	_lenLoc2 = glGetUniformLocation(_programId, "length2");
	_lenLoc3 = glGetUniformLocation(_programId, "length3");
	_ligLoc = glGetUniformLocation(_programId, "lightx");
	_ligLoc1 = glGetUniformLocation(_programId, "lighty");
	_islandConLoc = glGetUniformLocation(_programId, "island");

	spee[0] = 0.0005;
	spee[1] = 0.0005;
	spee[2] = 0.0005;
	spee[3] = 0.0005;

	amplitude[0] = 0.5;
	amplitude[1] = 0.5;
	amplitude[2] = 0.5;
	amplitude[3] = 0.5;

	directions[0] = -1 * 1/sqrt(2.0);
	directions[1] = -1 * 1/sqrt(2.0);
	directions[2] = -1 * 1/sqrt(2.0);
	directions[3] = -1 * 1/sqrt(2.0);
	directions[4] = -1 * 1/sqrt(2.0);
	directions[5] = -1 * 1/sqrt(2.0);
	directions[6] = -1 * 1/sqrt(2.0);
	directions[7] = -1 * 1/sqrt(2.0);

	lengths[0] = 1;
	lengths[1] = 1;
	lengths[2] = 1;
	lengths[3] = 1;

	lightloc[0] = 0;
	lightloc[1] = 0;

	islandConfirm = 0;

	_timeAtReset = 0;
	frame_count = 0;
}

void GPGPU::restart()
{
	_initialized = 0;
    _timeAtReset = _time;
}

void GPGPU::update()
{
    // Backup the viewport dimensions
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    glViewport(0, 0, _width, _height);
        
    // Copy the results to the texture
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glUseProgram(_programId);
            
    // Set the uniform variables
    glUniform1i(_texUnitLoc, 0);
	glUniform1i(_initializedLoc, _initialized);
	_initialized = 1;
    _time = glutGet(GLUT_ELAPSED_TIME);
	//_time = frame_count / 60;
	//frame_count++;
	_time -= _timeAtReset;
    glUniform1f(_timeLoc, _time);
	glUniform1f(_ampLoc, amplitude[0]);
	glUniform1f(_speedLoc, spee[0]);
	glUniform1f(_ampLoc1, amplitude[1]);
	glUniform1f(_speedLoc1, spee[1]);
	glUniform1f(_ampLoc2, amplitude[2]);
	glUniform1f(_speedLoc2, spee[2]);
	glUniform1f(_ampLoc3, amplitude[3]);
	glUniform1f(_speedLoc3, spee[3]);
	glUniform1f(_dirxLoc, directions[0]);
	glUniform1f(_diryLoc, directions[1]);
	glUniform1f(_dirxLoc1, directions[2]);
	glUniform1f(_diryLoc1, directions[3]);
	glUniform1f(_dirxLoc2, directions[4]);
	glUniform1f(_diryLoc2, directions[5]);
	glUniform1f(_dirxLoc3, directions[6]);
	glUniform1f(_diryLoc3, directions[7]);
	glUniform1f(_lenLoc, lengths[0]);
	glUniform1f(_lenLoc1, lengths[1]);
	glUniform1f(_lenLoc2, lengths[2]);
	glUniform1f(_lenLoc3, lengths[3]);
	glUniform1f(_ligLoc, lightloc[0]);
	glUniform1f(_ligLoc1, lightloc[1]);
	glUniform1i(_islandConLoc, islandConfirm);
	//glUniform4f(_ampLoc, amplitude[0], amplitude[1], amplitude[2], amplitude[3]);
	//glUniform4f(_speedLoc, spee[0], spee[1], spee[2], spee[3]);
	// By drawing a quad, the fragment shader will be called for each pixel.
    glBegin(GL_QUADS);
    {            
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -0.5f);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -0.5f);
        glTexCoord2f(1, 1); glVertex3f( 1,  1, -0.5f);
        glTexCoord2f(0, 1); glVertex3f(-1,  1, -0.5f);
    }
    glEnd();

    glUseProgram(0);
        
    // Copy the rendered image to the texture so that we can start from the current state in the next iteration
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
        
    // Restore the stored viewport dimensions
	glViewport(vp[0], vp[1], vp[2], vp[3]);
}

void GPGPU::display()
{
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glEnable(GL_TEXTURE_2D);

    // Render the result image.
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -0.5f);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -0.5f);
        glTexCoord2f(1, 1); glVertex3f( 1,  1, -0.5f);
        glTexCoord2f(0, 1); glVertex3f(-1,  1, -0.5f);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

int GPGPU::loadShader(char* filename, std::string& text)
{
	std::ifstream ifs;
	ifs.open(filename, std::ios::in);

	std::string line;
	while (ifs.good()) {
        getline(ifs, line);

		text += line + "\n";
    }

	return 0;
}