#include <QPainter>
#include <QDebug>
#include <QThreadPool>

#include "pixmapelement.h"
#include "loader.h"


PixmapOpenJob::PixmapOpenJob(PixmapElement * element):QRunnable(),m_element(element)
{
    Q_ASSERT(element);
}

void PixmapOpenJob::run()
{
    m_element->loadPixmap();
}


PixmapElement::PixmapElement(
        BaseElement *parent,
        BaseSpace * space,
        QRectF initRect
        ):
    BaseElement(parent,space,initRect),
    m_loading(false),
    m_loader(new Loader(this))
{

}


void PixmapElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPixmap pixmap = this->pixmap();
    painter->drawPixmap(this->rect(),pixmap,pixmap.rect());
}


void PixmapElement::startLoad()
{
    m_loading=true;
    m_loader->start();
}

void PixmapElement::finishLoad(bool)
{
    //m_geometry.setSize(pixmapNode->pixmap().rect().size());
    m_loader->stop();
    this->update();
    m_loading = false;
}

void PixmapElement::updateProgress(int progress)
{
    if (m_loader)
        m_loader->setProgress(progress);
}


void PixmapElement::load()
{
    PixmapOpenJob* job = new PixmapOpenJob(this);
    QThreadPool::globalInstance()->start(job);
}

void PixmapElement::loadPixmap()
{
    startLoad();
    updateProgress(0);

//    Document * doc =document();
//    QDir tempDir = doc->tempPath();
//    updateProgress(5);

//    QFileInfo originalFileInfo(m_originName);
//    QString fileSuffix = originalFileInfo.suffix();
//    QString tempFileName = tempDir.absoluteFilePath(
//                QString("%1.%2").arg(this->id()).arg(fileSuffix));
//    QFileInfo tempFileInfo(tempFileName);
//    if (!tempFileInfo.exists()){
//        QFile file(m_originName);
//        file.open(QIODevice::ReadOnly);
//        file.copy(tempFileName);
//    }
//    updateProgress(50);
//    QFile tempFile(tempFileName);
//    tempFile.open(QIODevice::ReadOnly);
//    updateProgress(80);

//    QByteArray data = tempFile.readAll();
//    char * fmt = fileSuffix.toLatin1().data();
//    this->m_pixmap.loadFromData(data,fmt);
//    this->setRect(QRectF(QPointF(0,0),this->m_pixmap.size()));
//    updateProgress(100);
//    finishLoad(true);
}
