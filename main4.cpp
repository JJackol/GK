#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glut.h>

const float N = 16;
static GLfloat theta[] = {0.0, 0.0, 0.0}; // trzy kąty obrotu
static GLfloat fi[] = {0.0, 0.0, 0.0}; // trzy kąty obrotu
static GLfloat up[] = {0.0, 1.0, 0.0}; // trzy kąty obrotu
static GLfloat viewer[] = {0.0, 0.0, 15.0};
static int T1 = 1;
static int T2 = 1;
static int model = 1;
static int rel = 0;
static int wheel = 0;
static GLfloat pseu_zoom = 1;
static GLfloat zoom = 1;
static GLfloat Radi = 15.0;
//static GLfloat theta = 0.0;   // kąt obrotu obiektu
static GLfloat pix2angle_x;     // przelicznik pikseli na stopnie
static GLfloat pix2angle_y;
static GLint status = 0;       // stan klawiszy myszy
static GLfloat  pix2radx;
static GLfloat  pix2rady;
                               // 1 - naciśnięty zostać lewy klawisz
static int x_pos_old=0;       // poprzednia pozycja kursora myszy
static int y_pos_old=0;       // poprzednia pozycja kursora myszy
static int delta_x = 0;        // różnica pomiędzy pozycją bieżącą
static int delta_y = 0;        // różnica pomiędzy pozycją bieżącą
                                      // i poprzednią kursora myszy
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
	r = 0.0;
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

			g = cos(u * M_PI * 4* T1)/2 + 0.5;
			b = sin(v * M_PI * 2* T2)/2 + 0.5;
			glColor3f(r, g, b);
			glVertex3f(x, y, z);

			u = (i+1)/ (N-1);
			v = j / (N-1);
			x = jajkoX(u, v);
			y = jajkoY(u, v);
			z = jajkoZ(u, v);
			y += -5;

			g = cos(u * M_PI * 4* T1)/2 + 0.5;
			b = sin(v * M_PI * 2* T2)/2 + 0.5;
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
    glLoadIdentity();

	if(status == 1 && !rel )                     // jeśli lewy klawisz myszy wcięnięty
	{
		if(wheel == 1)
		{
			pseu_zoom += 0.1;
		}
		else if(wheel == -1)
		{
			pseu_zoom -= 0.1;
		}
		wheel = 0;
		theta[0] += delta_x*pix2angle_x;    // modyfikacja kąta obrotu o kat proporcjonalny
		theta[1] += delta_y*pix2angle_y;    // modyfikacja kąta obrotu o kat proporcjonalny
	}                                  // do różnicy położeń kursora myszy
	if(status == 1 && rel )                     // jeśli lewy klawisz myszy wcięnięty
	{
		fi[0] += delta_x * pix2radx;
		fi[1] += delta_y * pix2rady;
		if(wheel == 1)
		{
			zoom += 0.1;
		}
		else if(wheel == -1)
		{
			zoom -= 0.1;
		}
		wheel = 0;
		//fi[0] += delta_x;
		//fi[1] += delta_y;
		
		viewer[0] = zoom * Radi * cos(fi[0]) * cos(fi[1]);
		viewer[1] = zoom * Radi * sin(fi[1]);
		viewer[2] = zoom * Radi * sin(fi[0]) * cos(fi[1]);
		if(delta_y != 0 )
		{
			GLfloat pre[3];
			pre[0] = up[0] * ((up[0]* viewer[0] + up[1]* viewer[1] + up[2]* viewer[2] ) / (up[0]*up[0] +up[1]*up[1] +up[2]*up[2]) );
			pre[1] = up[1] * ((up[0]* viewer[0] + up[1]* viewer[1] + up[2]* viewer[2] ) / (up[0]*up[0] +up[1]*up[1] +up[2]*up[2]) );
			pre[2] = up[2] * ((up[0]* viewer[0] + up[1]* viewer[1] + up[2]* viewer[2] )  / (up[0]*up[0] +up[1]*up[1] +up[2]*up[2]) );
			up[0] -= pre[0];
			up[1] -= pre[1];
			up[2] -= pre[2];
		}	
	}                                  
	gluLookAt(viewer[0],viewer[1],viewer[2], 0.0, 0.0, 0.0, up[0], up[1], up[2]);
    Axes();
	//glRotated(30.0, 1.0, 1.0, 1.0);
	glColor3f(1.0f, 1.0f, 1.0f); // Ustawienie koloru rysowania na biały

// do różnicy położeń kursora myszy
	glRotatef(theta[0], 0.0, 1.0, 0.0);  //obrót obiektu o nowy kąt
	glRotatef(theta[1], 0.0, 0.0, 1.0);  //obrót obiektu o nowy kąt
	glScalef(pseu_zoom, pseu_zoom, pseu_zoom);
	//glRotatef(theta[0], 1.0, 0.0, 0.0);
	//glRotatef(theta[1], 0.0, 1.0, 0.0);
	//glRotatef(theta[2], 0.0, 0.0, 1.0);

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
	if(key == '1') pseu_zoom += 0.1;
	if(key == '2') pseu_zoom -= 0.1;

	if(key == '3') zoom += 0.1;
	if(key == '4') zoom -= 0.1;

    if(key == 'q') model = 1;//points
    if(key == 'w') model = 2;//wire
    if(key == 'e') model = 3;//strip
    if(key == 't') model = 4;//teapot
    if(key == 'r') rel = !rel;//teapot
	//if(key == 23) exit(0);
    RenderScene(); // przerysowanie obrazu sceny
}


void mouse(int btn, int state, int x, int y)
{
	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN)       
	{
		x_pos_old=x;        // przypisanie aktualnie odczytanej pozycji kursora
		y_pos_old=y;        // przypisanie aktualnie odczytanej pozycji kursora
		status = 1;          // wcięnięty został lewy klawisz myszy
	} 
		// Wheel reports as button 3(scroll up) and button 4(scroll down)
	else if (btn == 3) 
	{
		wheel++;
		if(rel) zoom += 0.1;
		if(!rel) pseu_zoom += 0.1;
	}

	else if(btn == 4) // It's a wheel event
	{
		if(rel) zoom -= 0.1;
		if(!rel) pseu_zoom -= 0.1;
		wheel --;
	}
	//{
	//	wheel = 0;
	//	wheel = btn==3 ? 1 : -1;
	//	// Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
	//	if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
	//	wheel = btn==3 ? 1 : -1;
	//}
	else
		status = 0;          // nie został wcięnięty żaden klawisz
	glutPostRedisplay();
}

/*************************************************************************************/
// Funkcja "monitoruje" położenie kursora myszy i ustawia wartości odpowiednich
// zmiennych globalnych
void motion( GLsizei x, GLsizei y )
{
    delta_x = x-x_pos_old;     // obliczenie różnicy położenia kursora myszy
    delta_y = y-y_pos_old;     // obliczenie różnicy położenia kursora myszy
    x_pos_old = x;            // podstawienie bieżącego położenia jako poprzednie
    y_pos_old = y;            // podstawienie bieżącego położenia jako poprzednie
    glutPostRedisplay();     // przerysowanie obrazu sceny
}

/* void spinEgg() */
/* { */
/*     theta[0] -= 0.5; */
/*     if( theta[0] > 360.0 ) theta[0] -= 360.0; */
/*     theta[1] -= 0.5; */
/*     if( theta[1] > 360.0 ) theta[1] -= 360.0; */
/*     theta[2] -= 0.5; */
/*     if( theta[2] > 360.0 ) theta[2] -= 360.0; */
/*     glutPostRedisplay(); //odświeżenie zawartości aktualnego okna */
/* } */

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	viewer[0] = zoom * Radi * cos(fi[0]) * cos(fi[1]);
	viewer[1] = zoom * Radi * sin(fi[1]);
	viewer[2] = zoom * Radi * sin(fi[0]) * cos(fi[1]);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical )
{
	pix2angle_x = 360.0/(float)horizontal;  // przeliczenie pikseli na stopnie
	pix2angle_y = 360.0/(float)vertical;  // przeliczenie pikseli na stopnie
	pix2radx = M_PI*2/(float)horizontal;
	pix2rady = M_PI*2/(float)vertical;

    GLfloat AspectRatio;
    if(vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
        vertical = 1;  
    AspectRatio = (GLfloat)horizontal/(GLfloat)vertical;
	glMatrixMode(GL_PROJECTION);
    // Przełączenie macierzy bieżącej na macierz projekcji
    glLoadIdentity();
    // Czyszcznie macierzy bieżącej
    gluPerspective(70, AspectRatio, 1.0, 300.0);
    // Ustawienie parametrów dla rzutu perspektywicznego
    if(horizontal <= vertical)
        glViewport(0, (vertical-horizontal)/2, horizontal, horizontal);
    else
        glViewport((horizontal-vertical)/2, 0, vertical, vertical);
    // Ustawienie wielkości okna okna widoku (viewport) w zależności
    // relacji pomiędzy wysokością i szerokością okna
    glMatrixMode(GL_MODELVIEW);
    // Przełączenie macierzy bieżącej na macierz widoku modelu  
    glLoadIdentity();
    // Czyszczenie macierzy bieżącej
}

int main(int argc, char **argv)
{          
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Studio yayo");

    glutDisplayFunc(RenderScene);
	glutKeyboardFunc(keys);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);


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
