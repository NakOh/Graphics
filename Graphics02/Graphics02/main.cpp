#include <GL\glew.h>
#include <GL\glut.h>
#include <fstream>
#include <iostream>
#include <string>
#include "shader.hpp"

using namespace std;


class vec4 {
public:
	float x, y, z, w;
	vec4() {
		x = y = z = 0;
		w = 1;
	}
	vec4(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		w = 1;
	}
	void normalize() {
		if (x * x + y * y + z * z < 0) {
			printf("fdsafdsafdsahkflhafhewq hfeiqacfeioa cfweo");
		}
		float dist = sqrt(x * x + y * y + z * z);
		x /= dist;
		y /= dist;
		z /= dist;
	}
	void Mul(vec4& b) {
		x *= b.x;
		y *= b.y;
		z *= b.z;
		w = 1;
	}

	void Add(vec4& b) {
		x += b.x;
		y += b.y;
		z += b.z;
		w = 1;
	}
	void Sub(vec4& b) {
		x -= b.x;
		y -= b.y;
		z -= b.z;
		w = 1;
	}

	static vec4* Cross(vec4& a, vec4& b) {
		vec4* ret = new vec4();
		ret->x = (float)(a.y * b.z - a.z * b.y);
		ret->y = (float)(a.z * b.x - a.x * b.z);
		ret->z = (float)(a.x * b.y - a.y * b.x);
		ret->w = 1;
		return ret;
	}
};


int ProgramID; // Shader Program ID;
int modelMatrixID; // Shader ModelMatrix ID;
int viewMatrixID; // Shader ViewMatrix ID;
int projectionMatrixID; // Shader ProjectionMatrix ID;
int vertexPositionID; // Shader vertexPosition ID;
int vertexColorID; // Shader vectexColor ID;
int vertexNormalID; // Shader vertexNoraml ID;

int cameraID;
int lightID;

GLfloat eyex = 0, eyey = 0, eyez = 0;
GLfloat targetx = 0, targety = 0, targetz = 0;

GLfloat modelMatrix[16] = { 1,0,0,0 ,0,1,0,0 , 0,0,1,0 , 0,0,0,1 };
GLfloat modelMatrix2[16] = { 30,0,0,0 ,0,30,0,0 , 0,0,30,0 , -30,0,0,1 };
GLfloat modelMatrix3[16] = { 30,0,0,0 ,0,30,0,0 , 0,0,30,0 , 0,-30,0,1 };
GLfloat viewMatrix[16] = { 1,0,0,0 , 0,1,0,0 , 0,0,1,0 , 0,0,0,1 };
GLfloat projMatrix[16] = { 0.01f,0,0,0 , 0,0.01f,0,0 , 0,0,-0.01f,0 , 0,0,0,1 };

GLfloat w , h;

GLfloat g_x = 0.f;
int g_timeDelta = 0, g_prevTime = 0;
GLfloat g_velocityX = 5.f;

GLfloat *vertices;
GLfloat *newVertices;
GLfloat *colordata;

vec4 *faceNormalVec;
GLfloat *vertexNormalVec;

GLfloat centerX;
GLfloat centerY;
GLfloat centerZ;

GLint *indices;
GLfloat ratio;
GLfloat aspectRatio;

int info;
int vertex;
int i;

void changeSize(GLsizei w, GLsizei h);
void SetupRC(void);
int readData();
void RenderScene();
void Idle();
void parsingData(string line, int count, int *vertexCount, int *indiceCount);
void checkError();
void MyKeyboardFunc(unsigned char Key, int x, int y);
void MatrixMultiply();
void orthogonal();
void perspective();
void calNormalVector();
void calVertexNormalVector();
void multiply(GLfloat* mat1, GLfloat* mat2);

void main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	glutCreateWindow("HomeWork");

	SetupRC();		// initialize render context(RC)
	readData(); //�ϴ� dat ������ �о�ɴϴ�.
				
	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(MyKeyboardFunc);

	glutMainLoop();		// run GLUT framework
}


void SetupRC(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	int glewtest = glewInit();
	if (glewtest != GLEW_OK) {
		printf("����");
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	ProgramID = shader::LoadShaders("VertexShader.vert", "FragmentShader.frag");

	modelMatrixID = glGetUniformLocation(ProgramID, "modelMatrix");
	viewMatrixID = glGetUniformLocation(ProgramID, "viewMatrix");
	projectionMatrixID = glGetUniformLocation(ProgramID, "projectionMatrix");

	//Fragment
	cameraID = glGetUniformLocation(ProgramID, "cameraPosition");
	lightID = glGetUniformLocation(ProgramID, "lightPosition");
	
	vertexPositionID = glGetAttribLocation(ProgramID, "vertexPosition");
	vertexColorID = glGetAttribLocation(ProgramID, "vertexColor");
	vertexNormalID = glGetAttribLocation(ProgramID, "vertexNormal");



}

void MatrixMultiply() {

	for (int i = 0; i < 16; i++) {
		viewMatrix[i] = 0;
	}

	viewMatrix[0] = 1; viewMatrix[5] = 1; viewMatrix[10] = 1; viewMatrix[15] = 1;
	GLfloat rotationMatrix[16] = { cos(ratio),0,-sin(ratio),0 , 0,1,0, 0 , sin(ratio),0,cos(ratio),0 , 0,0,0,1  };
	GLfloat translate[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		-eyex,-eyey,-eyez,1 };
	
	multiply(rotationMatrix, viewMatrix);
	multiply(translate, viewMatrix);

	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0]);
}

void calVertexNormalVector() {

	vertexNormalVec = new GLfloat[info * 3 * 4]; //���� ���ʹ� Vertex  * 4 ��ŭ (ȣ�����Ͼ)
	vec4* result = new vec4[vertex];
	for (int i = 0; i < vertex; i++) {
		result[i] = vec4();
		for (int j = 0; j < info * 3; j++)		
			if (i == indices[j])	
				result[i].Add(faceNormalVec[j / 3]);
		result[i].normalize();
	}

	for (int i = 0; i < info * 3; i++) {
		int face = indices[i];
		vertexNormalVec[i * 4] = result[face].x;
		vertexNormalVec[i * 4 + 1] = result[face].y;
		vertexNormalVec[i * 4 + 2] = result[face].z;
		vertexNormalVec[i * 4 + 3] = result[face].w;

		cout << "   x : "  << vertexNormalVec[i * 4];
		cout << ",  y : " << vertexNormalVec[i * 4 + 1 ];
		cout << ",  z : " << vertexNormalVec[i * 4 + 2];
		cout << ",  w : " << vertexNormalVec[i * 4 + 3 ] << endl;
	}
	delete result;
	delete[] faceNormalVec;
}

void calNormalVector() {

	faceNormalVec = new vec4[info]; //���� ���ʹ� Vertex ����ŭ

	vec4 temp1 = vec4();
	vec4 temp2 = vec4();
	vec4 temp3 = vec4();
	vec4 *result;
	
	int index = 0;


	for (int j = 0; j < info; j++) {
		//indice�� 3���� �߶�, �� 3���� ������ ������ Normal Vector�� ���Ѵ�. 
		index = indices[j * 3]; // FACE ������ �����ͼ� �ش� vertex�� ��ǥ ������ ������ �Է�����.	
		temp1.x = vertices[3 * index + 0];
		temp1.y = vertices[3 * index + 1];
		temp1.z = vertices[3 * index + 2];
		temp1.w = 1;

		index = indices[j * 3 + 1]; // FACE ������ �����ͼ� �ش� vertex�� ��ǥ ������ ������ �Է�����.	
		temp2.x = vertices[3 * index + 0];
		temp2.y = vertices[3 * index + 1];
		temp2.z = vertices[3 * index + 2];
		temp2.w = 1;

		index = indices[j * 3 + 2]; // FACE ������ �����ͼ� �ش� vertex�� ��ǥ ������ ������ �Է�����.	
		temp3.x = vertices[3 * index + 0];
		temp3.y = vertices[3 * index + 1];
		temp3.z = vertices[3 * index + 2];
		temp3.w = 1;

		//�� ������ ������ Normal Vector�� ������.		
		temp2.Sub(temp1);
		temp3.Sub(temp1);

		result = vec4::Cross(temp2, temp3);
		result->normalize();

		faceNormalVec[j].x = result->x;
		faceNormalVec[j].y = result->y;
		faceNormalVec[j].z = result->z;
		faceNormalVec[j].w = result->w;

		/*
		cout << "x : " << faceNormalVec[j].x << ",  y : ";
		cout << faceNormalVec[j].y << ",  z : ";
		cout << faceNormalVec[j].z << "  ";
		cout << faceNormalVec[j].w << endl;
		//*/
	}
	delete result;
}

void multiply(GLfloat* mat1, GLfloat* mat2) {

	GLfloat tempMat[16] = { 0, };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float sum = 0.0f;
			for (int k = 0; k < 4; k++)
				sum += mat1[i * 4 + k] * mat2[k * 4 + j];
			tempMat[i * 4 + j] = sum;
		}
	}

	for (int i = 0; i < 16; i++) {
		mat2[i] = tempMat[i];
	}

}


void MyKeyboardFunc(unsigned char Key, int x, int y) {
	switch (Key) { 
	case 'w': //ī�޶� Z �� ����		
		eyez += 0.3f;
		break;
	case 's': //ī�޶� Z �� ����		
		eyez -= 0.3f;
		break;
	case 'a': //ī�޶� X �� ����
		eyex += 0.3f;
		break;
	case 'd': //ī�޶� X �� ����
		eyex -= 0.3f;
		break;
	case 'r': //ī�޶� Y �� ����		
		eyey += 0.3f;
		break;
	case 'f': //ī�޶� Y �� ����		
		eyey -= 0.3f;
		break;
	case 'q': //ī�޶� y�� ���� ȸ��
		ratio+= 0.01f;
		break;
	case 'e' : //ī�޶� y�� ���� ȸ��
		ratio -= 0.01f;		
		break;
	case 'p': //Perspective Projection
		perspective();
		break;
	case 'o': // Orthographic Projection
		orthogonal();
		break;
	}
	glutPostRedisplay();
}

void perspective() {
	float r = 50.f;
	float l = -50.f;
	float t = 50.f;
	float b = -50.f;
	float n = 2.f;
	float f = 3000.f;

	projMatrix[0] = 1/n;
	projMatrix[1] = 0;
	projMatrix[2] = 0;
	projMatrix[3] = 0;

	projMatrix[4] = 0;
	projMatrix[5] = 1/n;
	projMatrix[6] = 0;
	projMatrix[7] = 0;
	projMatrix[8] = 0;
	projMatrix[9] = 0;
	projMatrix[10] = -(f+n) / (f-n);
	projMatrix[11] = -1;
	projMatrix[12] = 0;
	projMatrix[13] = 0;
	projMatrix[14] = (-2 *f * n ) / (f-n);
	projMatrix[15] = 0;
}

void orthogonal() {
	//left = right, top = bottom
	projMatrix[0] = 0.01f;
	projMatrix[1] = 0;
	projMatrix[2] = 0;
	projMatrix[3] = 0;

	projMatrix[4] = 0;
	projMatrix[5] = 0.01f;
	projMatrix[6] = 0;
	projMatrix[7] = 0;

	projMatrix[8] = 0;
	projMatrix[9] = 0;
	projMatrix[10] = -0.01f;
	projMatrix[11] = 0;

	projMatrix[12] = 0;
	projMatrix[13] = 0;
	projMatrix[14] = 0;
	projMatrix[15] = 1;
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

		for (j = 0; j < info * 3; j++) {
			indexs = indices[j]; // FACE ������ �����ͼ� �ش� vertex�� ��ǥ ������ ������ �Է�����.	
			newVertices[i] = vertices[3 * (indexs)];
			colordata[i] = 1.0f;
			i = i + 1;
			newVertices[i] = vertices[(3 * (indexs)) + 1];
			colordata[i] = 1.0f;
			i = i + 1;
			newVertices[i] = vertices[(3 * (indexs)) + 2];
			colordata[i] = 1.0f;
			i = i + 1;
			newVertices[i] = 1;
			colordata[i] = 1.0f;
			i = i + 1;
		}
		calNormalVector(); //face Normal ���� ���ϱ�
		calVertexNormalVector(); // vertex Noraml ���� ���ϱ�
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
		vertices[*vertexCount] = std::stof(temp) / 300;  //���� �ϳ� ã�� vertices�� ���� �ִ´�.	
		centerX += stof(temp);
		(*vertexCount)++; //�� ���� ù��° ���� ���� �Ϸ�	

		line = line.substr(index + 1, 100);//���� ���ķ� �ܾ ����
		index = line.find(" ");
		temp = line.substr(0, index);
		vertices[*vertexCount] = std::stof(temp) / 300;  //���� �ϳ� ã�� vertices�� ���� �ִ´�.		
		centerY += stof(temp);
		(*vertexCount)++; //�� ���� �� ��° ���� ���� �Ϸ�

		line = line.substr(index, 100);

		temp = line.substr(1, 100); //�� ��° ���� ���ķ� �ӽ� ����
		vertices[*vertexCount] = std::stof(temp) / 300;  //���� �ϳ� ã�� vertices�� ���� �ִ´�.
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


void RenderScene() {

	static float rotation = 0.f;

	for (int i = 0; i < 16; i++) {
		modelMatrix[i] = 0;
		modelMatrix2[i] = 0;
		modelMatrix3[i] = 0;
	}

	modelMatrix[0] = 1;
	modelMatrix[5] = 1;
	modelMatrix[10] = 1;
	modelMatrix[15] = 1;

	modelMatrix2[0] = 1;
	modelMatrix2[5] = 1;
	modelMatrix2[10] = 1;
	modelMatrix2[15] = 1;

	modelMatrix3[0] = 1;
	modelMatrix3[5] = 1;
	modelMatrix3[10] = 1;
	modelMatrix3[15] = 1;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(ProgramID);
	
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projMatrix[0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0]);

	glUniform4f(cameraID, eyex, eyey, eyez, 1); //ī�޶� ��ġ
	glUniform4f(lightID, 1, -5, -5, 1);

	glEnableVertexAttribArray(vertexPositionID);
	glEnableVertexAttribArray(vertexColorID);
	glEnableVertexAttribArray(vertexNormalID);
	

	GLfloat scaleN = 30.0f;
	GLfloat pos[3] = { -1.0f,1.0f,0 };
	GLfloat scale[16] = {
		scaleN, 0, 0, 0,
		0, scaleN, 0, 0,
		0, 0, scaleN, 0,
		0, 0, 0, 1 };
	GLfloat translate[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		pos[0],pos[1],pos[2],1 };
	GLfloat translate2[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		-pos[0],-pos[1],-pos[2],1 };
	GLfloat rotationMatrix[16] = { cos(rotation),sin(rotation),0 ,0 ,-sin(rotation),cos(rotation),0, 0 , 0,0,1,0 , 0,0,0,1 };
	GLfloat rotationMatrix2[16] = { 1, 0, 0, 0, 0, cos(rotation),sin(rotation),0 ,0 ,-sin(rotation),cos(rotation),1 };
	rotation = rotation + 0.001f;

	multiply(scale, modelMatrix);
	multiply(translate, modelMatrix);
	multiply(rotationMatrix, modelMatrix);
	//multiply(rotationMatrix2, modelMatrix);
	multiply(translate2, modelMatrix);


	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0]);

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

	glVertexAttribPointer(
		vertexNormalID,				// The attribute we want to configure
		4,							    // size
		GL_FLOAT,						// type
		GL_FALSE,						// normalized?
		0,								// stride
		(void*)&vertexNormalVec[0]			// array buffer offset
		);



	glDrawArrays(GL_TRIANGLES, 0, info * 3);
	
	translate[12] = 1.0f;
	translate[13] = 0.0f;
	translate[14] = 0.0f;
	multiply(scale, modelMatrix2);
	
	multiply(translate, modelMatrix2);
	multiply(rotationMatrix, modelMatrix2);

	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix2[0]);	

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
	glVertexAttribPointer(
		vertexNormalID,				// The attribute we want to configure
		4,							    // size
		GL_FLOAT,						// type
		GL_FALSE,						// normalized?
		0,								// stride
		(void*)&vertexNormalVec[0]			// array buffer offset
		);

	glDrawArrays(GL_TRIANGLES, 0, info * 3);
	////


	translate[12] = -20.0f;
	translate[13] = -20.0f;
	translate[14] = 1.0f;

	static GLfloat thisVariable = 1.0f;
	static GLfloat thatVariable = 0.001f;
	
	if (thisVariable > 1.8f) {
		thatVariable = -0.001f;
	}
	if (thisVariable < 0.7f) {
		thatVariable = 0.001f;
	}

	thisVariable += thatVariable;
	GLfloat scale2[16] = {
		thisVariable, 0, 0, 0,
		0, thisVariable, 0, 0,
		0, 0, thisVariable, 0,
		0, 0, 0, 1 };

	multiply(translate, modelMatrix3);
	multiply(scale, modelMatrix3);
	multiply(scale2, modelMatrix3);

	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix3[0]);
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
	glVertexAttribPointer(
		vertexNormalID,				// The attribute we want to configure
		4,							    // size
		GL_FLOAT,						// type
		GL_FALSE,						// normalized?
		0,								// stride
		(void*)&vertexNormalVec[0]			// array buffer offset
		);

	glDrawArrays(GL_TRIANGLES, 0, info * 3);

	glDisableVertexAttribArray(vertexPositionID);
	glDisableVertexAttribArray(vertexColorID);
	glDisableVertexAttribArray(vertexNormalID);

	MatrixMultiply();

	glUseProgram(0);
	glutSwapBuffers();


}

void changeSize(GLsizei w2, GLsizei h2) {
	w = w2;
	h = h2;
	if (h == 0)
		h = 1;
	aspectRatio = (GLfloat)w / (GLfloat)h;
	glViewport(0, 0, w, h);

}


