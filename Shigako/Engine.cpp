#include "Engine.h"
#include "EngineLayout.h"

#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif

/************************************************************************/
/* Core Engine                                                         */
/************************************************************************/
Engine::Engine(QWidget* parent)
    : QWidget(parent),
    m_drawArea(new DrawArea){
    m_imageArea = new QScrollArea;
    m_imageArea->setWidget(m_drawArea);

    EngineLayout *layout = new EngineLayout;
    layout->addWidget(m_imageArea, EngineLayout::Center);
//     layout->addWidget(createLabel("North"), EngineLayout::North);
//     layout->addWidget(createLabel("West"), EngineLayout::West);
//     layout->addWidget(createLabel("East 1"), EngineLayout::East);
//     layout->addWidget(createLabel("East 2"), EngineLayout::East);
//     layout->addWidget(createLabel("South"), EngineLayout::South);
    setLayout(layout);
}

Engine::~Engine(){

}


void Engine::openImage(const QString& filename){
    m_drawArea->openImage(filename);
}

bool Engine::saveImage(const QString& fileName, const char* fileFormat){
    return m_drawArea->saveImage(fileName, fileFormat);
}

bool Engine::isModified(){
    return m_drawArea->isModified();
}



/************************************************************************/
/* Draw Area                                                            */
/************************************************************************/

DrawArea::DrawArea(QWidget* parent) :
    QWidget(parent),
    m_image(500, 400, QImage::Format_RGB32),
    m_lastPoint(-1, -1){
    /* Empty */
    m_image.fill(qRgb(255, 255, 255));
    m_modified = false;
    m_paintColor = qRgb(0, 0, 0);
    m_brushes[0] = QBrush(Qt::SolidPattern);
}

DrawArea::~DrawArea(){

}

bool DrawArea::openImage(const QString &fileName){
    QImage image;
    if (!image.load(fileName))
        return false;

    setImage(image);
    return true;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    m_image = loadedImage;
    m_modified = false;
    update();
    return true;
}

void DrawArea::setImage(const QImage &image){

}

bool DrawArea::saveImage(const QString &fileName, const char *fileFormat){
    QImage visibleImage = m_image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        m_modified = false;
        return true;
    }
    else {
        return false;
    }
}

void DrawArea::setPenColor(const QColor &newColor){
    m_currentBrush->setColor(newColor);
}

void DrawArea::setPenWidth(int newWidth){
    m_paintWidth = newWidth;
}

void DrawArea::clearImage(){
    m_image.fill(qRgb(255, 255, 255));
    m_modified = true;
    update();
}

void DrawArea::print(){
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = m_image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(m_image.rect());
        painter.drawImage(0, 0, m_image);
    }
#endif // QT_NO_PRINTER
}

void DrawArea::mousePressEvent(QMouseEvent *event){
    //Left mouse button
    if (event->button() == Qt::LeftButton) {
        m_lastPoint = event->pos();
        m_scribbling = true;
        drawLineTo(event->pos());
    }

    //Middle moust button
    if (event->button() == Qt::MiddleButton){

    }
}

void DrawArea::mouseMoveEvent(QMouseEvent *event){
    if ((event->buttons() & Qt::LeftButton) && m_scribbling)
        drawLineTo(event->pos());
}

void DrawArea::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton && m_scribbling) {
        drawLineTo(event->pos());
        m_scribbling = false;
    }
}

void DrawArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, m_image, dirtyRect);
}

void DrawArea::resizeEvent(QResizeEvent *event){
    if (width() > m_image.width() || height() > m_image.height()) {
        int newWidth = qMax(width() + 128, m_image.width());
        int newHeight = qMax(height() + 128, m_image.height());
        resizeImage(&m_image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void DrawArea::drawLineTo(const QPoint &endPoint){
    m_painter.setPen(QPen(m_paintColor, m_paintWidth, Qt::SolidLine, Qt::FlatCap,
        Qt::RoundJoin));
    m_painter.drawLine(lastPoint, endPoint);
    m_modified = true;

    int rad = (m_paintWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
        .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void DrawArea::resizeImage(QImage *image, const QSize &newSize){
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(0, 0, 0));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

/************************************************************************/
/* IColorPicker                                                        */
/************************************************************************/

IColorPicker::IColorPicker(){

}

IColorPicker::~IColorPicker(){

}
