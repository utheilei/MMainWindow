#include "mmainwindow.h"
#include "titlebar.h"

#include <QStylePainter>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

MMainWindow::MMainWindow(QWidget *parent) : QFrame(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setMouseTracking(true);

    m_titleBar = new TitleBar(this);
    installEventFilter(m_titleBar);
    m_contentWidget = new QWidget(this);
    m_contentWidget->setObjectName("Contents");
    m_layout = new QVBoxLayout;
    m_layout->addWidget(m_titleBar);
    m_layout->addWidget(m_contentWidget);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(5, 5, 5, 5);

    setLayout(m_layout);

    setWindowTitle(qApp->applicationName());
    setWindowIcon(qApp->windowIcon());
}

QWidget *MMainWindow::contentWidget()
{
    return m_contentWidget;
}

TitleBar *MMainWindow::titleBar() const
{
    return m_titleBar;
}

void MMainWindow::moveToCenter()
{
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

int MMainWindow::setCursorStyle(const QPoint &curPoint)
{
    int nCurWidth = this->width();
    int nCurHeight = this->height();
    int nRes = CursorNormal;

    if ( (nCurWidth - curPoint.x() <= 3) && (nCurHeight - curPoint.y() <= 3) )
    {
        setCursor(Qt::SizeFDiagCursor);
        nRes = CursorBottomRight;
    }
    else if (curPoint.x() <= 3 && curPoint.y() <= 3) {
        setCursor(Qt::SizeFDiagCursor);
        nRes = CursorTopLeft;
    }
    else if (curPoint.x() <= 3 && (nCurHeight - curPoint.y() <= 3)) {
        setCursor(Qt::SizeBDiagCursor);
        nRes = CursorBottomLeft;
    }
    else if ((nCurWidth - curPoint.x() <= 3) && curPoint.y() <= 3) {
        setCursor(Qt::SizeBDiagCursor);
        nRes = CursorTopRight;
    }
    else if (nCurWidth - curPoint.x() <= 3)
    {
        setCursor(Qt::SizeHorCursor);
        nRes = CursorRight;
    }
    else if (nCurHeight - curPoint.y() <= 3)
    {
        setCursor(Qt::SizeVerCursor);
        nRes = CursorBottom;
    }
    else if (curPoint.x() <= 3) {
        setCursor(Qt::SizeHorCursor);
        nRes = CursorLeft;
    }
    else if (curPoint.y() <= 3) {
        setCursor(Qt::SizeVerCursor);
        nRes = CursorTop;
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        nRes = CursorNormal;
    }

    return nRes;
}

void MMainWindow::paintEvent(QPaintEvent *event)
{
    QStylePainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);

    QPainterPath rectPath;
    rectPath.addRoundedRect(rect(),20,20);
    painter.setClipPath(rectPath);
    painter.drawPath(rectPath);

    painter.fillPath(rectPath,palette().color(QPalette::Window));
}

void MMainWindow::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton)
    {
        m_MousePressPos = event->globalPos();
        m_bMousePressed = true;
    }

    if (CursorNormal != m_nMouseResize)
    {
        m_bMouseResizePressed = true;
    }
}

void MMainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton )
    {
        m_bMousePressed = false;
        m_bMouseResizePressed = false;
        m_nMouseResize = setCursorStyle(event->pos());
    }
}

void MMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(Qt::WindowMaximized == windowState())
        return;

    QPoint curPoint = event->pos();
    if (!m_bMouseResizePressed)
        m_nMouseResize = setCursorStyle(curPoint);

    if( m_bMousePressed && (event->buttons() == Qt::LeftButton))
    {
        m_MouseMovePos = event->globalPos();
        QPoint movePoint = m_MouseMovePos - m_MousePressPos;
        if (CursorNormal == m_nMouseResize) {
            this->move( this->pos() + movePoint);
        } else {
            QRect rect = geometry();

            switch (m_nMouseResize)
            {
            case CursorTopLeft:
                rect.setTopLeft(rect.topLeft() + movePoint);
                break;
            case CursorTop:
                rect.setTop(rect.top() + movePoint.y());
                break;
            case CursorTopRight:
                rect.setTopRight(rect.topRight() + movePoint);
                break;
            case CursorLeft:
                rect.setLeft(rect.left() + movePoint.x());
                break;
            case CursorBottomLeft:
                rect.setBottomLeft(rect.bottomLeft() + movePoint);
                break;
            case CursorBottom:
                rect.setBottom(rect.bottom() + movePoint.y());
                break;
            case CursorBottomRight:
                rect.setBottomRight(rect.bottomRight() + movePoint);
                break;
            case CursorRight:
                rect.setRight(rect.right() + movePoint.x());
                break;
            default:
                break;
            }

            if(rect.width() <= minimumWidth() && rect.height() <= minimumHeight())
                return;

            setGeometry(rect);
        }
        m_MousePressPos = event->globalPos();
    }
}
