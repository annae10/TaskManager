#include "MyOpenGLWidget.h"

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), VAO(0), VBO(0), EBO(0) {}

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent, int a, int b, int c, int d, int e) : QOpenGLWidget(parent), VAO(0), VBO(0), EBO(0)
{
    qDebug()<<"a="<<point_a<<"b="<<point_b<<"c="<<point_c<<"d="<<point_d<<"e="<<point_e;
    point_a=a;point_b=b;point_c=c;point_d=d;point_e=e;
    qDebug()<<"a="<<point_a<<"b="<<point_b<<"c="<<point_c<<"d="<<point_d<<"e="<<point_e;
}

MyOpenGLWidget::~MyOpenGLWidget() {
    cleanup();
}

void MyOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:/Users/Professional/Documents/opengl_diagram/vertex_shader.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:/Users/Professional/Documents/opengl_diagram/fragment_shader.glsl");
    program.link();

    createDiagramPoints();
}

void MyOpenGLWidget::paintGL() {
    glClearColor(1.0f,1.0f,1.0f, 1);//background
    glClear(GL_COLOR_BUFFER_BIT);
    program.bind();
    glBindVertexArray(VAO);


    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    program.release();

}

void MyOpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void MyOpenGLWidget::renderText(const QPointF &point, const QString &text){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.drawText(point, text);
}

void MyOpenGLWidget::createDiagramPoints() {

    int point1=point_a;
    if(point_b>point1)point1=point_b;
    if(point_c>point1)point1=point_c;
    if(point_d>point1)point1=point_d;
    if(point_e>point1)point1=point_e;

    float f = 1.8/point1;

    int abc = 5;

    qDebug()<<"a="<<point_a<<"b="<<point_b<<"c="<<point_c<<"d="<<point_d<<"e="<<point_e;
    points << QVector3D(-0.9f, -0.9f, 0.0f) << QVector3D(-0.9f, f*point_a-0.9f, 0.0f)
           << QVector3D(-0.7f, f*point_a-0.9f, 0.0f) << QVector3D(-0.7f, -0.9f, 0.0f)
           << QVector3D(-0.5f, -0.9f, 0.0f) << QVector3D(-0.5f, f*point_b-0.9f, 0.0f)
           << QVector3D(-0.3f, f*point_b-0.9f, 0.0f) << QVector3D(-0.3f, -0.9f, 0.0f)
           << QVector3D(-0.1f, -0.9f, 0.0f) << QVector3D(-0.1f, f*point_c-0.9f, 0.0f)
           << QVector3D(0.1f, f*point_c-0.9f, 0.0f) << QVector3D(0.1f, -0.9f, 0.0f)
           << QVector3D(0.3f, -0.9f, 0.0f) << QVector3D(0.3f, f*point_d-0.9f, 0.0f)
           << QVector3D(0.5f, f*point_d-0.9f, 0.0f) << QVector3D(0.5f, -0.9f, 0.0f)
           << QVector3D(0.7f, -0.9f, 0.0f) << QVector3D(0.7f, f*point_e-0.9f, 0.0f)
           << QVector3D(0.9f, f*point_e-0.9f, 0.0f) << QVector3D(0.9f, -0.9f, 0.0f);

    colors << QVector3D(1.0f, 0.7f, 0.7f) << QVector3D(0.3f, 0.3f, 1.0f)
           << QVector3D(0.3f, 0.3f, 1.0f) << QVector3D(1.0f, 0.7f, 0.7f)
           << QVector3D(1.0f, 0.9f, 0.9f) << QVector3D(0.5f, 0.2f, 0.6f)
           << QVector3D(0.5f, 0.2f, 0.6f) << QVector3D(1.0f, 0.9f, 0.9f)
           << QVector3D(1.0f, 0.9f, 0.9f) << QVector3D(0.3f, 0.3f, 1.0f)
           << QVector3D(0.3f, 0.3f, 1.0f) << QVector3D(1.0f, 0.9f, 0.9f)
           << QVector3D(1.0f, 0.7f, 0.7f) << QVector3D(0.5f, 0.2f, 0.6f)
           << QVector3D(0.5f, 0.2f, 0.6f) << QVector3D(1.0f, 0.7f, 0.7f)
           << QVector3D(1.0f, 0.8f, 0.5f) << QVector3D(0.3f, 0.3f, 1.0f)
           << QVector3D(0.3f, 0.3f, 1.0f) << QVector3D(1.0f, 0.8f, 0.5f);

    indices = {0, 1, 2, 2, 3, 0,
               4, 5, 6, 6, 7, 4,
               8, 9, 10, 10, 11, 8,
              12,13,14,14,15,12,
               16,17,18,18,19,16
              };

    // Create VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (points.size() + colors.size()) * sizeof(QVector3D), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(QVector3D), points.constData());
    glBufferSubData(GL_ARRAY_BUFFER, points.size() * sizeof(QVector3D), colors.size() * sizeof(QVector3D), colors.constData());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.constData(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(points.size() * sizeof(QVector3D)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MyOpenGLWidget::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
