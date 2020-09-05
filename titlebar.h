#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

class QLabel;
class QPushButton;
class QMenu;

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);

    void setWindowTitle(const QString& title);
    void SetTitleBarIcon(const QIcon& icon);

    void setMenu(QMenu* menu);
    QMenu* menu() const;
private:
    void addMenu();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    void updateMaximize();

protected slots:
    void onClicked();

private:
    QLabel* m_iconLabel;
    QLabel* m_titleLabel;
    QPushButton *m_meunBtn = Q_NULLPTR;
    QPushButton* m_minimizeButton;
    QPushButton* m_maximizeButton;
    QPushButton* m_closeButton;
    QPoint m_start;//起始点
    QPoint m_end;//结束点
    bool m_leftButtonPressed;//鼠标左键按下标记
    QSize m_size = QSize(40,40);
};

#endif // TITLEBAR_H
