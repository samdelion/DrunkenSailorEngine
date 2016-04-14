#include "Dependencies\FreeGlut\include\Gl\glut.h"
#include "lighting.h"
#include "Water.h"
#include <iostream>

Water * water;
Lighting * light1;

void myinit(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);//sets the background to black
    glEnable(GL_DEPTH_TEST|GL_BLEND);//enables use of transparency (alpha)
	
	glPushMatrix();
		light1 -> startLight(0, 1, 1, 15, 100, 15, -25);
	glPopMatrix();
}

void reshape(int w, int h)
{ 
	if(h == 0) //if the height is 0
		h = 1;  //set to 1

    glMatrixMode (GL_PROJECTION);//sets matrix mode to projection
    glLoadIdentity ();
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);//sets the window view

	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1, 1000.0);

	gluLookAt(-5,10,10, 20, 0,-15, 0,1,0);//sets the initial camera position.
    glMatrixMode(GL_MODELVIEW);//sets the matrix mode back to modelview
	glLoadIdentity ();
}

void display()
{
	float slowFactor = 0.2;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

	glPushMatrix();		
	glTranslatef(3, 3, 0);

	water -> DiamondSquare(1, 1,  water -> GetSizeX(), water -> GetSizeZ(), 3500, (2^10000));
	//glColor4f(1, 1, 1, 0.8);
	//for(float i = 0; i < slowFactor; i+=.01)
		for(int z = 2; z < water -> GetSizeZ() - 1; z++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for(int x = 2; x < water -> GetSizeX(); x++)
			{
				float height = water->GetValAt(x, z);

				glColor4f(0, height, 1, 0.1);
				glVertex3f(x, height, -z);

				height = water->GetValAt(x, z+1);

				glColor4f(0.02f, height, 1, 0.1);
				glVertex3f(x, height, -(z+1));
			}
			glEnd();
		}

	glLoadIdentity();
	glPopMatrix();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'w':
		case 'W':
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			display();
			break;
		case 'm':
		case 'M':
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			display();
			break;
		case 27:
			exit(0);
			break;
	}
}

int main(int argc, char** argv)
{	
	srand(time(NULL));

	light1 = new Lighting();
	water = new Water(513, 257, 0.0f);
	water -> DiamondSquare(1, 1,  water -> GetSizeX(), water -> GetSizeZ(), 3500, (2^1000000));
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000,700);
    glutInitWindowPosition(30,0);
    glutCreateWindow("Water!");
    myinit();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);		//press key function
	glutIdleFunc(display);

	glutMainLoop(); /*enter event loop*/

    return 0;
}