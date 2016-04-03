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
	readData(); //일단 dat 파일을 읽어옵니다.
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
		while (getline(myfile, line)) //dat파일을 한줄 씩 읽는다.
		{					
			//cout << line << '\n';
			if (line.find("VERTEX") != string::npos) { //만약 VERTEX가 있다면
				index = line.find("="); //일단 = 를 찾는다.				
				info = std::stoi(line.substr(index + 2, 4)); //찾은 인덱스부터 2번째 뒤부터 맨 끝까지의 string값을 int로 바꾼다.				
				vertices = new GLfloat(info); // 해당 int만큼 배열 동적 생성
			} 
			else if (line.find("FACE") != string::npos) { //만약 FACE가 있다면
				index = line.find("="); //일단 = 를 찾는다.				
				info = std::stoi(line.substr(index + 2, 4)); //찾은 인덱스부터 2번째 뒤부터 맨 끝까지의 string값을 int로 바꾼다.				
				indices = new GLubyte(info); // 해당 int만큼 배열 동적 생성
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


