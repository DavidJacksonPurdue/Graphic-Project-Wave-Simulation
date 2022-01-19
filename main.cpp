//OPTION A WAVE SIMULATION

#ifdef _WIN32
#include <windows.h>
#endif

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include "GPGPU.h"

static int winWidth = 512;
static int winHeight = 512;
static int winId = -1;
static GLUI *glui;

float wave1Amp = 0.5;
float wave2Amp = 0.5;
float wave3Amp = 0.5;
float wave4Amp = 0.5;

float wave1Spe = 0.005;
float wave2Spe = 0.005;
float wave3Spe = 0.005;
float wave4Spe = 0.005;

float wave1Dirx = 1;
float wave1Diry = 1;
float wave2Dirx = 1;
float wave2Diry = 1;
float wave3Dirx = 1;
float wave3Diry = 1;
float wave4Dirx = 1;
float wave4Diry = 1;

float wave1Len = 2.5;
float wave2Len = 2.5;
float wave3Len = 2.5;
float wave4Len = 2.5;

int optionSelect = 0;

int islandSelect = 0;

GPGPU  *gpgpu;

/*****************************************************************************
*****************************************************************************/
static void
leftButtonDownCB(void)
{
   gpgpu->restart();
}

/*****************************************************************************
*****************************************************************************/
static void
leftButtonUpCB(void)
{
}

static void
start(void)
{
	gpgpu->amplitude[0] = wave1Amp;
	gpgpu->amplitude[1] = wave2Amp;
	gpgpu->amplitude[2] = wave3Amp;
	gpgpu->amplitude[3] = wave4Amp;
	gpgpu->spee[0] = wave1Spe;
	gpgpu->spee[1] = wave2Spe;
	gpgpu->spee[2] = wave3Spe;
	gpgpu->spee[3] = wave4Spe;
	
	//Every Direction vector is normalized
	float norm = sqrt(wave1Dirx * wave1Dirx + wave1Diry * wave1Diry);
	gpgpu->directions[0] = -1 * wave1Dirx / norm;
	gpgpu->directions[1] = -1 * wave1Diry / norm;
	norm = sqrt(wave2Dirx * wave2Dirx + wave2Diry * wave2Diry);
	gpgpu->directions[2] = -1 * wave2Dirx / norm;
	gpgpu->directions[3] = -1 * wave2Diry / norm;
	norm = sqrt(wave3Dirx * wave3Dirx + wave3Diry * wave3Diry);
	gpgpu->directions[4] = -1 * wave3Dirx / norm;
	gpgpu->directions[5] = -1 * wave3Diry / norm;
	norm = sqrt(wave4Dirx * wave4Dirx + wave4Diry * wave4Diry);
	gpgpu->directions[6] = -1 * wave4Dirx / norm;
	gpgpu->directions[7] = -1 * wave4Diry / norm;

	gpgpu->lengths[0] = wave1Len;
	gpgpu->lengths[1] = wave2Len;
	gpgpu->lengths[2] = wave3Len;
	gpgpu->lengths[3] = wave4Len;

	if (optionSelect == 0) {
		gpgpu->lightloc[0] = 0;
		gpgpu->lightloc[1] = 0;
	}
	else if (optionSelect == 1) {
		gpgpu->lightloc[0] = 5;
		gpgpu->lightloc[1] = 0;
	}
	else if (optionSelect == 2) {
		gpgpu->lightloc[0] = 5;
		gpgpu->lightloc[1] = 5;
	}
	else {
		gpgpu->lightloc[0] = 0;
		gpgpu->lightloc[1] = 5;
	}

	gpgpu->islandConfirm = islandSelect;

	gpgpu->restart();
}

/*****************************************************************************
*****************************************************************************/
static void
middleButtonDownCB(void)
{
}


/*****************************************************************************
*****************************************************************************/
static void
middleButtonUpCB(void)
{
}

/*****************************************************************************
*****************************************************************************/
static void
rightButtonDownCB(void)
{
	//printf("Dir %f\n", wave1Dir[0]);
	//printf("Dir %f\n", wave1Dir[1]);
	//printf("Dir %f\n", wave1Dir[4]);
	//printf("Dir %f\n", wave1Dir[5]);
}


/*****************************************************************************
*****************************************************************************/
static void
rightButtonUpCB(void)
{
}

/*****************************************************************************
*****************************************************************************/
static void
mouseCB(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
      leftButtonDownCB();
   else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
      leftButtonUpCB();
   else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
      middleButtonDownCB();
   else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
      middleButtonUpCB();
   else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
      rightButtonDownCB();
   else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
      rightButtonUpCB();
}


/*****************************************************************************
*****************************************************************************/
static void
motionCB(int x, int y)
{
}


/*****************************************************************************
*****************************************************************************/
void
reshapeCB(int width, int height)
{
    if (height == 0) height = 1;
    
    glViewport(0, 0, width, height);
    
	// Use orthographic projection
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();               
    gluOrtho2D(-1, 1, -1, 1);       
    glMatrixMode(GL_MODELVIEW);     
    glLoadIdentity();   
}

/*****************************************************************************
*****************************************************************************/
void
keyboardCB(unsigned char key, int x, int y)
{
}

/*****************************************************************************
*****************************************************************************/
void
idleFunc()
{	
	if (glutGetWindow() != winId) {
		glutSetWindow(winId);
	}
	glutPostRedisplay();
}

/*****************************************************************************
*****************************************************************************/
void
refreshCB()
{
	for (int i = 0; i < 10; ++i) {
		// Update the cells' states
		gpgpu->update();  

		// Display the results
		gpgpu->display();
	}

    glutSwapBuffers();
}

/*****************************************************************************
*****************************************************************************/
void initialize()
{
    // Initialize glew library
    glewInit();

    // Create the gpgpu object
    gpgpu = new GPGPU(512, 512);
}

/*****************************************************************************
*****************************************************************************/
void MakeGUI()
{
	glui = GLUI_Master.create_glui("GPGPU Settings", 0, 0, 0);
	glui->add_statictext("Wave Amplitude Settings");
	GLUI_Spinner *W1 = glui->add_spinner("Wave 1 Amplitude (0-1):", GLUI_SPINNER_FLOAT, &wave1Amp);
	W1->set_float_limits(0, 1);
	W1->set_float_val(0.5);
	GLUI_Spinner *W2 = glui->add_spinner("Wave 2 Amplitude (0-1):", GLUI_SPINNER_FLOAT, &wave2Amp);
	W2->set_float_limits(0, 1);
	W2->set_float_val(0.5);
	GLUI_Spinner *W3 = glui->add_spinner("Wave 3 Amplitude (0-1):", GLUI_SPINNER_FLOAT, &wave3Amp);
	W3->set_float_limits(0, 1);
	W3->set_float_val(0.5);
	GLUI_Spinner *W4 = glui->add_spinner("Wave 4 Amplitude (0-1):", GLUI_SPINNER_FLOAT, &wave4Amp);
	W4->set_float_limits(0, 1);
	W4->set_float_val(0.5);
	glui->add_separator();
	glui->add_statictext("Wave Speed Settings");
	GLUI_Spinner *WS1 = glui->add_spinner("Wave 1 Speed (0-0.01):", GLUI_SPINNER_FLOAT, &wave1Spe);
	WS1->set_float_limits(0, 0.01);
	WS1->set_float_val(0.0005);
	GLUI_Spinner *WS2 = glui->add_spinner("Wave 2 Speed (0-0.01):", GLUI_SPINNER_FLOAT, &wave2Spe);
	WS2->set_float_limits(0, 0.01);
	WS2->set_float_val(0.0005);
	GLUI_Spinner *WS3 = glui->add_spinner("Wave 3 Speed (0-0.01):", GLUI_SPINNER_FLOAT, &wave3Spe);
	WS3->set_float_limits(0, 0.01);
	WS3->set_float_val(0.0005);
	GLUI_Spinner *WS4 = glui->add_spinner("Wave 4 Speed (0-0.01):", GLUI_SPINNER_FLOAT, &wave4Spe);
	WS4->set_float_limits(0, 0.01);
	WS4->set_float_val(0.0005);
	glui->add_separator();
	glui->add_statictext("Wave Direction Settings");
	GLUI_Spinner *WD1 = glui->add_spinner("Wave 1 Dirx (-1-1):", GLUI_SPINNER_FLOAT, &wave1Dirx);
	WD1->set_float_limits(-1, 1);
	WD1->set_float_val(1);
	GLUI_Spinner *WD2 = glui->add_spinner("Wave 1 Diry (-1-1)", GLUI_SPINNER_FLOAT, &wave1Diry);
	WD2->set_float_limits(-1, 1);
	WD2->set_float_val(1);
	GLUI_Spinner *WD3 = glui->add_spinner("Wave 2 Dirx (-1-1):", GLUI_SPINNER_FLOAT, &wave2Dirx);
	WD3->set_float_limits(-1, 1);
	WD3->set_float_val(1);
	GLUI_Spinner *WD4 = glui->add_spinner("Wave 2 Diry (-1-1):", GLUI_SPINNER_FLOAT, &wave2Diry);
	WD4->set_float_limits(-1, 1);
	WD4->set_float_val(1);
	GLUI_Spinner *WD5 = glui->add_spinner("Wave 3 Dirx (-1-1):", GLUI_SPINNER_FLOAT, &wave3Dirx);
	WD5->set_float_limits(-1, 1);
	WD5->set_float_val(1);
	GLUI_Spinner *WD6 = glui->add_spinner("Wave 3 Diry (-1-1):", GLUI_SPINNER_FLOAT, &wave3Diry);
	WD6->set_float_limits(-1, 1);
	WD6->set_float_val(1);
	GLUI_Spinner *WD7 = glui->add_spinner("Wave 4 Dirx (-1-1):", GLUI_SPINNER_FLOAT, &wave4Dirx);
	WD7->set_float_limits(-1, 1);
	WD7->set_float_val(1);
	GLUI_Spinner *WD8 = glui->add_spinner("Wave 4 Diry (-1-1):", GLUI_SPINNER_FLOAT, &wave4Diry);
	WD8->set_float_limits(-1, 1);
	WD8->set_float_val(1);
	glui->add_separator();
	GLUI_Spinner *WL1 = glui->add_spinner("Wave 1 Wavelength (1-5):", GLUI_SPINNER_FLOAT, &wave1Len);
	WL1->set_float_limits(1,5);
	WL1->set_float_val(2.5);
	GLUI_Spinner *WL2 = glui->add_spinner("Wave 2 Wavelength (1-5):", GLUI_SPINNER_FLOAT, &wave2Len);
	WL2->set_float_limits(1,5);
	WL2->set_float_val(2.5);
	GLUI_Spinner *WL3 = glui->add_spinner("Wave 3 Wavelength (1-5):", GLUI_SPINNER_FLOAT, &wave3Len);
	WL3->set_float_limits(1,5);
	WL3->set_float_val(2.5);
	GLUI_Spinner *WL4 = glui->add_spinner("Wave 4 Wavelength (1-5):", GLUI_SPINNER_FLOAT, &wave4Len);
	WL4->set_float_limits(1,5);
	WL4->set_float_val(2.5);
	glui->add_separator();
	GLUI_Panel *light_panel = glui->add_panel("Light Location:");
	GLUI_RadioGroup *options = glui->add_radiogroup_to_panel(light_panel, &optionSelect);
	glui->add_radiobutton_to_group(options, "Bottom Left");
	glui->add_radiobutton_to_group(options, "Bottom Right");
	glui->add_radiobutton_to_group(options, "Top Right");
	glui->add_radiobutton_to_group(options, "Top Left");
	glui->add_separator();
	GLUI_Checkbox *islandCheck = glui->add_checkbox("Include \"Island\"?", &islandSelect);
	glui->add_separator();

	glui->add_button("Restart", 0, (GLUI_Update_CB)start);
	glui->set_main_gfx_window(winId);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(idleFunc);
}

/*****************************************************************************
*****************************************************************************/
int
main(int argc, char *argv[])
{
	// init OpenGL/GLUT
	glutInit(&argc, argv);
	
	// create main window
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(winWidth, winHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	winId = glutCreateWindow("MyWindow");

	initialize();
	
	// setup callbacks
	glutDisplayFunc(refreshCB);
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseCB);
	glutMotionFunc(motionCB);
	glutIdleFunc(idleFunc);
	
	// force initial matrix setup
	reshapeCB(winWidth, winHeight);

	// set modelview matrix stack to identity
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// make GLUI GUI
	MakeGUI();
	glutMainLoop();

	return (TRUE);
}
