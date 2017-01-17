#pragma once
#include <QColor>
#include <QImage>
#include <QPoint>
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

class Engine : 
    public QWidget{
    Q_OBJECT

public:
    Engine(QWidget* parent = 0);
    ~Engine();

    void openImage(const QString& filename);
    bool saveImage(const QString& fileName, const char* fileFormat);
    void clearImage();

    bool isModified();
    QColor penColor();
    void setPenColor(const QColor& color);
    int penWidth();
    void setPenWidth(int width);

private:
    void setupWidgets();
    
    DrawArea* m_drawArea;
    Adjustments* m_adjustments;
    Layers* m_layers;
    ColorPicker* m_colorPicker;
    Tools* m_tools;
    QScrollArea* m_imageArea;
};

enum DrawTool{
    BRUSH,
    SELECT,
    PENCIL,
    TEXT,
    FILL
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

class DrawArea :
    public QWidget{
    Q_OBJECT

public:
    DrawArea(QWidget* parent = 0);
    ~DrawArea();

    bool        openImage(const QString &fileName);
    bool        saveImage(const QString &fileName, const char *fileFormat);
    void        setPenColor(const QColor &newColor);
    void        setPenWidth(int newWidth);

    bool        isModified() const { return m_modified; }
    QColor      penColor() const { return m_currentBrush->getColor(); }
    DrawBrush*  brush() const { return m_currentBrush; }
    int         penWidth() const { return m_currentBrush->getWidth(); }

    public slots:
    void        clearImage();

protected:
    void        keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void        mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void        mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void        mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void        paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void        resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private:
    void        drawLineTo(const QPoint &endPoint);
    void        resizeImage(QImage *image, const QSize &newSize);

    //Brush ID, Brush
    std::vector<DrawBrush> m_brushes;

    bool m_modified = false;
    bool m_drawing = false;
    bool m_backColor = false;
    QColor m_colors[2];
    QImage m_image;
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
    void init(QString filePath, std::function<void()> CallFunction){
        
    }

    bool setIcon(QString fileName){ return true; }
    void setLocation(Location location){}
    void setCall(std::function<void()> callFunc){}
    void setSize(int w, int h){}

    void callf(){ callFunc(); }
private:
    std::function<void()> callFunc = nullptr;
};

class ShigakoLabel :
    public QLabel{
    Q_OBJECT
public:
    void init(QString label){
        this->setText(label);
    }
};

class ShigakoImage :
    public QLabel{
    Q_OBJECT
public:
    void init(QString filePath){
        QPixmap pic;
        if (!pic.load(filePath)){
            std::printf("Failed to load %s! Is it there?", filePath.toStdString().c_str());
        }else this->setPixmap(QPixmap(filePath));
    }
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

    QGridLayout* m_layout;
};