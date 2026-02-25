#include "grapheditorwidget.h"
#include "graphicsqt.h"
#include <QPainter>
#include <QMouseEvent>
GraphEditorWidget::GraphEditorWidget(QWidget *parent)
    : QWidget{parent}
{

    setMouseTracking(true);
    editor = GraphEditor::getInstance();
    connect(&timer,&QTimer::timeout,this,[=](){
        editor->simulationStep();
        this->repaint();
    });
    timer.start(1000.0/60);

}

void GraphEditorWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    GraphicsQT adapter(&painter,rect());
    editor->draw(adapter);
}

void GraphEditorWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos  = event->pos();
    editor->mouseMoveEvent(pos.x(),pos.y());
}

void GraphEditorWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint pos  = event->pos();
    auto b = event->button();
    if(b == Qt::LeftButton){
        editor->mousePressEvent(pos.x(),pos.y(),GraphEditor::MOUSE_BUTTON_LEFT);
    }
    else if(b == Qt::RightButton){
        editor->mousePressEvent(pos.x(),pos.y(),GraphEditor::MOUSE_BUTTON_RIGHT);
    }
}

void GraphEditorWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pos  = event->pos();
    auto b = event->button();
    if(b == Qt::LeftButton){
        editor->mouseReleaseEvent(pos.x(),pos.y(),GraphEditor::MOUSE_BUTTON_LEFT);
    }
    else if(b == Qt::RightButton){
        editor->mouseReleaseEvent(pos.x(),pos.y(),GraphEditor::MOUSE_BUTTON_RIGHT);
    }
}
