#include <gl/glut.h>
#include <cmath>

#define M_PI 3.14159265358979323846
#define N 50

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

void drawCircle(double R, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b) {
    // ������������� ���� ��� �����
    glColor3f(r, g, b);
    // ������ ����
    glBegin(GL_POLYGON);
    for (int i = 0; i < N; ++i)
        glVertex2f(x + R * cos(2 * M_PI / N * i), y + R * sin(2 * M_PI / N * i));
    glEnd();
}

void drawPicture() {
    // ������ ����
    drawCircle(0.3, 0.0, 0.0, 0.0, 1.0, 1.0);

    // ������ ���� ������ �������
    drawCircle(0.15, 0.0, -0.05, 0.5, 0.5, 0.0);

    //������� ������ ������� �����
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(-0.04, 0.00);   // ������� ����� �������
    glVertex2f(0.06, 0.00);    // ������� ������ �������
    glVertex2f(0.06, -0.10);   // ������ ������ �������
    glVertex2f(-0.04, -0.10);  // ������ ����� �������
    glEnd();
}

void display() {
    // ������� ����� �����
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(1.0);
    // ������ �����������
    drawPicture();
    // ������������� ������� ��� � �����
    glFlush();
}

int main(int argc, char** argv) {
    // ������������� GLUT
    glutInit(&argc, argv);
    // ������������� ������ ���� � ��� ���������
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(480, 270);
    // ������� ���� � ���������� "Lab3"
    glutCreateWindow("Lab3");
    // �������������
    init();
    // ������������� ������� �����������
    glutDisplayFunc(display);
    // ��������� ������� ���� GLUT
    glutMainLoop();
    return 0;
}