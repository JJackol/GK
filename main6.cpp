#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glut.h>

GLbyte *pBytes1;
GLint ImWidth1, ImHeight1, imComponents1;
GLenum ImFormat1;

GLbyte *pBytes2;
GLint ImWidth2, ImHeight2, ImComponents2;
GLenum ImFormat2;

// Definicja materiału z jakiego zrobiony jest czajnik
GLfloat mat_ambient[]  = {1.0, 1.0, 1.0, 1.0};       
// współczynniki ka =[kar,kag,kab] dla światła otoczenia
GLfloat mat_diffuse[]  = {1.0, 1.0, 1.0, 1.0};
// współczynniki kd =[kdr,kdg,kdb] światła rozproszonego
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
// współczynniki ks =[ksr,ksg,ksb] dla światła odbitego               
GLfloat mat_shininess  = {20.0};
// współczynnik n opisujący połysk powierzchni

// Definicja źródła światła
GLfloat light_position[] = {-5.0, 1.0, 9.0, 1.0};   
GLfloat light_ambient[] = {0.02, 0.0, 0.0, 1.0};
GLfloat light_diffuse[] = {0.2, 0.3, 0.2, 1.0};       
GLfloat light_specular[]= {1.0, 1.0, 1.0, 1.0};
GLfloat att_constant  = {1.0};
GLfloat att_linear    = {0.05};
GLfloat att_quadratic  = {0.001};

const float N = 128;
static GLfloat theta[] = {0.0, 0.0, 0.0}; // trzy kąty obrotu
static GLfloat spin[] = {0.0, 0.0, 0.0}; // trzy kąty obrotu
static GLfloat fi[] = {0.0, 0.0, 0.0}; // trzy kąty obrotu
static GLfloat up[] = {0.0, 1.0, 0.0}; // trzy kąty obrotu
static GLfloat viewer[] = {0.0, 0.0, 15.0};
static int T1 = 1;
static int T2 = 1;
static int model = 4;
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

/*************************************************************************************/
 // Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie
 // FileName, alokuje pamięć i zwraca wskaźnik (pBits) do bufora w którym
 // umieszczone są dane.
 // Ponadto udostępnia szerokość (ImWidth), wysokość (ImHeight) obrazu
 // tekstury oraz dane opisujące format obrazu według specyfikacji OpenGL
 // (ImComponents) i (ImFormat).
 // Jest to bardzo uproszczona wersja funkcji wczytującej dane z pliku TGA.
 // Działa tylko dla obrazów wykorzystujących 8, 24, or 32 bitowy kolor.
 // Nie obsługuje plików w formacie TGA kodowanych z kompresją RLE.
/*************************************************************************************/
GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{
// Struktura dla nagłówka pliku  TGA
    #pragma pack(1)           
    typedef struct                       
    {
        GLbyte    idlength;             
        GLbyte    colormaptype;          
        GLbyte    datatypecode;            
        unsigned short    colormapstart; 
        unsigned short    colormaplength;
        unsigned char     colormapdepth;  
        unsigned short    x_orgin;        
        unsigned short    y_orgin;        
        unsigned short    width;         
        unsigned short    height;        
        GLbyte    bitsperpixel;                  
        GLbyte    descriptor;            
    }TGAHEADER;
    #pragma pack(8)

    FILE *pFile;                   
    TGAHEADER tgaHeader;           
    unsigned long lImageSize;       
    short sDepth;                   
    GLbyte    *pbitsperpixel = NULL; 
/*************************************************************************************/ 
// Wartości domyślne zwracane w przypadku błędu
    *ImWidth = 0;               
    *ImHeight = 0;
    *ImFormat = GL_BGR_EXT;
    *ImComponents = GL_RGB8;
   
    pFile = fopen(FileName, "rb");
    if(pFile == NULL)
      return NULL;
	/*************************************************************************************/
	// Przeczytanie nagłówka pliku 
    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);
	/*************************************************************************************/

	// Odczytanie szerokości, wysokości i głębi obrazu
    *ImWidth = tgaHeader.width;
    *ImHeight = tgaHeader.height;
    sDepth = tgaHeader.bitsperpixel / 8;
	/*************************************************************************************/
	// Sprawdzenie, czy głębia spełnia założone warunki (8, 24, lub 32 bity)
    if(tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
        return NULL;

	// Obliczenie rozmiaru bufora w pamięci
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
/*************************************************************************************/   
// Alokacja pamięci dla danych obrazu
	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
		if(pbitsperpixel == NULL)
        return NULL;

    if(fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
        {
        free(pbitsperpixel);
        return NULL;
        }
/*************************************************************************************/
// Ustawienie formatu OpenGL
    switch(sDepth)
        {
        case 3:    
            *ImFormat = GL_BGR_EXT;
            *ImComponents = GL_RGB8;
            break;
        case 4:
            *ImFormat = GL_BGRA_EXT;
            *ImComponents = GL_RGBA8;
            break;
        case 1:
            *ImFormat = GL_LUMINANCE;
            *ImComponents = GL_LUMINANCE8;
            break;
        };
    fclose(pFile);
    return pbitsperpixel;
}
/*************************************************************************************/ 


void spinLight()
{
    spin[0] -= 0.5;
    if( spin[0] > 360.0 ) spin[0] -= 360.0;
    spin[1] -= 0.5;
    if( spin[1] > 360.0 ) spin[1] -= 360.0;
    spin[2] -= 0.5;
    if( spin[2] > 360.0 ) spin[2] -= 360.0;

	light_specular[0] = (cos(spin[1]*M_PI/180) + 1) / 2;
	light_specular[1] = (sin(spin[1]*M_PI/180) + 1) / 2;
    glutPostRedisplay(); //odświeżenie zawartości aktualnego okna
}

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

void dirt(float a)
{
    point3  Avert = {-a, -a, a};
    point3  Bvert = {a, -a, a};
    point3  Cvert = {a, a, a};
    point3  Dvert = {-a, a, a};

    point3  Evert = {-a, -a, -a};
    point3  Fvert = {a, -a, -a};
    point3  Gvert = {a, a, -a};
    point3  Hvert = {-a, a, -a};

	
	
	glBegin(GL_TRIANGLES);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(Avert);
			glTexCoord2f(0.5f, 0.0f);
			glVertex3fv(Bvert);
			glTexCoord2f(0.0f, 0.5f);
			glVertex3fv(Dvert);

			glTexCoord2f(0.5f, 0.0f);
			glVertex3fv(Bvert);
			glTexCoord2f(0.5f, 0.5f);
			glVertex3fv(Cvert);
			glTexCoord2f(0.0f, 0.5f);
			glVertex3fv(Dvert);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(Bvert);
			glTexCoord2f(0.5f, 0.0f);
			glVertex3fv(Fvert);
			glTexCoord2f(0.0f, 0.5f);
			glVertex3fv(Cvert);

			glTexCoord2f(0.5f, 0.0f);
			glVertex3fv(Fvert);
			glTexCoord2f(0.5f, 0.5f);
			glVertex3fv(Gvert);
			glTexCoord2f(0.0f, 0.5f);
			glVertex3fv(Cvert);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(Fvert);
			glTexCoord2f(0.5f, 0.0f);
			glVertex3fv(Evert);
			glTexCoord2f(0.0f, 0.5f);
			glVertex3fv(Gvert);

			glTexCoord2f(0.5f, 0.0f);
			glVertex3fv(Evert);
			glTexCoord2f(0.5f, 0.5f);
			glVertex3fv(Hvert);
			glTexCoord2f(0.0f, 0.5f);
			glVertex3fv(Gvert);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(Evert);
			glTexCoord2f(0.5f, 0.0f);
			glVertex3fv(Avert);
			glTexCoord2f(0.0f, 0.5f);
			glVertex3fv(Hvert);

			glTexCoord2f(0.5f, 0.0f);
			glVertex3fv(Avert);
			glTexCoord2f(0.5f, 0.5f);
			glVertex3fv(Dvert);
			glTexCoord2f(0.0f, 0.5f);
			glVertex3fv(Hvert);

			//top
			glTexCoord2f(0.0f, 0.5f);
			glVertex3fv(Dvert);
			glTexCoord2f(0.5f, 0.5f);
			glVertex3fv(Cvert);
			glTexCoord2f(0.5f, 1.0f);
			glVertex3fv(Gvert);

			glTexCoord2f(0.0f, 0.5f);
			glVertex3fv(Dvert);
			glTexCoord2f(0.5f, 1.0f);
			glVertex3fv(Gvert);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3fv(Hvert);

			//bot
			glTexCoord2f(0.5f, 0.0f);
			glVertex3fv(Evert);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3fv(Fvert);
			glTexCoord2f(1.0f, 0.5f);
			glVertex3fv(Bvert);

			glTexCoord2f(0.5f, 0.1f);
			glVertex3fv(Evert);
			glTexCoord2f(1.0f, 0.5f);
			glVertex3fv(Bvert);
			glTexCoord2f(0.5f, 0.5f);
			glVertex3fv(Avert);



	glEnd();
}

float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

void jajkoTri()
{
	float u, v;
	float x, y, z;
	float X, Y, Z;
	float xu, yu, zu, xv, yv, zv;
	int i, j;
	float r,g,b;
	float inv;
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

			xu = -45 * (1 - 8* u + 4* u*u*u + 10 *u*u*u*u)* cos(v * M_PI);
			xv = M_PI * 45 * u * (1 - 4 *u + u*u*u + 2 *u*u*u*u) * sin(v * M_PI );
			yu = 320 * u* (1 - 3* u + 2 *u*u);
			yv = 0.0;
			zu = -45 * (1 - 8* u + 4* u*u*u + 10 *u*u*u*u)* sin(v * M_PI);
			zv = M_PI * -45 * u * (1 - 4 *u + u*u*u + 2 *u*u*u*u) * cos(v * M_PI );
			X = yu*zv - zu*yv;
			Y = zu*xv - xu*zv;
			Z = xu*yv - yu*xv;
		//	inv = Q_rsqrt((yu*zv - zu*yv) * (yu*zv - zu*yv) +(zu*xv - xu*zv) * (zu*xv - xu*zv) + (xu*yv - yu*xv) * (xu*yv - yu*xv));
			inv = Q_rsqrt(X * X + Y * Y + Z * Z);
			glNormal3f(X*inv, Y*inv, Z*inv);

			glTexCoord2f(u, v);
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
			glTexCoord2f(u, v);
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
		theta[1] += delta_y*pix2angle_y;    // modyfikacja kąta obrotu o kat proporcjonalnyego naego naego na
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
		
	//	float phi = fmod(delta_y, M_PI*2);
	//	if(phi > M_PI/2 && phi < M_PI*3/2)
	//	{
	//		up[1] = -1;
	//	}	
	//	else
	//		up[1] = 1;
	}                                  
	viewer[0] = zoom * Radi * cos(fi[0]) * cos(fi[1]);
	viewer[1] = zoom * Radi * sin(fi[1]);
	viewer[2] = zoom * Radi * sin(fi[0]) * cos(fi[1]);
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
		case 0:
			glBegin(GL_TRIANGLES);
					glTexCoord2f(0.0f, 0.0f);
					glVertex3f(-3.0, -2.0, 0.0);

					glTexCoord2f(1.0f, 0.0f);
					glVertex3f(3.0, -2.0, 0.0);

					glTexCoord2f(0.5f, 1.0f);
					glVertex3f(0.0, 3.0, 0.0);
			glEnd();
			break;
		case 1:
			jajko();
			break;
		case 2:
			jajkoWire();
			break;
		case 3:
			jajkoTri();
			break;
		case 4:
			dirt(3.0);
			break;
		default:
			glutSolidTeapot(3.0);
	}

	glColor3fv(light_specular);
	//glutSolidTetrahedron();

    glBegin(GL_TRIANGLE_FAN); 
        glVertex3fv(light_position);
        glVertex3f(light_position[0]+0.3,light_position[1]+0.3,light_position[2]+0.3 );
        glVertex3f(light_position[0]-0.3,light_position[1],light_position[2] );
        glVertex3f(light_position[0]-0.0,light_position[1]-0.3,light_position[2] );
    glEnd();

	//dirt(5.0);

    glFlush();
    glutSwapBuffers();

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
/*************************************************************************************/
// Ustawienie parametrów źródła
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);
} // koniec renderScene

void keys(unsigned char key, int x, int y)
{
	if(key == 'a') light_ambient[1]+=0.1;
	if(key == 'z') light_ambient[1]-=0.1;
	if(key == '=') T1++;
	if(key == '-') T1--;
	if(key == '0') T2++;
	if(key == '9') T2--;
	if(key == '1') pseu_zoom += 0.1;
	if(key == '2') pseu_zoom -= 0.1;

	if(key == '3') zoom += 0.1;
	if(key == '4') zoom -= 0.1;

    if(key == 'q') model = 1;//points
    if(key == 'w')
	{
		model = 2;//wire
		glTexImage2D(GL_TEXTURE_2D, 0, ImComponents2, ImWidth2, ImHeight2, 0, ImFormat2, GL_UNSIGNED_BYTE, pBytes2);
	}
    if(key == 'e')
	{
		model = 3;//strip
		glTexImage2D(GL_TEXTURE_2D, 0, ImComponents2, ImWidth2, ImHeight2, 0, ImFormat2, GL_UNSIGNED_BYTE, pBytes2);
	}

    if(key == 't') model = 4;//teapot
    if(key == 'r') rel = !rel;//teapot
    if(key == 'y') model = 0; // texture triangle
    if(key == 'm')
	{
		model = 4;
		glTexImage2D(GL_TEXTURE_2D, 0, ImComponents1, ImWidth1, ImHeight1, 0, ImFormat1, GL_UNSIGNED_BYTE, pBytes1);
	}
 // texture triangle
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

	/*************************************************************************************/
	// Zmienne dla obrazu tekstury
	// Teksturowanie będzie prowadzone tyko po jednej stronie ściany
	glEnable(GL_CULL_FACE);
	//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga
	pBytes1 = LoadTGAImage("dirtBox.tga", &ImWidth1, &ImHeight1, &ImComponents1, &ImFormat1);
	pBytes2 = LoadTGAImage("pilkatex.tga", &ImWidth2, &ImHeight2, &ImComponents2, &ImFormat2);
	// Zdefiniowanie tekstury 2-D
	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents1, ImWidth1, ImHeight1, 0, ImFormat1, GL_UNSIGNED_BYTE, pBytes1);
	// Zwolnienie pamięci
	/*************************************************************************************/
	// Włączenie mechanizmu teksturowania
	glEnable(GL_TEXTURE_2D);
	/*************************************************************************************/
	// Ustalenie trybu teksturowania
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Określenie sposobu nakładania tekstur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// ..................................
			//       Pozostała część funkcji MyInit()
			// ..................................

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	viewer[0] = zoom * Radi * cos(fi[0]) * cos(fi[1]);
	viewer[1] = zoom * Radi * sin(fi[1]);
	viewer[2] = zoom * Radi * sin(fi[0]) * cos(fi[1]);


// Ustawienie parametrów materiału i źródła światła
/*************************************************************************************/
// Ustawienie patrametrów materiału
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
/*************************************************************************************/
// Ustawienie parametrów źródła
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

/*************************************************************************************/
// Ustawienie opcji systemu oświetlania sceny
    glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
    glEnable(GL_LIGHTING);   // właczenie systemu oświetlenia sceny
    glEnable(GL_LIGHT0);     // włączenie źródła o numerze 0
    glEnable(GL_LIGHT1);     // włączenie źródła o numerze 0
    glEnable(GL_DEPTH_TEST); // włączenie mechanizmu z-bufora
	
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
        glViewport(0, (vertical-horizontal)/2, horizontal, vertical);
    else
        glViewport((horizontal-vertical)/2, 0, horizontal, vertical);
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

	glutIdleFunc(spinLight);
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
