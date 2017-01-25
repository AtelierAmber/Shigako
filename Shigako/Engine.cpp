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

    m_layout = new EngineLayout;
    m_layout->addWidget(m_imageArea, EngineLayout::Center);
    m_layout->addWidget(m_adjustments, EngineLayout::East);
    m_layout->addWidget(m_colorPicker, EngineLayout::East);
    m_layout->addWidget(m_layers, EngineLayout::East);
    m_layout->addWidget(m_tools, EngineLayout::West);
    setLayout(m_layout);
}

Engine::~Engine(){
    delete m_layout;
    delete m_imageArea;
    delete m_colorPicker;
    delete m_adjustments;
    delete m_layers;
    delete m_tools;
}

void Engine::openImage(const QString& filename){
    m_drawArea->openImage(filename);
}

bool Engine::saveImage(const QString& fileName, const char* fileFormat){
    return m_drawArea->saveImage(fileName, fileFormat);
}

void Engine::newImage(const QSize& size, const QColor& color){
    m_drawArea->newImage(size, color);
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

void Engine::setDrawTool(const DrawTool& tool){
    m_drawArea->setDrawTool(tool);
}

/************************************************************************/
/* Draw Area                                                            */
/************************************************************************/

DrawArea::DrawArea(QWidget* parent) :
    QWidget(parent), m_lastPoint(-1, -1){
    newLayer(QSize(256, 256));
    m_layers[0].data()->fill(qRgba(255, 255, 255, 255));
    m_modified = false;
    createBrushes();
    m_currentBrush = &m_brushes[0];
    m_colors[0] = qRgba(255, 255, 255, 255);
    m_colors[1] = qRgba(0, 0, 0, 255);
    m_currentBrush->setColor(m_colors[0]);
    setFocus(Qt::ActiveWindowFocusReason);
    setFocusPolicy(Qt::StrongFocus);
    update();
}

DrawArea::~DrawArea(){

}

void DrawArea::createBrushes(){
    m_brushes.emplace_back(DrawTool::BRUSH);
    m_brushes.emplace_back(DrawTool::PENCIL);
}

bool DrawArea::openImage(const QString &fileName){
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;
    loadedImage.convertToFormat(QImage::Format_A2BGR30_Premultiplied);
    m_imageBackground = qRgba(0, 0, 0, 0);
    QSize newSize = loadedImage.size();
    m_layers.clear();
    newLayer(newSize);
    *(m_layers[0].data()) = loadedImage;
    m_modified = false;
    this->setGeometry(QRect(0, 0, newSize.width(), newSize.height()));
    update();
    return true;
}

bool DrawArea::saveImage(const QString &fileName, const char *fileFormat){
    QPainter p(m_layers[0].data());
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawRect(m_layers[0].data()->rect());
    for (int i = 0; i < m_layers.size(); ++i){
        if (!m_layers[i].isHidden()){
            p.drawImage(0, 0, *(m_layers[i].data()));
            p.end();
        }
    }
    QImage visibleImage = *(m_layers[0].data());

    if (visibleImage.save(fileName, fileFormat)) {
        m_modified = false;
        return true;
    }
    else {
        return false;
    }
}

void DrawArea::newImage(const QSize& imageSize, const QColor& color /*= qRgba(0, 0, 0, 0)*/){
    m_layers.clear();
    newLayer(imageSize);
    m_imageBackground = color;
    m_layers[0].data()->fill(color);
    m_modified = false;
    update();
    this->setGeometry(QRect(0, 0, imageSize.width(), imageSize.height()));
}

void DrawArea::setDrawTool(const DrawTool& tool){
    m_currentTool = tool;
    if ((int)tool < m_brushes.size()){
        m_currentBrush = &m_brushes[(int)tool];
    }
}

void DrawArea::clearImage(){
    QSize size = m_layers[0].data()->size();
    m_layers.clear();
    newLayer(size);
    m_layers[0].data()->fill(m_imageBackground);
    m_modified = true;
    update();
}

bool DrawArea::setLayer(unsigned int num){
    if (num < m_layers.size()){
        m_currentImage = num;
        return true;
    }
    return false;
}

void DrawArea::keyReleaseEvent(QKeyEvent *event){
    std::printf("\nKey: %i - %s pressed", event->key(), event->text().toStdString().c_str());
    switch (event->key()){
    case Qt::Key_X:
        m_backColor = !m_backColor;
        m_currentBrush->setColor(m_colors[(int)(m_backColor)]);
        break;
    default:
        QWidget::keyReleaseEvent(event);
        break;
    }
}

void DrawArea::mousePressEvent(QMouseEvent *event){
    //Left mouse button
    if (event->button() == Qt::LeftButton) {
        switch (m_currentTool){

        }
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

void DrawArea::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    for (int i = 0; i < m_layers.size(); ++i){
        if (!m_layers[i].isHidden()){
            painter.drawImage(0, 0, *(m_layers[i].data()));
        }
    }
}

void DrawArea::drawLineTo(const QPoint &endPoint){
    QPainter painter(m_layers[m_currentImage].data());
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

/************************************************************************/
/* Shigako Widget                                                       */
/************************************************************************/
ShigakoWidget::ShigakoWidget(QWidget *parent)
    : QWidget(parent), m_mousePos(0, 0){
    m_layout = new QGridLayout(this);
}

ShigakoWidget::~ShigakoWidget(){ /* Empty */}

ShigakoButton* ShigakoWidget::addButton(std::function<void()> CallFunction, Location location, QString filePath){
    std::printf("Added Button at %i,%i | %i,%i, a size of %i,%i | %i,%i and with image %s. Use this function's return to modify the button.\n", 
        location.col, location.row, location.x, location.y, location.cSpan, location.rSpan, 
        location.width, location.height, filePath.toStdString().c_str());
    ShigakoButton* newButton = new ShigakoButton();
    newButton->init(filePath, CallFunction, m_layout);
    if (location.layout){
        m_layout->addWidget(newButton, location.row, location.col, location.rSpan, location.cSpan);
    }
    else newButton->setGeometry(location.x, location.y, location.width, location.height);
    connect(newButton, SIGNAL(clicked()), newButton, SLOT(callf()));
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

ShigakoSlider* ShigakoWidget::addSlider(int min, int max, int* val, bool upDown, Location location, std::function<void()> CallFunction){
    std::printf("Added Slider at %i,%i | %i,%i, with min %i, max %i, and orientation %i. Use this function's return to modify the slider.\n",
        location.col, location.row, location.x, location.y, min, max, (int)upDown);
    ShigakoSlider* newSlider = new ShigakoSlider();
    newSlider->init(max, min, val, upDown, CallFunction);
    if (location.layout){
        m_layout->addWidget(newSlider, location.row, location.col, location.rSpan, location.cSpan);
    }
    else newSlider->setGeometry(location.x, location.y, location.width, location.height);
    return newSlider;
}