#include <GL\glut.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

GLfloat g_x = 0.f;
int g_timeDelta = 0, g_prevTime = 0;
GLfloat g_velocityX = 5.f;
GLfloat *vertices;

GLfloat centerX;
GLfloat centerY;
GLfloat centerZ;

GLint *indices;
int info;
int vertex;

void changeSize(GLsizei w, GLsizei h);
void SetupRC(void);
int readData();
void RenderScene();
void Idle();
void update();
void parsingData(string line,int count, int *vertexCount, int *indiceCount);

int	main(int argc, char* argv[])
{	
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
	glutCreateWindow("HomeWork");
	readData(); //일단 dat 파일을 읽어옵니다.
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
				vertex = std::stoi(line.substr(index + 2, 4)); //찾은 인덱스부터 2번째 뒤부터 맨 끝까지의 string값을 int로 바꾼다.				
				vertices = new GLfloat[vertex*3]; // 해당 int만큼 배열 동적 생성
				count = 0;
			} 
			else if (line.find("FACE") != string::npos) { //만약 FACE가 있다면
				index = line.find("="); //일단 = 를 찾는다.				
				info = std::stoi(line.substr(index + 2, 4)); //찾은 인덱스부터 2번째 뒤부터 맨 끝까지의 string값을 int로 바꾼다.				
				indices = new GLint[info*3]; // 해당 int만큼 배열 동적 생성
				count = 1;
			}
			else { //그냥 숫자 일때				
				parsingData(line, count, &vertexCount, &indiceCount);					
			}
		}
		
		myfile.close();
	}

	else cout << "Unable to open file";

	return 0;

}

void parsingData(string line,int count, int *vertexCount, int *indiceCount) {
	int index;	
	string temp;

	if (count == 0) {
		//VERTEX를 만난 후
		index = line.find(" "); //띄어쓰기를 찾는다. 
		temp = line.substr(0, index); //처음부터 그 인덱스까지 한 숫자
		vertices[*vertexCount] = std::stof(temp)/400;  //숫자 하나 찾고 vertices에 집어 넣는다.	
		centerX += stof(temp);
		(*vertexCount)++; //한 줄의 첫번째 변수 주입 완료	

		line = line.substr(index+1, 100);//띄어쓰기 이후로 단어를 저장
		index = line.find(" ");
		temp = line.substr(0, index);
		vertices[*vertexCount] = std::stof(temp)/400;  //숫자 하나 찾고 vertices에 집어 넣는다.		
		centerY += stof(temp);
		(*vertexCount)++; //한 줄의 두 번째 변수 주입 완료
		
		line = line.substr(index, 100);

		temp = line.substr(1, 100); //두 번째 변수 이후로 임시 저장
		vertices[*vertexCount] = std::stof(temp)/400;  //숫자 하나 찾고 vertices에 집어 넣는다.
		centerZ += stof(temp);		
		(*vertexCount)++; //한 줄의 두 번째 변수 주입 완료	
		
	}
	else if (count == 1) {
		//FACE를 만난 후
		index = line.find(" "); //띄어쓰기를 찾는다. 
		temp = line.substr(0, index); //처음부터 그 인덱스까지 한 숫자
		indices[*indiceCount] = std::stoi(temp);  //숫자 하나 찾고 vertices에 집어 넣는다.		
		(*indiceCount)++; //한 줄의 첫번째 변수 주입 완료		
		
		line = line.substr(index + 1, 100);//띄어쓰기 이후로 단어를 저장
		index = line.find(" ");
		temp = line.substr(0, index);
		indices[*indiceCount] = std::stoi(temp);  //숫자 하나 찾고 vertices에 집어 넣는다.				
		(*indiceCount)++; //한 줄의 두 번째 변수 주입 완료

		line = line.substr(index, 100);
		temp = line.substr(1, 3); //두 번째 변수 이후로 임시 저장
		indices[*indiceCount] = std::stoi(temp);  //숫자 하나 찾고 vertices에 집어 넣는다.						
		(*indiceCount)++; //한 줄의 세 번째 변수 주입 완료
		
	}
	else {
		//VERTEX나 FACE를 못 만났을때
		cout << "파일이 잘못되었습니다";		
	}
	
}


void Idle() {
	glutPostRedisplay();	
}

void SetupRC(void)
{
	// setup clear color
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void RenderScene() {
	static GLfloat rot = 1.0f;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.f, 1.f, 1.f);
	glMatrixMode(GL_MODELVIEW);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glLoadIdentity();	
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	update();		
	glutSwapBuffers();		
}

void update() {
	static GLfloat rot = 0.0f;
	static GLfloat scale = 1.0f;
	static GLfloat x = 0.0001f;
	static GLfloat center_x = centerX / (vertex *  400);
	static GLfloat center_y = centerY / (vertex * 400);
	static GLfloat center_z = centerZ / (vertex * 400);

	glEnableClientState(GL_VERTEX_ARRAY);

	//자신 중심 회전
	glPopMatrix();
	glTranslatef(-0.7f, 0, 0);
	glTranslatef(center_x , center_y, center_z);
	glRotatef(rot, 0.0, 0.0, 1.0);
	glTranslatef(-(center_x) , -center_y, -center_z);	
	glDrawElements(GL_TRIANGLES, info * 3, GL_UNSIGNED_INT, indices);
	
	rot -= 0.005f;	

	//크기 커짐 작아짐	
	glPopMatrix();
	glTranslatef(0.5f, 0.5f, 0.f);
	glScalef(scale, scale, scale);
	if (scale > 1.5f){
		x = -x;
	}
	else  if (scale < 0){
		x = -x;
	}	
	scale += x;
	glDrawElements(GL_TRIANGLES, info * 3, GL_UNSIGNED_INT, indices);	

	//다른 점 중심 회전	
	glPopMatrix();
	glTranslatef(center_x , center_y - 0.5f, center_z);
	glRotatef(rot, 0.0, 0.0, 1.0);
	glTranslatef(-center_x, -(center_y - 0.5f) , -center_z);
	glDrawElements(GL_TRIANGLES, info * 3, GL_UNSIGNED_INT, indices);	

	glDisableClientState(GL_VERTEX_ARRAY);
}

void changeSize(GLsizei w, GLsizei h) {
	
	GLfloat aspectRatio;
	if (h == 0)
		h = 1;
	aspectRatio = (GLfloat)w / (GLfloat)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60.f, aspectRatio, 0.f, 100.f);
	//glOrtho(-100.f, 100.f, -100.f, 100.f, -100.f, 100.f);
	
}


