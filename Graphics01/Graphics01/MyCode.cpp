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
int info;

void changeSize(GLsizei w, GLsizei h);
void SetupRC(void);
int readData();
void RenderScene();
void Idle();
void parsingData(string line,int count, int *vertexCount, int *indiceCount);

int	main(int argc, char* argv[])
{	
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
	glutCreateWindow("HomeWork");
	readData(); //�ϴ� dat ������ �о�ɴϴ�.
	// callback function
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(changeSize);
	//glutIdleFunc(Idle);
	SetupRC();		// initialize render context(RC)
	glutMainLoop();		// run GLUT framework
	return 0;	

}

int readData() {
	string line;
	string temp;

	int index;
	int secondIndex;
	int count = -1;
	int vertexCount = 0;
	int indiceCount = 0;

	ifstream myfile("models/teapot.dat");

	if (myfile.is_open())
	{			
		while (getline(myfile, line)) //dat������ ���� �� �д´�.
		{					
			//cout << line << '\n';
			if (line.find("VERTEX") != string::npos) { //���� VERTEX�� �ִٸ�
				index = line.find("="); //�ϴ� = �� ã�´�.				
				info = std::stoi(line.substr(index + 2, 4)); //ã�� �ε������� 2��° �ں��� �� �������� string���� int�� �ٲ۴�.				
				vertices = new GLfloat[info*3]; // �ش� int��ŭ �迭 ���� ����
				count = 0;
			} 
			else if (line.find("FACE") != string::npos) { //���� FACE�� �ִٸ�
				index = line.find("="); //�ϴ� = �� ã�´�.				
				info = std::stoi(line.substr(index + 2, 4)); //ã�� �ε������� 2��° �ں��� �� �������� string���� int�� �ٲ۴�.				
				indices = new GLint[info*3]; // �ش� int��ŭ �迭 ���� ����
				count = 1;
			}
			else { //�׳� ���� �϶�				
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
		//VERTEX�� ���� ��
		index = line.find(" "); //���⸦ ã�´�. 
		temp = line.substr(0, index); //ó������ �� �ε������� �� ����
		vertices[*vertexCount] = std::stof(temp) / 200;  //���� �ϳ� ã�� vertices�� ���� �ִ´�.	
		(*vertexCount)++; //�� ���� ù��° ���� ���� �Ϸ�	

		line = line.substr(index+1, 100);//���� ���ķ� �ܾ ����
		index = line.find(" ");
		temp = line.substr(0, index);
		vertices[*vertexCount] = std::stof(temp) / 200;  //���� �ϳ� ã�� vertices�� ���� �ִ´�.		
		(*vertexCount)++; //�� ���� �� ��° ���� ���� �Ϸ�
		
		line = line.substr(index, 100);

		temp = line.substr(1, 100); //�� ��° ���� ���ķ� �ӽ� ����
		vertices[*vertexCount] = std::stof(temp) / 200;  //���� �ϳ� ã�� vertices�� ���� �ִ´�.
		(*vertexCount)++; //�� ���� �� ��° ���� ���� �Ϸ�	
		
	}
	else if (count == 1) {
		//FACE�� ���� ��
		index = line.find(" "); //���⸦ ã�´�. 
		temp = line.substr(0, index); //ó������ �� �ε������� �� ����
		indices[*indiceCount] = std::stoi(temp);  //���� �ϳ� ã�� vertices�� ���� �ִ´�.
		cout << indices[*indiceCount];
		(*indiceCount)++; //�� ���� ù��° ���� ���� �Ϸ�		
		
		line = line.substr(index + 1, 100);//���� ���ķ� �ܾ ����
		index = line.find(" ");
		temp = line.substr(0, index);
		indices[*indiceCount] = std::stoi(temp);  //���� �ϳ� ã�� vertices�� ���� �ִ´�.		
		cout << indices[*indiceCount];
		(*indiceCount)++; //�� ���� �� ��° ���� ���� �Ϸ�

		line = line.substr(index, 100);
		

		temp = line.substr(1, 3); //�� ��° ���� ���ķ� �ӽ� ����
		indices[*indiceCount] = std::stoi(temp);  //���� �ϳ� ã�� vertices�� ���� �ִ´�.				
		cout << indices[*indiceCount] << endl;
		(*indiceCount)++; //�� ���� �� ��° ���� ���� �Ϸ�
		
	}
	else {
		//VERTEX�� FACE�� �� ��������
		cout << "������ �߸��Ǿ����ϴ�";		
	}
	
}


void Idle() {
	/*
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	g_timeDelta = currentTime - g_prevTime;
	g_prevTime = currentTime;
	glutPostRedisplay();
	*/
}

void SetupRC(void)
{
	// setup clear color
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.5f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawElements(GL_TRIANGLES, info * 3, GL_UNSIGNED_INT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
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
	//gluPerspective(60.f, aspectRatio, 0.f, 100.f);
	//glOrtho(-100.f, 100.f, -100.f, 100.f, -100.f, 100.f);
	
}


