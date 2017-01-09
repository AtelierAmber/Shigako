#pragma once
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QPainter>
#include <QLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>

#include <functional>
#include <map>
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

    bool isModified();

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
    operator QPen() const { return QPen(brush, 0, style, capStyle, joinStyle); }
    operator QBrush() const { return brush; }
    DrawBrush(){}
    DrawBrush(QBrush b) : brush(b), style(Qt::SolidLine), capStyle(Qt::RoundCap), joinStyle(Qt::MPenJoinStyle) 
    {/* Empty */ }

    void setColor(const QColor& color){ brush.setColor(color); }

protected:
    QBrush brush;
    Qt::PenStyle style;
    Qt::PenCapStyle capStyle;
    Qt::PenJoinStyle joinStyle;
};

class DrawArea :
    public QWidget{
    Q_OBJECT

public:
    DrawArea(QWidget* parent = 0);
    ~DrawArea();

    bool openImage(const QString &fileName);
    void setImage(const QImage &image);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return m_modified; }
    QColor paintColor() const { return m_paintColor; }
    DrawBrush* brush() const { return m_currentBrush; }
    int paintWidth() const { return m_paintWidth; }

    public slots:
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    //Brush ID, Brush
    std::map<int, DrawBrush> m_brushes;

    bool m_modified;
    bool m_scribbling;
    int m_paintWidth;
    QColor m_paintColor;
    QImage m_image;
    QPoint lastPoint;
    QPainter m_painter;
    DrawTool m_currentTool;
    DrawBrush* m_currentBrush;
    QPoint m_lastPoint;
};

struct vec2{
    vec2(int X, int Y) : x(X), y(Y){}
    int x, y;
};

typedef unsigned int LocationID;
namespace Location{
    enum{
        TOP = 0x1,
        BOTTOM = 0x2,
        LEFT = 0x4,
        RIGHT = 0x8,
        MIDDLE = 0x10,
    };
};

class ShigakoButton :
    public QPushButton{
    Q_OBJECT
public:
    void init(QString filePath, std::function<void()> CallFunction, vec2 size, LocationID location){}

    bool setIcon(QString fileName){}
    void setLocation(LocationID location){}
    void setCall(std::function<void()> callFunc){}
    void setSize(int w, int h){}
private:
    std::function<void()> callFunc = nullptr;
};

class ShigakoLabel :
    public QLabel{
    Q_OBJECT
public:
    void init(QString label, LocationID location){}
};

class ShigakoImage :
    public QLabel{
    Q_OBJECT
public:
    void init(QString filePath, LocationID location){}
};

class ShigakoWidget :
    public QWidget{
    Q_OBJECT
public:
    ShigakoWidget(QWidget *parent = 0);
    ~ShigakoWidget();
protected:
    ShigakoButton addButton(std::function<void()> CallFunction, vec2 size, LocationID location, QString filePath = "NULL");
    ShigakoLabel addLabel(QString label, LocationID location);
    ShigakoImage addImage(QString filePath, LocationID location);
};