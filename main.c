#include <stdio.h>
#include <stdlib.h>


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

void sierp(float x, float y, float a)
{
	if (a<10)
		return;
	float new_a = a/3;
	glColor3f(0.1, 0.9, 0.1);
	rect(x, y, a, a);


	glColor3f(0.1, 0.1, 0.3);
	rect(x+a/3, y+a/3, a/3, new_a);

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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutCreateWindow("Pierwszy program w OpenGL");
	glutReshapeFunc(ChangeSize);

	glutDisplayFunc(RenderScene);

	MyInit();

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

