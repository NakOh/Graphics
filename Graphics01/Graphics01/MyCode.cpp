#include <GL\glut.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

GLfloat g_x = 0.f;
int g_timeDelta = 0, g_prevTime = 0;
GLfloat g_velocityX = 5.f;
GLfloat *vertices;
GLint *indices;

void changeSize(GLsizei w, GLsizei h);
void SetupRC(void);
int readData();
void RenderScene();
void Idle();
void parsingFloatData(string line,int count, int *vertexCount, int *indiceCount);

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
	string temp;

	int index;
	int secondIndex;
	int info;
	int count = -1;
	int vertexCount = 0;
	int indiceCount = 0;

	ifstream myfile("models/cube.dat");

	if (myfile.is_open())
	{			
		while (getline(myfile, line)) //dat파일을 한줄 씩 읽는다.
		{					
			//cout << line << '\n';
			if (line.find("VERTEX") != string::npos) { //만약 VERTEX가 있다면
				index = line.find("="); //일단 = 를 찾는다.				
				info = std::stoi(line.substr(index + 2, 4)); //찾은 인덱스부터 2번째 뒤부터 맨 끝까지의 string값을 int로 바꾼다.				
				vertices = new GLfloat[info*3]; // 해당 int만큼 배열 동적 생성
				count = 0;
			} 
			else if (line.find("FACE") != string::npos) { //만약 FACE가 있다면
				index = line.find("="); //일단 = 를 찾는다.				
				info = std::stoi(line.substr(index + 2, 4)); //찾은 인덱스부터 2번째 뒤부터 맨 끝까지의 string값을 int로 바꾼다.				
				indices = new GLint[info*3]; // 해당 int만큼 배열 동적 생성
				count = 1;
			}
			else { //그냥 숫자 일때				
				parsingFloatData(line, count, &vertexCount, &indiceCount);				
			}
		}
		myfile.close();
	}

	else cout << "Unable to open file";

	return 0;

}

void parsingFloatData(string line,int count, int *vertexCount, int *indiceCount) {
	int index;	
	string temp;

	if (count == 0) {
		//VERTEX를 만난 후
		index = line.find(" "); //띄어쓰기를 찾는다. 
		temp = line.substr(0, index); //처음부터 그 인덱스까지 한 숫자
		vertices[*vertexCount] = std::stof(temp);  //숫자 하나 찾고 vertices에 집어 넣는다.
		(*vertexCount)++;
		temp = line.substr(index, 100);//띄어쓰기 이후로 단어를 임시 저장
	}
	else if (count == 1) {
		//FACE를 만난 후
		index = line.find(" "); //띄어쓰기를 찾는다. 
		temp = line.substr(0, index); //처음부터 그 인덱스까지 한 숫자
		vertices[*indiceCount] = std::stoi(temp);  //숫자 하나 찾고 vertices에 집어 넣는다.
		(*indiceCount)++;
		temp = line.substr(index, 100);//띄어쓰기 이후로 단어를 임시 저장
	}
	else {
		//VERTEX나 FACE를 못 만났을때
		cout << "파일이 잘못되었습니다";
		
	}
	
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


