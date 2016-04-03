#include <GL\glut.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

GLfloat g_x = 0.f;
int g_timeDelta = 0, g_prevTime = 0;
GLfloat g_velocityX = 5.f;
GLfloat *vertices;
GLubyte *indices;

void changeSize(GLsizei w, GLsizei h);
void SetupRC(void);
int readData();
void RenderScene();
void Idle();

int	main(int argc, char* argv[])
{
	readData(); //�ϴ� dat ������ �о�ɴϴ�.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("HomeWork");
	// callback function
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(Idle);
	SetupRC();		// initialize render context(RC)
	glutMainLoop();		// run GLUT framework
	return 0;	

}

int readData() {

	string line;
	int index;
	int info;

	ifstream myfile("models/Chair.dat");

	if (myfile.is_open())
	{			
		while (getline(myfile, line)) //dat������ ���� �� �д´�.
		{					
			//cout << line << '\n';
			if (line.find("VERTEX") != string::npos) { //���� VERTEX�� �ִٸ�
				index = line.find("="); //�ϴ� = �� ã�´�.				
				info = std::stoi(line.substr(index + 2, 4)); //ã�� �ε������� 2��° �ں��� �� �������� string���� int�� �ٲ۴�.				
				vertices = new GLfloat(info); // �ش� int��ŭ �迭 ���� ����
			} 
			else if (line.find("FACE") != string::npos) { //���� FACE�� �ִٸ�
				index = line.find("="); //�ϴ� = �� ã�´�.				
				info = std::stoi(line.substr(index + 2, 4)); //ã�� �ε������� 2��° �ں��� �� �������� string���� int�� �ٲ۴�.				
				indices = new GLubyte(info); // �ش� int��ŭ �迭 ���� ����
			}
		}
		myfile.close();
	}

	else cout << "Unable to open file";

	return 0;

}

void Idle() {
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	g_timeDelta = currentTime - g_prevTime;
	g_prevTime = currentTime;
	glutPostRedisplay();
}

void SetupRC(void)
{
	// setup clear color
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void RenderScene() {

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(0.f, 0.f, -80.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	glPushMatrix();
	glTranslatef(25.f, -30.f, -50.f);
	glScaled(2.f, 2.f, 1.f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(-10.f, 0.f, 0.f);
	glVertex3f(10.f, 0.f, 0.f);
	glVertex3f(0.f, 20.f, 0.f);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-25.f, -30.f, 0.f);
	glScaled(2.f, 2.f, 1.f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(10.f, 0.f, 0.f);
	glVertex3f(-10.f, 0.f, 0.f);
	glVertex3f(0.f, 20.f, 0.f);
	glEnd();
	glPopMatrix();
	glutSwapBuffers();
}

void changeSize(GLsizei w, GLsizei h) {
	GLfloat aspectRatio;
	if (h == 0)
		h = 1;
	aspectRatio = (GLfloat)w / (GLfloat)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, aspectRatio, 10.f, 200.f);
	//glOrtho(-100.f, 100.f, -100.f, 100.f, -100.f, 100.f);
}


