#include "titlebar.h"

#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QApplication>
#include <QMouseEvent>
#include <QStyle>
#include <QMenu>
#include <QDebug>

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(50);
    setWindowFlags(Qt::FramelessWindowHint);
    setObjectName("titleBar");
    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(35, 35);
    m_iconLabel->setScaledContents(true);

    m_titleLabel = new QLabel(this);
    m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_meunBtn = new QPushButton(this);
    m_meunBtn->setFixedSize(m_size);
    m_meunBtn->setObjectName("menuButton");
    m_meunBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarMenuButton));
    m_meunBtn->setIconSize(m_size);
    m_meunBtn->setFlat(true);
    addMenu();

    m_minimizeButton = new QPushButton(this);
    m_minimizeButton->setFixedSize(m_size);
    m_minimizeButton->setObjectName("minimizeButton");
    m_minimizeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton));
    m_minimizeButton->setIconSize(m_size);
    m_minimizeButton->setFlat(true);

    m_maximizeButton = new QPushButton(this);
    m_maximizeButton->setFixedSize(m_size);
    m_maximizeButton->setObjectName("maximizeButton");
    m_maximizeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    m_maximizeButton->setIconSize(m_size);
    m_maximizeButton->setFlat(true);

    m_closeButton = new QPushButton(this);
    m_closeButton->setFixedSize(m_size);
    m_closeButton->setObjectName("closeButton");
    m_closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    m_closeButton->setIconSize(m_size);
    m_closeButton->setFlat(true);

    m_minimizeButton->setToolTip("Minimize");
    m_maximizeButton->setToolTip("Maximize");
    m_closeButton->setToolTip("Close");

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(m_iconLabel);
    layout->addStretch();
    layout->addWidget(m_titleLabel, 0, Qt::AlignCenter);
    layout->addStretch();
    layout->addWidget(m_meunBtn);
    layout->addWidget(m_minimizeButton);
    layout->addWidget(m_maximizeButton);
    layout->addWidget(m_closeButton);
    setLayout(layout);

    connect(m_minimizeButton, &QPushButton::clicked, this, &TitleBar::onClicked);
    connect(m_maximizeButton, &QPushButton::clicked, this, &TitleBar::onClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &TitleBar::onClicked);
}

void TitleBar::setWindowTitle(const QString &title)
{
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setText(title);
}

void TitleBar::SetTitleBarIcon(const QIcon& icon)
{
    m_iconLabel->setPixmap(icon.pixmap(m_iconLabel->size()));
}

void TitleBar::setMenu(QMenu *menu)
{
    m_meunBtn->setMenu(menu);
}

QMenu *TitleBar::menu() const
{
    return m_meunBtn->menu();
}

void TitleBar::addMenu()
{
    QMenu *menu = new QMenu(this);
    menu->addAction(tr("Theme"));
    menu->addSeparator();
    menu->addAction(tr("About"));
    menu->addAction(tr("Exit"));
    //m_meunBtn->setMenu(menu);
    connect(m_meunBtn,&QPushButton::clicked,[=](){
        QPoint pos = mapToGlobal(m_meunBtn->geometry().bottomLeft());
        menu->exec(pos);
    });
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_maximizeButton->click();

    QWidget::mouseDoubleClickEvent(event);
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    // 鼠标左键按下事件
    if (event->button() == Qt::LeftButton)
    {
        // 记录鼠标左键状态
        m_leftButtonPressed = true;
        //记录鼠标在屏幕中的位置
        m_start = event->globalPos();
    }

    QWidget::mousePressEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    // 持续按住才做对应事件
    if(m_leftButtonPressed)
    {
        //将父窗体移动到父窗体原来的位置加上鼠标移动的位置：event->globalPos()-m_start
        parentWidget()->move(parentWidget()->geometry().topLeft() +
                             event->globalPos() - m_start);
        //将鼠标在屏幕中的位置替换为新的位置
        m_start = event->globalPos();
    }

    QWidget::mouseMoveEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    // 鼠标左键释放
    if (event->button() == Qt::LeftButton)
    {
        // 记录鼠标状态
        m_leftButtonPressed = false;
    }

    QWidget::mouseReleaseEvent(event);
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
    //设置标题
    case QEvent::WindowTitleChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            m_titleLabel->setText(pWidget->windowTitle());
            return true;
        }
        break;
    }
        //设置图标
    case QEvent::WindowIconChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            QIcon icon = pWidget->windowIcon();
            m_iconLabel->setPixmap(icon.pixmap(m_iconLabel->size()));
            return true;
        }
        break;
    }
        // 窗口状态变化、窗口大小变化
    case QEvent::WindowStateChange:
    case QEvent::Resize:
        updateMaximize();
        return true;
    }

    return QWidget::eventFilter(obj, event);
}

void TitleBar::resizeEvent(QResizeEvent *event)
{
    m_titleLabel->move((this->width()-m_titleLabel->width())/2,
                       (this->height()-m_titleLabel->height())/2);

    QWidget::resizeEvent(event);
}

void TitleBar::updateMaximize()
{
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            m_maximizeButton->setToolTip(tr("Restore"));
            m_maximizeButton->setProperty("maximizeProperty", "restore");
            m_maximizeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarNormalButton));
        }
        else
        {
            m_maximizeButton->setProperty("maximizeProperty", "maximize");
            m_maximizeButton->setToolTip(tr("Maximize"));
            m_maximizeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
        }

        m_maximizeButton->setStyle(QApplication::style());
    }
}

void TitleBar::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == m_minimizeButton)
        {
            pWindow->showMinimized();
        }
        else if (pButton == m_maximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton == m_closeButton)
        {
            pWindow->close();
        }
    }
}
