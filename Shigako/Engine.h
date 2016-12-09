#pragma once

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QPainter>
#include <QLayout>

#include <map>

class DrawArea;

class Engine : 
    public QWidget{
    Q_OBJECT

public:
    Engine(QWidget* parent = 0);
    ~Engine();

    void openImage(QString){}

    bool isModified();

    private slots:


private:
    void setupWidgets();
    
    DrawArea* m_drawArea;
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
    DrawArea(QColor paintColor, QImage image, int brushID);
    DrawArea(){/* Empty */ }
    ~DrawArea();

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    void init();
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