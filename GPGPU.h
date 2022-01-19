#pragma once

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>

class GPGPU
{
public:
	GPGPU(int w, int h);

	void restart();
	void update();
	void display();
	int loadShader(char* filename, std::string& text);

private:
    int _width;				// width of the screen
	int _height;			// height of the screen
	int _initialized;		// if the cells are initialized (=1) or not (=0)
	float _time;
	float _timeAtReset;
	int frame_count;

public: float spee[4];
public: float amplitude[4];
public: float directions[8];
public: float lengths[4];
public: float lightloc[2];
public: int islandConfirm;

	GLuint _initializedLoc;
    GLuint _texUnitLoc;
	GLuint _timeLoc;
	GLuint _ampLoc;
	GLuint _speedLoc;
	GLuint _ampLoc1;
	GLuint _speedLoc1;
	GLuint _ampLoc2;
	GLuint _speedLoc2;
	GLuint _ampLoc3;
	GLuint _speedLoc3;
	GLuint _dirxLoc;
	GLuint _diryLoc;
	GLuint _dirxLoc1;
	GLuint _diryLoc1;
	GLuint _dirxLoc2;
	GLuint _diryLoc2;
	GLuint _dirxLoc3;
	GLuint _diryLoc3;
	GLuint _lenLoc;
	GLuint _lenLoc1;
	GLuint _lenLoc2;
	GLuint _lenLoc3;
	GLuint _ligLoc;
	GLuint _ligLoc1;
	GLuint _islandConLoc;

    GLuint _textureId;		// The texture ID used to store data array
    GLuint _programId;		// the program ID

    GLuint _fragmentShader;
};

