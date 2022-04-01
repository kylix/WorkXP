#ifndef DOCKPANEL_H
#define DOCKPANEL_H

#include <QDockWidget>
#include <QImage>

class QMainWindow;

class DockPanel : public QDockWidget
{
Q_OBJECT
public:
    explicit DockPanel(QMainWindow *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

protected:
    //void resizeEvent(QResizeEvent *e) override;
};

class DockPanelTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit DockPanelTitleBar(QWidget * parent = nullptr);

    QSize sizeHint() const override { return minimumSizeHint(); }

    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent * event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


private:
    QPixmap m_closePm;

    bool m_expanded;
};

#endif // DOCKPANEL_H
