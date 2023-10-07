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
    // Устанавливаем цвет для круга
    glColor3f(r, g, b);
    // Рисуем круг
    glBegin(GL_POLYGON);
    for (int i = 0; i < N; ++i)
        glVertex2f(x + R * cos(2 * M_PI / N * i), y + R * sin(2 * M_PI / N * i));
    glEnd();
}

void drawPicture() {
    // Первый круг
    drawCircle(0.3, 0.0, 0.0, 0.0, 1.0, 1.0);

    // Второй круг внутри первого
    drawCircle(0.15, 0.0, -0.05, 0.5, 0.5, 0.0);

    //Квадрат внутри второго круга
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(-0.04, 0.00);   // Верхняя левая вершина
    glVertex2f(0.06, 0.00);    // Верхняя правая вершина
    glVertex2f(0.06, -0.10);   // Нижняя правая вершина
    glVertex2f(-0.04, -0.10);  // Нижняя левая вершина
    glEnd();
}

void display() {
    // Очищаем буфер цвета
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(1.0);
    // Рисуем изображение
    drawPicture();
    // Принудительно выводим все в буфер
    glFlush();
}

int main(int argc, char** argv) {
    // Инициализация GLUT
    glutInit(&argc, argv);
    // Устанавливаем размер окна и его положение
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(480, 270);
    // Создаем окно с заголовком "Lab3"
    glutCreateWindow("Lab3");
    // Инициализация
    init();
    // Устанавливаем функцию отображения
    glutDisplayFunc(display);
    // Запускаем главный цикл GLUT
    glutMainLoop();
    return 0;
}