#pragma once
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QSlider>
#include <QWidget>
#include <QLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QPainter>

#include <functional>
#include <vector>
#include <cstdio>

class DrawArea;
class Adjustments;
class Layers;
class ColorPicker;
class Tools;
class EngineLayout;
class ShigakoLayer;

enum class DrawTool{
    BRUSH = 0,
    PENCIL = 1,
    ERASER = 2,
    SELECT,
    TEXT,
    FILL
};

class Engine : 
    public QWidget{
    Q_OBJECT

public:
    Engine(QWidget* parent = 0);
    ~Engine();

    void openImage(const QString& filename);
    bool saveImage(const QString& fileName, const char* fileFormat);
    void newImage(const QSize& size, const QColor& color);
    void clearImage();

    bool isModified();
    QColor penColor();
    void setPenColor(const QColor& color);
    int penWidth();
    void setPenWidth(int width);
    void setDrawTool(const DrawTool& tool);
    int                         newLayer(const QSize& size);
    std::vector<ShigakoLayer>*  getLayers();
    bool                        setLayer(unsigned int num);
    void                        toggleLayer(unsigned int num);

private:
    
    EngineLayout* m_layout;
    DrawArea* m_drawArea;
    Adjustments* m_adjustments;
    ColorPicker* m_colorPicker;
    Layers* m_layers;
    Tools* m_tools;
    QScrollArea* m_imageArea;
};

struct DrawBrush{
    DrawBrush(QBrush b, int width = 5) : m_brush(b){
        m_pen = QPen(m_brush, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    DrawBrush(QPen p) : m_pen(p) {}
    DrawBrush(DrawTool tool, int width = 5, Qt::PenStyle style = Qt::SolidLine, Qt::PenCapStyle cap = Qt::RoundCap, Qt::PenJoinStyle join = Qt::RoundJoin) {
        m_pen = QPen();
        m_drawTool = tool;
        m_pen.setStyle(style);
        m_pen.setCapStyle(cap);
        m_pen.setJoinStyle(join);
        m_pen.setColor(qRgba(255, 255, 255, 255));
        m_pen.setWidth(width);
        m_pen.setBrush(Qt::SolidPattern);
    }

    void        setBrush(const QBrush& brush){ m_brush = brush; m_pen.setBrush(brush); }
    void        setColor(const QColor& color){ m_pen.setColor(color); }
    const       QColor& getColor() const { return m_pen.color(); }
    void        setStyle(const Qt::PenStyle& style){ m_pen.setStyle(style); }
    void        setCap(const Qt::PenCapStyle cap){ m_pen.setCapStyle(cap); }
    void        setJoin(const Qt::PenJoinStyle& join){ m_pen.setJoinStyle(join); }
    void        setWidth(int width){ m_pen.setWidth(width); }
    int         getWidth(){ return m_pen.width(); }
    void        setToolType(DrawTool tool){ m_drawTool = tool; }
    DrawTool    getToolType(){ return m_drawTool; }
    QPen        Pen(){ return m_pen; }

protected:
    DrawTool m_drawTool;
    QBrush m_brush;
    QPen m_pen;
};

class ShigakoLayer{
public:
    ShigakoLayer(){ }
    ShigakoLayer(const QSize& size, QImage::Format iFormat) :
        m_image(size, iFormat){ }

    QImage* data(){ return &m_image; }
    bool isHidden() const { return m_hidden; }
    void toggle(){ m_hidden = !m_hidden; }

private:
    bool m_hidden = false;
    QImage m_image;
};

class DrawArea :
    public QWidget{
    Q_OBJECT

public:
    DrawArea(QWidget* parent = 0);
    QImage checkers(int size);
    ~DrawArea();

    bool        openImage(const QString &fileName);
    bool        saveImage(const QString &fileName, const char *fileFormat);
    void        newImage(const QSize& size, const QColor& color = qRgba(0, 0, 0, 0));

    void        setPenColor(const QColor &newColor){ m_colors[0] = newColor; m_currentBrush->setColor(newColor); }
    void        setPenWidth(int newWidth){ m_currentBrush->setWidth(newWidth); }
    void        setDrawTool(const DrawTool& tool);

    bool        isModified() const { return m_modified; }
    QColor      penColor() const { return m_currentBrush->getColor(); }
    DrawBrush*  brush() const { return m_currentBrush; }
    int         penWidth() const { return m_currentBrush->getWidth(); }
    void        createBrushes();

    int                         newLayer(const QSize& size){ m_layers.emplace_back(size, QImage::Format_A2BGR30_Premultiplied); return m_layers.size() - 1; }
    std::vector<ShigakoLayer>*  getLayers(){ return &m_layers; }
    bool                        setLayer(unsigned int num);
    void                        toggleLayer(unsigned int num){ m_layers[num].toggle(); }

    public slots:
    void        clearImage();

protected:
    void        keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void        mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void        mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void        mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void        paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void        drawLineTo(const QPoint &endPoint);
    void        recursiveFill(const QPoint& curPoint, const QColor& targetColor, const QColor& fillColor, int itr);

    //Brush ID, Brush
    std::vector<DrawBrush> m_brushes;
    bool m_modified = false;
    bool m_drawing = false;
    QColor m_colors[2];
    unsigned int m_currentImage = 0;
    std::vector<ShigakoLayer> m_layers;
    DrawTool m_currentTool;
    DrawBrush* m_currentBrush;
    QPoint m_lastPoint;
};

struct vec2{
    vec2(int X, int Y) : x(X), y(Y){}
    int x, y;
};

struct Location{
    Location(int Col, int Row, int RSpan = 1, int CSpan = 1) : 
        col(Col), row(Row), rSpan(RSpan), cSpan(CSpan), layout(true){ /* Empty */ }
    Location(int X, int Y, int Width, int Height, void*) : 
        x(X), y(Y), width(Width), height(Height), layout(false){ /* Empty */ }
    int col = -1, row = -1, rSpan = 0, cSpan = 0;
    int x = -1, y = -1, width = 0, height = 0;
    bool layout = false;
};

class ShigakoButton :
    public QPushButton{
    Q_OBJECT
public:
    ShigakoButton(){ /* Empty */ }
    void init(QString filePath, std::function<void()> CallFunction, QGridLayout* layoutParent){
        std::printf("%s", (this->setButtonIcon(filePath)) ? "+\n" : "Failed to load icon" + filePath + "! Is it there?\n");
        m_callFunc = CallFunction;
        m_layoutParent = layoutParent;
    }

    bool setButtonIcon(QString filePath){ 
        QIcon icon;
        QPixmap iconP;
        if (!iconP.load(filePath)){
            return false;
        }
        icon.addPixmap(iconP);
        this->setIcon(icon);
        std::printf("+\n");
        return true;
    }
    void setCall(std::function<void()> callFunc){
        m_callFunc = callFunc;
        
    }

    int ID() const { return m_ID; }
    void setID(int id){ m_ID = id; }

public slots:
    void callf(){ m_callFunc(); }
private:
    std::function<void()> m_callFunc = nullptr;
    QGridLayout* m_layoutParent;
    int m_ID = -1;
};

class ShigakoLabel :
    public QLabel{
    Q_OBJECT
public:
    ShigakoLabel(){ /* Empty */ }
    void init(QString label){
        this->setText(label);
        std::printf("+\n");
    }
};

class ShigakoImage :
    public QLabel{
    Q_OBJECT
public:
    ShigakoImage(){ /* Empty */ }
    void init(QString filePath){
        if (!m_pic.load(filePath)){
            std::printf("Failed to load %s! Is it there?\n", filePath.toStdString().c_str());
        }
        else {
            this->setPixmap(m_pic);
            std::printf("+\n");
        }
    }

    void setColor(const QColor& color){
        m_pic.fill(color);
        this->setPixmap(m_pic);
    }

private:
    QPixmap m_pic;
};

class ShigakoSlider :
    public QSlider{
    Q_OBJECT
public:
    ShigakoSlider(){ /* Empty */ }
    void init(int max, int min, int* val, bool upDown, std::function<void()> CallFunction){
        this->setMinimum(min);
        this->setMaximum(max);
        this->setTickInterval(1);
        this->setOrientation((upDown) ? Qt::Vertical : Qt::Horizontal);
        this->setFocusPolicy(Qt::StrongFocus);
        this->setTickPosition(QSlider::NoTicks);
        valueDef = val;
        connect(this, SIGNAL(valueChanged(int)), this, SLOT(editValue(int)));
        m_callF = CallFunction;
    }

public slots:
    void editValue(int val){
        *valueDef = val;
        m_callF();
    }

private:
    int* valueDef;
    std::function<void()> m_callF;;
};

class ShigakoWidget :
    public QWidget{
    Q_OBJECT
public:
    ShigakoWidget(QWidget *parent = 0);
    ~ShigakoWidget();
protected:
    ShigakoButton* addButton(std::function<void()> CallFunction, Location location, QString filePath = "NULL");
    ShigakoLabel* addLabel(QString label, Location location);
    ShigakoImage* addImage(QString filePath, Location location);
    ShigakoSlider* addSlider(int min, int max, int* val, bool upDown, Location location, std::function<void()> CallFunction);
    
    void ButtonCall();

    QGridLayout* m_layout;
    vec2 m_mousePos;
};