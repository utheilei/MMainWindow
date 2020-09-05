#ifndef MMAINWINDOW_H
#define MMAINWINDOW_H

#include <QFrame>

class TitleBar;
class QVBoxLayout;

class MMainWindow : public QFrame
{
    Q_OBJECT
public:
    explicit MMainWindow(QWidget *parent = nullptr);

    enum tagCursorCtrlStyle
    {
        CursorNormal = 0,    // 普通鼠标
        CursorTopLeft,
        CursorTop,
        CursorTopRight,
        CursorLeft,
        CursorBottomLeft,
        CursorBottom,
        CursorBottomRight,
        CursorRight
    };

    int setCursorStyle(const QPoint& curPoint);    // 当前位置设置鼠标样式

    QWidget* contentWidget();

    TitleBar *titleBar() const;

    void moveToCenter();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    QPoint m_MousePressPos;                 // 鼠标点击位置
    QPoint m_MouseMovePos;                  // 鼠标移动位置
    bool   m_bMousePressed = false;         // 鼠标是否按下
    int    m_nMouseResize;                  // 鼠标设置大小
    bool   m_bMouseResizePressed = false;   // 设置大小的按下
    TitleBar *m_titleBar = Q_NULLPTR;
    QWidget* m_contentWidget;//内容组件
    QVBoxLayout* m_layout;//布局管理器
};

#endif // MMAINWINDOW_H
