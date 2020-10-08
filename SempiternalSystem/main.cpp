#include<GL/glut.h>
//#include<GL/glew.h>
#include "EternalSystem.h"
#include "Configuration.h"

#pragma comment(lib, "ws2_32.lib")

//渲染指令综合处理函数
void RenderPhase();

//键盘键按下处理函数(for GLUT)
//p_key: 按下键的值
//p_x: 事件发生时，鼠标所处位置（x
//p_y: 时间发生时，鼠标所处位置（y
void KeyDownHandler(unsigned char p_key, int p_x, int p_y);

void KeyUpHandler(unsigned char p_key, int p_x, int p_y);

int main(int argc, char** argv)
{
	printf("Eternal System Starting Up...\n==================================\n");
	printf("Loading configuration...\n");
	Configuration* cfg = new Configuration();
	if (cfg->LoadConfigurationFromFile(".\\esc.txt"))
	{
		fprintf(stderr, "Load Config File Error.\n");
	}
	printf("Preparing completed successfully.\n");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);//double buffering, rgba mode
	glutInitWindowSize(cfg->videoHSize, cfg->videoVSize);
	glutInitWindowPosition(50, 40);
	glutCreateWindow("Eternal System");
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glutDisplayFunc(RenderPhase);
	glutKeyboardFunc(KeyDownHandler);
	glutKeyboardUpFunc(KeyUpHandler);
	glutMainLoop();
	return 0;
}

void RenderPhase()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

void KeyDownHandler(unsigned char p_key, int p_x, int p_y)
{
}

void KeyUpHandler(unsigned char p_key, int p_x, int p_y)
{
}
