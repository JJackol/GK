#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <GL/gl.h>
#include <GL/glut.h>

// Funkcaja określająca, co ma być rysowane
// (zawsze wywoływana, gdy trzeba przerysować scenę)
//const char*  zielony = {10, 120, 10};
void rect(float x, float y, float a, float b)
{
	glBegin(GL_TRIANGLES);
		glVertex2f(x, y);
		glVertex2f(x+a, y);
		glVertex2f(x+a, y+b);

		glVertex2f(x+a, y+b);
		glVertex2f(x, y+b);
		glVertex2f(x, y);
	glEnd();
}

void rect2(float x, float y, float a, float b)
{
	glColor3f(0.1, 0.9, 0.1);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(x, y);
		glVertex2f(x+a, y);
		glVertex2f(x+a, y+b);
		glVertex2f(x, y+b);
		glVertex2f(x, y);
	glEnd();
}

void triangle(){
	glBegin(GL_TRIANGLES);

	glColor3f(0.1, 0.9, 0.1);
	glVertex2f(-40.0, -30.0);
	glColor3f(0.9, 0.1, 0.1);
	glVertex2f(40.0, -30.0);
	glColor3f(0.1, 0.1, 0.9);
	glVertex2f(0.0, 30.0);

	glEnd();
}

float mrand(){
	return  (float)rand()/(float)(RAND_MAX);
}

void sierp(float x, float y, float a)
{
	if (a<3)
		return;
	glColor3f(0.9, 0.9, 0.4);
	rect(x, y, a, a);
	a = a/3;

	sierp(x, y , a);
	sierp(x+a, y, a);
	sierp(x+2*a , y, a);

	sierp(x, y+a, a);
	sierp(x+a, y+a, a);
	sierp(x+2*a, y+a, a);

	sierp(x, y+2*a, a);
	sierp(x+a, y+2*a, a);
	sierp(x+2*a, y+2*a, a);

	float r = 0.7*mrand() + 0.2;
	float b = 0.7*mrand() + 0.2;
	glColor3f(r, 0.1, b);
	rect(x+a, y+a, a, a);
}


float X=-80.0;
float Y=-80.0;
float A=136.0;
float B=130.0;
float cols[] = {0.5, 0.5, 0.5};
void rand_rect()
{
	float r;
	r = mrand()-0.5;
	X += r*2;
	r = mrand()-0.5;
	Y += r*2;
	r = mrand()-0.5;
	A += r*3;
	r = mrand()-0.5;
	B += r*3;
	cols[0] += 0.10*(mrand()-0.5);
	cols[1] += 0.10*(mrand()-0.5);
	cols[2] += 0.10*(mrand()-0.5);
	glColor3f(cols[0], cols[1], cols[2]);

	rect(X, Y, A, B);
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszczącym

	glColor3f(0.1, 0.9, 0.1);
	rect(-33.0, -31.0, 31.0, 63.0);
	rect2(32.0, -31.0, 31.0, 63.0);
	triangle();
	sierp(-80.0, -80.0, 160.0);

	glClear(GL_COLOR_BUFFER_BIT);
	float r, g, b;
	r=mrand();
	r=mrand();
	r=mrand();
	glColor3f(r, g, b);
	rand_rect();
	glFlush();
	// Przekazanie poleceń rysujących do wykonania
}


void MyInit(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	// Kolor okna wnętrza okna - ustawiono na szary
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
     GLfloat AspectRatio;
    if(vertical==0)
        vertical = 1;

     glViewport(0, 0, horizontal, vertical);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    AspectRatio = (GLfloat)horizontal/(GLfloat)vertical;
    if(horizontal <= vertical)
    glOrtho(-100.0,100.0,-100.0/AspectRatio,100.0/AspectRatio,1.0,-1.0);

     else
    glOrtho(-100.0*AspectRatio,100.0*AspectRatio,-100.0,100.0,1.0,-1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void main(int argc, char** argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutCreateWindow("Pierwszy program w OpenGL");
	glutReshapeFunc(ChangeSize);

	glutDisplayFunc(RenderScene);

	MyInit();

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

