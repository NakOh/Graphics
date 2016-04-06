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
	readData(); //�ϴ� dat ������ �о�ɴϴ�.
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
		while (getline(myfile, line)) //dat������ ���� �� �д´�.
		{					
			//cout << line << '\n';
			if (line.find("VERTEX") != string::npos) { //���� VERTEX�� �ִٸ�
				index = line.find("="); //�ϴ� = �� ã�´�.				
				vertex = std::stoi(line.substr(index + 2, 4)); //ã�� �ε������� 2��° �ں��� �� �������� string���� int�� �ٲ۴�.				
				vertices = new GLfloat[vertex*3]; // �ش� int��ŭ �迭 ���� ����
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
		vertices[*vertexCount] = std::stof(temp)/400;  //���� �ϳ� ã�� vertices�� ���� �ִ´�.	
		centerX += stof(temp);
		(*vertexCount)++; //�� ���� ù��° ���� ���� �Ϸ�	

		line = line.substr(index+1, 100);//���� ���ķ� �ܾ ����
		index = line.find(" ");
		temp = line.substr(0, index);
		vertices[*vertexCount] = std::stof(temp)/400;  //���� �ϳ� ã�� vertices�� ���� �ִ´�.		
		centerY += stof(temp);
		(*vertexCount)++; //�� ���� �� ��° ���� ���� �Ϸ�
		
		line = line.substr(index, 100);

		temp = line.substr(1, 100); //�� ��° ���� ���ķ� �ӽ� ����
		vertices[*vertexCount] = std::stof(temp)/400;  //���� �ϳ� ã�� vertices�� ���� �ִ´�.
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

	//�ڽ� �߽� ȸ��
	glPopMatrix();
	glTranslatef(-0.7f, 0, 0);
	glTranslatef(center_x , center_y, center_z);
	glRotatef(rot, 0.0, 0.0, 1.0);
	glTranslatef(-(center_x) , -center_y, -center_z);	
	glDrawElements(GL_TRIANGLES, info * 3, GL_UNSIGNED_INT, indices);
	
	rot -= 0.005f;	

	//ũ�� Ŀ�� �۾���	
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

	//�ٸ� �� �߽� ȸ��	
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


