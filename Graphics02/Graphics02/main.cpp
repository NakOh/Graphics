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
int i;

void changeSize(GLsizei w, GLsizei h);
void SetupRC(void);
int readData();
void RenderScene();
void Idle();
void update();
void parsingData(string line, int count, int *vertexCount, int *indiceCount);
void checkError();
void MyKeyboardFunc(unsigned char Key, int x, int y);

void main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	
	glutCreateWindow("HomeWork");

	SetupRC();		// initialize render context(RC)
	readData(); //�ϴ� dat ������ �о�ɴϴ�.
				
	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(MyKeyboardFunc);

	glutMainLoop();		// run GLUT framework
}

void MyKeyboardFunc(unsigned char Key, int x, int y) {
	switch (Key) { 
	case 'w': //ī�޶� Z �� ����
		break;
	case 's': //ī�޶� Z �� ����
		break;
	case 'a': //ī�޶� X �� ����
		break;
	case 'd': //ī�޶� X �� ����
		break;
	case 'r': //ī�޶� Y �� ����
		break;
	case 'f': //ī�޶� Y �� ����
		break;
	case 'q': //ī�޶� y�� ���� ȸ��
		break;
	case 'e' : //ī�޶� y�� ���� ȸ��
		break;
	case 'p': //Perspective Projection
		break;
	case 'o': // Orthographic Projection
		break;
	}
	glutPostRedisplay();
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
		while (getline(myfile, line)) //dat������ ���� �� �д´�.
		{
			//cout << line << '\n';
			if (line.find("VERTEX") != string::npos) { //���� VERTEX�� �ִٸ�
				index = line.find("="); //�ϴ� = �� ã�´�.				
				vertex = std::stoi(line.substr(index + 2, 4)); //ã�� �ε������� 2��° �ں��� �� �������� string���� int�� �ٲ۴�.				
				vertices = new GLfloat[vertex * 3]; // �ش� int��ŭ �迭 ���� ����				
				count = 0;
			}
			else if (line.find("FACE") != string::npos) { //���� FACE�� �ִٸ�
				index = line.find("="); //�ϴ� = �� ã�´�.				
				info = std::stoi(line.substr(index + 2, 4)); //ã�� �ε������� 2��° �ں��� �� �������� string���� int�� �ٲ۴�.				
				indices = new GLint[info * 3]; // �ش� int��ŭ �迭 ���� ����
				count = 1;
				newVertices = new GLfloat[info * 3 * 4]; //ȣ�����Ͼ ��ǥ��� �ٲٱ� ���� �迭 ����
				colordata = new GLfloat[info * 3 * 4]; // �÷� �迭 ����
			}
			else { //�׳� ���� �϶�				
				parsingData(line, count, &vertexCount, &indiceCount);
			}
		}

		int j = 0;
		int indexs = 0;

		for (j = 0; j < info * 3; j++){
			indexs = indices[j]; // FACE ������ �����ͼ� �ش� vertex�� ��ǥ ������ ������ �Է�����.	
			if (indexs == 0) {
				newVertices[i] = vertices[0];
				colordata[i] = 1.0f;			
				i= i+1;
				newVertices[i] = vertices[1];
				colordata[i] = 1.0f;
				i = i + 1;
				newVertices[i] = vertices[2];
				colordata[i] = 1.0f;
				i = i + 1;				
				newVertices[i] = 1;
				colordata[i] = 1.0f;
				i = i + 1;
			}else{
				newVertices[i] = vertices[3 * (indexs-1)];
				colordata[i] = 1.0f;
				i = i + 1;
				newVertices[i] = vertices[(3 * (indexs - 1)) + 1];
				colordata[i] = 1.0f;
				i = i + 1;
				newVertices[i] = vertices[(3 * (indexs - 1)) + 2];
				colordata[i] = 1.0f;
				i = i + 1;
				newVertices[i] = 1;
				colordata[i] = 1.0f;
				i = i + 1;
			}
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
		//VERTEX�� ���� ��
		index = line.find(" "); //���⸦ ã�´�. 
		temp = line.substr(0, index); //ó������ �� �ε������� �� ����
		vertices[*vertexCount] = std::stof(temp) * 0.05f;  //���� �ϳ� ã�� vertices�� ���� �ִ´�.	
		
		centerX += stof(temp);
		(*vertexCount)++; //�� ���� ù��° ���� ���� �Ϸ�	

		line = line.substr(index + 1, 100);//���� ���ķ� �ܾ ����
		index = line.find(" ");
		temp = line.substr(0, index);
		vertices[*vertexCount] = std::stof(temp) * 0.05f;  //���� �ϳ� ã�� vertices�� ���� �ִ´�.		
		centerY += stof(temp);
		(*vertexCount)++; //�� ���� �� ��° ���� ���� �Ϸ�

		line = line.substr(index, 100);

		temp = line.substr(1, 100); //�� ��° ���� ���ķ� �ӽ� ����
		vertices[*vertexCount] = std::stof(temp) * 0.05f;  //���� �ϳ� ã�� vertices�� ���� �ִ´�.
		centerZ += stof(temp);
		(*vertexCount)++; //�� ���� �� ��° ���� ���� �Ϸ�	

	}
	else if (count == 1) {
		//FACE�� ���� ��
		index = line.find(" "); //���⸦ ã�´�. 
		temp = line.substr(0, index); //ó������ �� �ε������� �� ����
		indices[*indiceCount] = std::stoi(temp);  //���� �ϳ� ã�� vertices�� ���� �ִ´�.		
		(*indiceCount)++; //�� ���� ù��° ���� ���� �Ϸ�		

		line = line.substr(index + 1, 100);//���� ���ķ� �ܾ ����
		index = line.find(" ");
		temp = line.substr(0, index);
		indices[*indiceCount] = std::stoi(temp);  //���� �ϳ� ã�� vertices�� ���� �ִ´�.				
		(*indiceCount)++; //�� ���� �� ��° ���� ���� �Ϸ�

		line = line.substr(index, 100);
		temp = line.substr(1, 3); //�� ��° ���� ���ķ� �ӽ� ����
		indices[*indiceCount] = std::stoi(temp);  //���� �ϳ� ã�� vertices�� ���� �ִ´�.						
		(*indiceCount)++; //�� ���� �� ��° ���� ���� �Ϸ�

	}
	else {
		//VERTEX�� FACE�� �� ��������
		cout << "������ �߸��Ǿ����ϴ�";
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
		printf("����");
	}

	ProgramID = shader::LoadShaders("VertexShader.vert", "FragmentShader.frag");

	modelMatrixID = glGetUniformLocation(ProgramID, "modelMatrix");
	viewMatrixID = glGetUniformLocation(ProgramID, "viewMatrix");
	projectionMatrixID = glGetUniformLocation(ProgramID, "projectionMatrix");

	vertexPositionID = glGetAttribLocation(ProgramID, "vertexPosition");
	vertexColorID = glGetAttribLocation(ProgramID, "vertexColor");
}

void RenderScene() {

	GLfloat modelMatrix[16] = { 10,0,0,0 ,0,10,0,0 , 0,0,10,0 , 30,0,0,1 };
	GLfloat viewMatrix[16] = { 1,0,0,0 , 0,1,0,0 , 0,0,1,0 , 0,0,0,1 };
	GLfloat projMatrix[16] = { 0.01f,0,0,0 , 0,0.01f,0,0 , 0,0,-0.01f,0 , 0,0,0,1 };

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
	
	glDrawArrays(GL_TRIANGLES, 0, info * 3);

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


