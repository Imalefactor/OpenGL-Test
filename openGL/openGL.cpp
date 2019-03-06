#include <GL/glut.h> 
#include <stdio.h> 
 
#define LEFT_EDGE   1   
#define RIGHT_EDGE  2	 
#define BOTTOM_EDGE 4	 
#define TOP_EDGE    8	 
 
bool bInput, accept, bDraw; 
struct Rectangle 
{ 
	float xmin,xmax,ymin,ymax; 
}; 
Rectangle  rect; 
 
struct Point {	int x, y;}; 
Point pt[2]; 
 
void LineGL(Point pt0, Point pt1) 
{ 
	glBegin (GL_LINES); 
	glColor3f (1.0f, 0.0f, 0.0f);   glVertex2f (pt0.x,pt0.y); 
	glColor3f (0.0f, 1.0f, 0.0f);   glVertex2f (pt1.x,pt1.y);	 
	glEnd (); 
} 
 
void PointGL(Point pt) 
{ 
	glPointSize(2); 
	glBegin (GL_POINTS); 
	glColor3f (1.0f, 0.0f, 0.0f);   glVertex2f (pt.x,pt.y); 
	glEnd (); 
} 
 
int CompCode(Point pt,Rectangle rect) 
{ 
	int code=0x00; 
	if(pt.y<rect.ymin) 
		code=code|BOTTOM_EDGE; 
	if(pt.y>rect.ymax) 
		code=code|TOP_EDGE; 
	if(pt.x>rect.xmax) 
		code=code|RIGHT_EDGE; 
	if(pt.x<rect.xmin) 
		code=code|LEFT_EDGE; 
	return code; 
} 
 
void cohensutherlandlineclip(Rectangle  rect, Point &pt0, Point &pt1) 
{ 
	int &x0 = pt0.x; 
	int &y0 = pt0.y; 
	int &x1 = pt1.x; 
	int &y1 = pt1.y; 
 
	bool done = false; 
	float x,y; 
 
	int code0,code1, codeout; 
	code0 = CompCode(pt0,rect); 
	code1 = CompCode(pt1,rect); 
	do{ 
		if(!(code0 | code1)) 
		{ 
			accept=true; 
			done=true; 
		} 
		else if(code0 & code1) 
		{  
			done = true; 
		} 
		else 
		{    
			if(code0!=0) 
				codeout = code0; 
			else 
				codeout = code1; 
 
			if(codeout&LEFT_EDGE){ 
				y=y0+(y1-y0)*(rect.xmin-x0)/(x1-x0); 
				x=(float)rect.xmin; 
			} 
			else if(codeout&RIGHT_EDGE){ 
				y=y0+(y1-y0)*(rect.xmax-x0)/(x1-x0); 
				x=(float)rect.xmax; 
			} 
			else if(codeout&BOTTOM_EDGE){ 
				x=x0+(x1-x0)*(rect.ymin-y0)/(y1-y0); 
				y=(float)rect.ymin;  
			} 
			else if(codeout&TOP_EDGE){ 
				x=x0+(x1-x0)*(rect.ymax-y0)/(y1-y0); 
				y=(float)rect.ymax; 
			} 
 
			if(codeout == code0) 
			{  
				x0=x;y0=y; 
				code0 = CompCode(pt0,rect); 
			} 
			else 
			{ 
				x1=x;y1=y; 
				code1 = CompCode(pt1,rect); 
			} 
		} 
	}while(!done); 
} 
 
void myDisplay() 
{ 
	glClear(GL_COLOR_BUFFER_BIT); 
	glColor3f (1.0f, 0.0f, 0.0f);  
	glRectf(rect.xmin,rect.ymin,rect.xmax,rect.ymax); 
 
	if(accept)		 
	{ 
		if(bDraw) 
		{ 
			LineGL(pt[0],pt[1]); 
		} 
		else 
			PointGL(pt[0]); 
	} 
 
	glFlush(); 
} 
 
void Init() 
{ 
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glShadeModel(GL_FLAT); 
 
	rect.xmin=100; 
	rect.xmax=300;  
	rect.ymin=100;  
	rect.ymax=300; 
 
	accept = true; 
	bInput = false; 
//	printf("Press Left Mouse Button to Draw two Point to create a line!\nPress key 'c' to Clip!\n"); 
} 
 
void Reshape(int w, int h) 
{ 
	glViewport(0, 0, (GLsizei) w,  (GLsizei) h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h); 
} 
 
void keyboard(unsigned char key, int x, int y) 
{ 
	switch (key) 
	{ 
	case 'c': 
		cohensutherlandlineclip(rect, pt[0],pt[1]);	 
		glutPostRedisplay(); 
		break; 
 
	default: 
		break; 
	} 
} 
 
void mouse(int button, int state, int x, int y) 
{ 
	switch (button) 
	{ 
	case GLUT_LEFT_BUTTON: 
		if (state == GLUT_DOWN)  
		{ 
			if (bInput == false) 
			{ 
				pt[0].x = x; 
				pt[0].y = 480 - y; 
				bInput = true; 
 
				bDraw = false; 
				accept = true; 
				glutPostRedisplay();// 
			} 
			else 
			{ 
				pt[1].x = x; 
				pt[1].y = 480 - y; 
				bDraw = true; 
				bInput = false; 
				glutPostRedisplay();// 
			}	 
		} 
		break; 
	default: 
		break; 
	} 
} 
 
int main(int argc, char *argv[]) 
{ 
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); 
	glutInitWindowPosition(100, 100); 
	glutInitWindowSize(640, 480); 
	glutCreateWindow("Cohen-Sutherland ²Ã¼ôÖ±Ïß"); 
 
	Init(); 
	glutDisplayFunc(myDisplay); 
	glutReshapeFunc(Reshape); 
	glutKeyboardFunc(keyboard); 
	glutMouseFunc(mouse); 
	glutMainLoop(); 
	return 0; 
}
