#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLBuffer>
#include <QPainter>

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT

public:
    MyOpenGLWidget(QWidget *parent = nullptr);
    MyOpenGLWidget(QWidget *parent = nullptr, int a=0, int b=0, int c=0, int d=0, int e=0);
    ~MyOpenGLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    QOpenGLShaderProgram program;
    GLuint VAO, VBO, EBO;
    QVector<QVector3D> points;
    QVector<GLuint> indices;

    QVector<QString> labels;
    QVector<QVector3D> colors;

    QVector<GLuint> indicesX;
    QVector<GLuint> indicesY;
    QVector<QVector3D> axisX;
    QVector<QVector3D> axisY;

    void createDiagramPoints();
    void cleanup();
    void renderText(const QPointF &point, const QString &text);

    int point_a,point_b,point_c,point_d,point_e;
};

#endif // MYOPENGLWIDGET_H
