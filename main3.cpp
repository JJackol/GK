#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glut.h>

const float N = 32;
static GLfloat theta[] = {0.0, 0.0, 0.0}; // trzy kąty obrotu

static int T1 = 1;
static int T2 = 1;
static int model = 1;

typedef float point3[3]; 

void Axes(void)
{

    point3  x_min = {-5.0, 0.0, 0.0};
    point3  x_max = { 5.0, 0.0, 0.0};
    // początek i koniec obrazu osi x
    point3  y_min = {0.0, -5.0, 0.0};
    point3  y_max = {0.0,  5.0, 0.0};
    // początek i koniec obrazu osi y
    point3  z_min = {0.0, 0.0, -5.0};
    point3  z_max = {0.0, 0.0,  5.0};
    //  początek i koniec obrazu osi y
    glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
    glBegin(GL_LINES); // rysowanie osi x
        glVertex3fv(x_min);
        glVertex3fv(x_max);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
    glBegin(GL_LINES);  // rysowanie osi y
        glVertex3fv(y_min);
        glVertex3fv(y_max);                           
		glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
    glBegin(GL_LINES); // rysowanie osi z
        glVertex3fv(z_min);
        glVertex3fv(z_max);
    glEnd();
}

GLfloat jajkoX(float u, float v){return (-90*u*u*u*u*u + 225*u*u*u*u -270u*u*u*u + 180*u*u - 45*u) * cos( M_PI * (v) ); }
GLfloat jajkoY(float u, float v){return ( 160*u*u*u*u - 320*u*u*u + 160*u*u ) ; }
GLfloat jajkoZ(float u, float v){return (-90*u*u*u*u*u + 225*u*u*u*u -270u*u*u*u + 180*u*u - 45*u) * sin( M_PI * (v) ); }

void jajko()
{
	float u, v;
	float x, y, z;
	int i, j;

	glBegin(GL_POINTS);
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			//
			u = i / (N-1);
			v = j / (N-1);

			x = jajkoX(u, v);
			y = jajkoY(u, v);
			z = jajkoZ(u, v);
			y += -5;
			
			glVertex3f(x, y, z);
		}
	}
	glEnd();
}

void jajkoTri()
{
	float u, v;
	float x, y, z;
	int i, j;
	float r,g,b;
	r = 0.3;
	g = 0.3;
	b = 0.3;


	for (i = 0; i < N-1; ++i) {
		glBegin(GL_TRIANGLE_STRIP);
		for (j = 0; j < N; ++j) {
			u = i / (N-1);
			v = j / (N-1);
			x = jajkoX(u, v);
			y = jajkoY(u, v);
			z = jajkoZ(u, v);
			y += -5;

			g = cos(u * M_PI * 2* T1)/2 + 0.5;
			b = sin(v * M_PI * T2)/2 + 0.5;
			glColor3f(r, g, b);
			glVertex3f(x, y, z);

			u = (i+1)/ (N-1);
			v = j / (N-1);
			x = jajkoX(u, v);
			y = jajkoY(u, v);
			z = jajkoZ(u, v);
			y += -5;

			g = cos(u * M_PI * 2* T1)/2 + 0.5;
			b = sin(v * M_PI * T2)/2 + 0.5;
			glColor3f(r, g, b);
			glVertex3f(x, y, z);

		}
		glEnd();
	}
}

void jajkoWire()
{
	float u, v;
	float x, y, z;
	int i, j;

	for (i = 0; i < N; ++i) {
		glBegin(GL_LINE_STRIP);
		for (j = 0; j < N; ++j) {
			//
			u = i / (N-1);
			v = j / (N-1);

			x = jajkoX(u, v);
			y = jajkoY(u, v);
			z = jajkoZ(u, v);
			y += -5;
			
			glVertex3f(x, y, z);
		}
		glEnd();
	}
	for (j = 0; j < N; ++j) {
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < N; ++i) {
			//
			u = i / (N-1);
			v = j / (N-1);

			x = jajkoX(u, v);
			y = jajkoY(u, v);
			z = jajkoZ(u, v);
			y += -5;
			
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

void RenderScene(void)
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszczącym
    glLoadIdentity();
    // Czyszczenie macierzy bieżącej
    Axes();

	//glRotated(30.0, 1.0, 1.0, 1.0);
	glColor3f(1.0f, 1.0f, 1.0f); // Ustawienie koloru rysowania na biały

	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);

	switch (model) {
		case 1:
			jajko();
			break;
		case 2:
			jajkoWire();
			break;
		case 3:
			jajkoTri();
			break;
		default:
			glutWireTeapot(3.0);
	}

    glFlush();
    glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
	if(key == '=') T1++;
	if(key == '-') T1--;
	if(key == '0') T2++;
	if(key == '9') T2--;
    if(key == 'q') model = 1;//points
    if(key == 'w') model = 2;//wire
    if(key == 'e') model = 3;//strip
    if(key == 't') model = 4;//teapot
    RenderScene(); // przerysowanie obrazu sceny
}

void spinEgg()
{
    theta[0] -= 0.5;
    if( theta[0] > 360.0 ) theta[0] -= 360.0;
    theta[1] -= 0.5;
    if( theta[1] > 360.0 ) theta[1] -= 360.0;
    theta[2] -= 0.5;
    if( theta[2] > 360.0 ) theta[2] -= 360.0;
    glutPostRedisplay(); //odświeżenie zawartości aktualnego okna
}


void MyInit(void)
{
glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical )
{
    GLfloat AspectRatio;
    // Deklaracja zmiennej AspectRatio  określającej proporcję
    // wymiarów okna 
    if(vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
        vertical = 1;  

    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielkościokna okna widoku (viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)  
    glMatrixMode(GL_PROJECTION);
    // Przełączenie macierzy bieżącej na macierz projekcji 
    glLoadIdentity();
    // Czyszcznie macierzy bieżącej            
    AspectRatio = (GLfloat)horizontal/(GLfloat)vertical;
    // Wyznaczenie współczynnika  proporcji okna
    // Gdy okno nie jest kwadratem wymagane jest określenie tak zwanej
    // przestrzeni ograniczającej pozwalającej zachować właściwe
    // proporcje rysowanego obiektu.
    // Do okreslenia przestrzeni ograniczjącej służy funkcja
    // glOrtho(...)            
    if(horizontal <= vertical)
        glOrtho(-7.5,7.5,-7.5/AspectRatio,7.5/AspectRatio,10.0, -10.0); 
    else
        glOrtho(-7.5*AspectRatio,7.5*AspectRatio,-7.5,7.5,10.0,-10.0);                     

    glMatrixMode(GL_MODELVIEW);
    // Przełączenie macierzy bieżącej na macierz widoku modelu                                    
    glLoadIdentity();
    // Czyszcenie macierzy bieżącej
}

int main(int argc, char **argv)
{          
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
    glutInitWindowSize(300, 300);
    glutCreateWindow("Studio yayo");

    glutDisplayFunc(RenderScene);
	glutIdleFunc(spinEgg);
	glutKeyboardFunc(keys);

    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
    // zazmiany rozmiaru okna      
    MyInit();
    glEnable(GL_DEPTH_TEST);
    // Włączenie mechanizmu usuwania powierzchni niewidocznych
    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT

	return 0;
}
