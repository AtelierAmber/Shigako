#include "Engine.h"
#include "EngineLayout.h"
#include "Layers.h"
#include "ColorPicker.h"
#include "Adjustments.h"
#include "Tools.h"

#include <QtWidgets>

/************************************************************************/
/* Core Engine                                                         */
/************************************************************************/
Engine::Engine(QWidget* parent)
    : QWidget(parent){
    m_imageArea = new QScrollArea;
    m_drawArea = new DrawArea(m_imageArea);
    m_imageArea->setBackgroundRole(QPalette::Dark);
    m_imageArea->setWidget(m_drawArea);
    m_imageArea->setVisible(true);
    m_drawArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    m_colorPicker = new ColorPicker(this);
    m_adjustments = new Adjustments(this);
    m_layers = new Layers(this);
    m_tools = new Tools(this);

    EngineLayout *layout = new EngineLayout;
    layout->addWidget(m_imageArea, EngineLayout::Center);
    layout->addWidget(m_adjustments, EngineLayout::East);
    layout->addWidget(m_colorPicker, EngineLayout::East);
    layout->addWidget(m_layers, EngineLayout::East);
    layout->addWidget(m_tools, EngineLayout::West);
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

void Engine::clearImage(){
    m_drawArea->clearImage();
}

bool Engine::isModified(){
    return m_drawArea->isModified();
}

QColor Engine::penColor(){
    return m_drawArea->penColor();
}

void Engine::setPenColor(const QColor& color){
    m_drawArea->setPenColor(color);
}

int Engine::penWidth(){
    return m_drawArea->penWidth();
}

void Engine::setPenWidth(int width){
    m_drawArea->setPenWidth(width);
}

/************************************************************************/
/* Draw Area                                                            */
/************************************************************************/

DrawArea::DrawArea(QWidget* parent) :
    QWidget(parent), m_image(500, 400, QImage::Format_ARGB32), m_lastPoint(-1, -1){
    m_image.fill(qRgb(255, 2, 255));
    m_modified = false;
    m_brushes.emplace_back(DrawTool::PENCIL);
    m_currentBrush = &m_brushes[0];
    m_colors[0] = qRgba(255, 255, 255, 255);
    m_colors[1] = qRgba(0, 0, 0, 255);
    m_currentBrush->setColor(m_colors[0]);
    setFocus(Qt::ActiveWindowFocusReason);
    setFocusPolicy(Qt::StrongFocus);
}

DrawArea::~DrawArea(){

}

bool DrawArea::openImage(const QString &fileName){
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    m_image = loadedImage;
    m_modified = false;
    resize(m_image.size());
    update();
    return true;
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
    m_currentBrush->setWidth(newWidth);
}

void DrawArea::clearImage(){
    m_image.fill(qRgb(255, 255, 255));
    m_modified = true;
    update();
}

void DrawArea::keyReleaseEvent(QKeyEvent *event){
    std::printf("\nKey: %i - %s pressed", event->key(), event->text().toStdString().c_str());
    switch (event->key()){
    case Qt::Key_X:
        m_backColor = !m_backColor;
        m_currentBrush->setColor(m_colors[(int)(m_backColor != 0)]);
        break;
    default:
        QWidget::keyReleaseEvent(event);
        break;
    }
}

void DrawArea::mousePressEvent(QMouseEvent *event){
    //Left mouse button
    if (event->button() == Qt::LeftButton) {
        m_lastPoint = event->pos();
        m_drawing = true;
        drawLineTo(event->pos());
    }

    //Middle mouse button
    if (event->button() == Qt::MiddleButton){

    }
}

void DrawArea::mouseMoveEvent(QMouseEvent *event){
    if ((event->buttons() & Qt::LeftButton) && m_drawing)
        drawLineTo(event->pos());
}

void DrawArea::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton && m_drawing) {
        drawLineTo(event->pos());
        m_drawing = false;
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
    QPainter painter(&m_image);
    painter.setPen(m_currentBrush->Pen());
    if (m_lastPoint == endPoint){
        painter.drawPoint(endPoint);
    }
    painter.drawLine(m_lastPoint, endPoint);
    m_modified = true;

    int rad = (m_currentBrush->getWidth() / 2) + 2;
    update(QRect(m_lastPoint, endPoint).normalized()
        .adjusted(-rad, -rad, +rad, +rad));
    m_lastPoint = endPoint;
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
/* Shigako Widget                                                       */
/************************************************************************/
ShigakoWidget::ShigakoWidget(QWidget *parent)
    : QWidget(parent){
    m_layout = new QGridLayout(this);
}

ShigakoWidget::~ShigakoWidget(){ /* Empty */}

ShigakoButton* ShigakoWidget::addButton(std::function<void()> CallFunction, Location location, QString filePath){
    std::printf("Added Button at %i,%i | %i,%i, a size of %i,%i | %i,%i and with image %s. Use this function's return to modify the button.\n", 
        location.col, location.row, location.x, location.y, location.cSpan, location.rSpan, 
        location.width, location.height, filePath.toStdString().c_str());
    ShigakoButton* newButton = new ShigakoButton();
    newButton->init(filePath, CallFunction);
    if (location.layout){
        m_layout->addWidget(newButton, location.row, location.col, location.rSpan, location.cSpan);
    }
    else newButton->setGeometry(location.x, location.y, location.width, location.height);
    return newButton;
}
ShigakoLabel* ShigakoWidget::addLabel(QString label, Location location){
    std::printf("Added Label at %i,%i | %i,%i, with label %s. Use this function's return to modify the label.\n", 
        location.col, location.row, location.x, location.y, label.toStdString().c_str());
    ShigakoLabel* newLabel = new ShigakoLabel();
    newLabel->init(label);
    if (location.layout){
        m_layout->addWidget(newLabel, location.row, location.col, location.rSpan, location.cSpan);
    }
    else newLabel->setGeometry(location.x, location.y, location.width, location.height);
    return newLabel;
}
ShigakoImage* ShigakoWidget::addImage(QString filePath, Location location){
    std::printf("Added Image at %i,%i | %i,%i, with image %s. Use this function's return to modify the image.\n", 
        location.col, location.row, location.x, location.y, filePath.toStdString().c_str());
    ShigakoImage* newImage = new ShigakoImage();
    newImage->init(filePath);
    if (location.layout){
        m_layout->addWidget(newImage, location.row, location.col, location.rSpan, location.cSpan);
    }
    else newImage->setGeometry(location.x, location.y, location.width, location.height);
    return newImage;
}