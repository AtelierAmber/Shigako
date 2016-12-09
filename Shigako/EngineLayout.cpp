#include "EngineLayout.h"

EngineLayout::EngineLayout(QWidget *parent, int margin /* = 0 */, int spacing /* = -1 */){
    setMargin(margin);
    setSpacing(spacing);
}

EngineLayout::~EngineLayout(){
    QLayoutItem *l;
    while ((l = takeAt(0)))
        delete l;
}

void EngineLayout::addItem(QLayoutItem *item){
    add(item, West);
}

void EngineLayout::addWidget(QWidget *widget, Position position){
    add(new QWidgetItem(widget), position);
}

bool EngineLayout::hasHeightForWidth() const{
    return false;
}

int EngineLayout::count() const{
    return m_items.size();
}

QLayoutItem * EngineLayout::itemAt(int index) const{
    ItemWrapper *wrapper = m_items.value(index);
    if (wrapper)
        return wrapper->item;
    else
        return 0;
}

QSize EngineLayout::minimumSize() const{
    return calculateSize(MinimumSize);
}

void EngineLayout::setGeometry(const QRect &rect){
    ItemWrapper *center = 0;
    int eastWidth = 0;
    int westWidth = 0;
    int northHeight = 0;
    int southHeight = 0;
    int centerHeight = 0;
    int i;

    QLayout::setGeometry(rect);

    for (i = 0; i < m_items.size(); ++i) {
        ItemWrapper* wrapper = m_items.at(i);
        QLayoutItem* item = wrapper->item;
        Position position = wrapper->position;

        if (position == North) {
            item->setGeometry(QRect(rect.x(), northHeight, rect.width(),
                item->sizeHint().height()));

            northHeight += item->geometry().height() + spacing();
        }
        else if (position == South) {
            item->setGeometry(QRect(item->geometry().x(),
                item->geometry().y(), rect.width(),
                item->sizeHint().height()));

            southHeight += item->geometry().height() + spacing();

            item->setGeometry(QRect(rect.x(),
                rect.y() + rect.height() - southHeight + spacing(),
                item->geometry().width(),
                item->geometry().height()));
        }
        else if (position == Center) {
            center = wrapper;
        }
    }

    centerHeight = rect.height() - northHeight - southHeight;

    for (i = 0; i < m_items.size(); ++i) {
        ItemWrapper *wrapper = m_items.at(i);
        QLayoutItem *item = wrapper->item;
        Position position = wrapper->position;

        if (position == West) {
            item->setGeometry(QRect(rect.x() + westWidth, northHeight,
                item->sizeHint().width(), centerHeight));

            westWidth += item->geometry().width() + spacing();
        }
        else if (position == East) {
            item->setGeometry(QRect(item->geometry().x(), item->geometry().y(),
                item->sizeHint().width(), centerHeight));

            eastWidth += item->geometry().width() + spacing();

            item->setGeometry(QRect(
                rect.x() + rect.width() - eastWidth + spacing(),
                northHeight, item->geometry().width(),
                item->geometry().height()));
        }
    }

    if (center)
        center->item->setGeometry(QRect(westWidth, northHeight,
        rect.width() - eastWidth - westWidth,
        centerHeight));
}

QSize EngineLayout::sizeHint() const{
    return calculateSize(SizeHint);
}

QLayoutItem * EngineLayout::takeAt(int index){
    if (index >= 0 && index < m_items.size()) {
        ItemWrapper *layoutStruct = m_items.takeAt(index);
        return layoutStruct->item;
    }
    return 0;
}

void EngineLayout::add(QLayoutItem *item, Position position){
    m_items.append(new ItemWrapper(item, position));
}

QSize EngineLayout::calculateSize(SizeType sizeType) const{
    QSize totalSize;

    for (int i = 0; i < m_items.size(); ++i) {
        ItemWrapper *wrapper = m_items.at(i);
        Position position = wrapper->position;
        QSize itemSize;

        if (sizeType == MinimumSize)
            itemSize = wrapper->item->minimumSize();
        else // (sizeType == SizeHint)
            itemSize = wrapper->item->sizeHint();

        if (position == North || position == South || position == Center)
            totalSize.rheight() += itemSize.height();

        if (position == West || position == East || position == Center)
            totalSize.rwidth() += itemSize.width();
    }
    return totalSize;
}