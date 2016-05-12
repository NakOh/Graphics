#include <GL\glew.h>
#include <GL\glut.h>
#include <fstream>
#include <iostream>
#include <string>
#include "shader.hpp"

using namespace std;



int ProgramID; // Shader Program ID;
int modelMatrixID; // Shader ModelMatrix ID;
int viewMatrixID; // Shader ViewMatrix ID;
int projectionMatrixID; // Shader ProjectionMatrix ID;
int vertexPositionID; // Shader vertexPosition ID;
int vertexColorID; // Shader vectexColor ID;


GLfloat g_x = 0.f;
int g_timeDelta = 0, g_prevTime = 0;
GLfloat g_velocityX = 5.f;

GLfloat *vertices;
GLfloat *newVertices;
GLfloat *colordata;

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
void parsingData(string line, int count, int *vertexCount, int *indiceCount);
void checkError();

void main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	
	glutCreateWindow("HomeWork");

	SetupRC();		// initialize render context(RC)
	readData(); //일단 dat 파일을 읽어옵니다.
				
	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutReshapeFunc(changeSize);
	
	glutMainLoop();		// run GLUT framework
}


void checkError() {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		printf("Error. glError 0x%04X\n", err);
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
				vertices = new GLfloat[vertex * 3]; // 해당 int만큼 배열 동적 생성
				colordata = new GLfloat[vertex * 4]; // 컬러 배열 선언
				newVertices = new GLfloat[vertex * 4]; //호모지니어스 좌표계로 바꾸기 위한 배열 선언
				count = 0;
			}
			else if (line.find("FACE") != string::npos) { //만약 FACE가 있다면
				index = line.find("="); //일단 = 를 찾는다.				
				info = std::stoi(line.substr(index + 2, 4)); //찾은 인덱스부터 2번째 뒤부터 맨 끝까지의 string값을 int로 바꾼다.				
				indices = new GLint[info * 3]; // 해당 int만큼 배열 동적 생성
				count = 1;
			}
			else { //그냥 숫자 일때				
				parsingData(line, count, &vertexCount, &indiceCount);
			}
		}

		int i, j = 0;

		for (i = 0; i < vertex * 4; i++){
			if (i%4 == 3) {
				newVertices[i] = 1;							

				cout << "1 주입" <<endl;
			}
			else {
				newVertices[i] = vertices[j];
				j++;			

				cout << newVertices[i];
			}		
			colordata[i] = 1.0f;
		}

		myfile.close();
	}

	else cout << "Unable to open file";

	return 0;

}

void parsingData(string line, int count, int *vertexCount, int *indiceCount) {
	int index;
	string temp;

	if (count == 0) {
		//VERTEX를 만난 후
		index = line.find(" "); //띄어쓰기를 찾는다. 
		temp = line.substr(0, index); //처음부터 그 인덱스까지 한 숫자
		vertices[*vertexCount] = std::stof(temp) / 100;  //숫자 하나 찾고 vertices에 집어 넣는다.	
		
		centerX += stof(temp);
		(*vertexCount)++; //한 줄의 첫번째 변수 주입 완료	

		line = line.substr(index + 1, 100);//띄어쓰기 이후로 단어를 저장
		index = line.find(" ");
		temp = line.substr(0, index);
		vertices[*vertexCount] = std::stof(temp) / 100;  //숫자 하나 찾고 vertices에 집어 넣는다.		
		centerY += stof(temp);
		(*vertexCount)++; //한 줄의 두 번째 변수 주입 완료

		line = line.substr(index, 100);

		temp = line.substr(1, 100); //두 번째 변수 이후로 임시 저장
		vertices[*vertexCount] = std::stof(temp) / 100;  //숫자 하나 찾고 vertices에 집어 넣는다.
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
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	g_timeDelta = currentTime - g_prevTime;
	g_prevTime = currentTime;
	glutPostRedisplay();
}

void SetupRC(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	int glewtest = glewInit();
	if (glewtest != GLEW_OK) {
		printf("오류");
	}

	ProgramID = shader::LoadShaders("VertexShader.vert", "FragmentShader.frag");

	modelMatrixID = glGetUniformLocation(ProgramID, "modelMatrix");
	viewMatrixID = glGetUniformLocation(ProgramID, "viewMatrix");
	projectionMatrixID = glGetUniformLocation(ProgramID, "projectionMatrix");

	vertexPositionID = glGetAttribLocation(ProgramID, "vertexPosition");
	vertexColorID = glGetAttribLocation(ProgramID, "vertexColor");
}

void RenderScene() {

	g_x += g_velocityX * g_timeDelta / 1000.f;
	if (g_x > 10) g_velocityX *= -1;
	if (g_x < -10) g_velocityX *= -1;


	GLfloat modelMatrix[16] = { 1,0,0,0 ,0,1,0,0 , 0,0,1,0 , 0,0,0,1 };
	GLfloat viewMatrix[16] = { 1,0,0,0 , 0,1,0,0 , 0,0,1,0 , 0,0,0,1 };
	GLfloat projMatrix[16] = { 0.1f,0,0,0 , 0,0.1f,0,0 , 0,0,-1,0 , 0,0,0,1 };

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(ProgramID);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0]);
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projMatrix[0]);

	glEnableVertexAttribArray(vertexPositionID);
	glEnableVertexAttribArray(vertexColorID);

	glVertexAttribPointer(
		vertexPositionID,				// The attribute we want to configure
		4,							    // size
		GL_FLOAT,						// type
		GL_FALSE,						// normalized?
		0,								// stride
		(void*)&newVertices[0]			// array buffer offset
		);

	glVertexAttribPointer(
		vertexColorID,				// The attribute we want to configure
		4,							    // size
		GL_FLOAT,						// type
		GL_FALSE,						// normalized?
		0,								// stride
		(void*)&colordata[0]			// array buffer offset
		);

	glDrawArrays(GL_TRIANGLES, 0, vertex);

	glDisableVertexAttribArray(vertexPositionID);
	glDisableVertexAttribArray(vertexColorID);

	

	glUseProgram(0);
	glutSwapBuffers();

}

void update() {

}

void changeSize(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
}


