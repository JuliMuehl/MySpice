#ifndef MOUSEEVENTOBSERVER_H
#define MOUSEEVENTOBSERVER_H

class MouseEventObserver
{
public:
    static constexpr int MOUSE_BUTTON_LEFT = 0;
    static constexpr int MOUSE_BUTTON_RIGHT = 1;
    virtual void mouseMoveEvent(int x,int y) {};
    virtual void mousePressEvent(int x, int y, int button) {};
    virtual void mouseReleaseEvent(int x, int y, int button) {};
};

#endif // MOUSEEVENTOBSERVER_H
