#ifndef GRAPHEDITORWIDGET_H
#define GRAPHEDITORWIDGET_H

#include <QWidget>
#include <QTimer>
#include "grapheditor.h"

class GraphEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphEditorWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event) override;
private:
    QTimer timer;
    GraphEditor* editor;
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
signals:
};

#endif // GRAPHEDITORWIDGET_H
