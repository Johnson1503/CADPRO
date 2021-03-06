#include "scene.h"
#include <QDebug>
#include <QPainter>

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent),
    name(""),
    curShape(Shape::None),
    modified(false),
    drawable(true),
    moveable(false),
    drawing(false),
    penWidth(1),
    scaleFactor(1)
{
    setSceneRect(SHRT_MIN, SHRT_MIN, SHRT_MAX * 2, SHRT_MAX * 2);
}

void Scene::setName(QString name)
{
    this->name = name;
}

QString Scene::getName()
{
    return this->name;
}

void Scene::setCurShape(Shape::ShapeType curShape)
{  
    this->setDrawable(true);
    this->setMoveable(false);
    this->curShape = curShape;
}

Shape::ShapeType Scene::getCurShape()
{
    return this->curShape;
}

int Scene::getitemListLength() const
{
    return this->itemList.length();
}

void Scene::setModified(bool modified)
{
    this->modified = modified;
}

bool Scene::isModified() const
{
    return this->modified;
}

void Scene::setMoveable(bool moveable)
{
    this->moveable = moveable;
}

bool Scene::isMoveable() const
{
    return this->moveable;
}

void Scene::setDrawable(bool flag)
{
    this->drawable = flag;
}

void Scene::setEntityStyle(Configure::EntityStyle eStyle)
{
    this->eStyle = eStyle;
}

Configure::EntityStyle Scene::getEntityStyle()
{
    return this->eStyle;
}

void Scene::setAxesGrid(Configure::AxesGrid axesGrid)
{
    this->axesGrid = axesGrid;
}

void Scene::addCustomLineItem(Line *line)
{
    line->setShapeId(getitemListLength()+1);
    addItem(line);
    itemList.append(line);
    connect(line, &Shape::sceneMoveableChanged, line, &Line::onSceneMoveableChanged);
}

void Scene::addCustomRectItem(Rect *rect)
{
    rect->setShapeId(getitemListLength()+1);
    addItem(rect);
    itemList.append(rect);
    connect(rect, &Shape::sceneMoveableChanged, rect, &Rect::onSceneMoveableChanged);
}


int Scene::getPolygon_type() const
{
    return polygon_type;
}

void Scene::setPolygon_type(int value)
{
    polygon_type = value;
}

int Scene::getPolygon_line_num() const
{
    return polygon_line_num;
}

void Scene::setPolygon_line_num(int value)
{
    polygon_line_num = value;
}

double Scene::getPolygon_radius() const
{
    return polygon_radius;
}

void Scene::setPolygon_radius(double value)
{
    polygon_radius = value;
}

qreal Scene::getPolygon_alpha() const
{
    return polygon_alpha;
}

void Scene::setPolygon_alpha(const qreal &value)
{
    polygon_alpha = value;
}

double Scene::getTrapezium_H() const
{
    return trapezium_H;
}

void Scene::setTrapezium_H(double value)
{
    trapezium_H = value;
}

double Scene::getTrapezium_toplength() const
{
    return trapezium_toplength;
}

void Scene::setTrapezium_toplength(double value)
{
    trapezium_toplength = value;
}

double Scene::getTrapezium_alpha1() const
{
    return trapezium_alpha1;
}

void Scene::setTrapezium_alpha1(double value)
{
    trapezium_alpha1 = value;
}

double Scene::getTrapezium_alpha2() const
{
    return trapezium_alpha2;
}

void Scene::setTrapezium_alpha2(double value)
{
    trapezium_alpha2 = value;
}

int Scene::getTrapezium_type() const
{
    return trapezium_type;
}

void Scene::setTrapezium_type(int value)
{
    trapezium_type = value;
}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 左键可控制 画图、选择、移动、
    if(event->button() == Qt::LeftButton){
        // 可画标识为真，表示目前为可画，而且是新添图形
        if(drawable){
            if(drawing) {
                // 对于需要不止两个条件就可确定的图形，
                // 还需要继续进行执行drawing函数
                if(curItem && curItem->updateFlag(event)) {
                    drawing = false;
                    qDebug() << "结束绘图";
                }
            } else {
                int id = itemList.length();
                switch(curShape) {
                case Shape::None:
                {
                    curItem = NULL;
                    break;
                }
                case Shape::Point:{
                    Point *point = new Point;
                    point->setShapeId(id+1);
                    QPen pen = QPen();
                    pen.setColor(eStyle.referPoint.color);
                    pen.setStyle(Qt::SolidLine);
                    pen.setWidthF(1);
                    point->setPen(pen);
                    point->setOffset(eStyle.referPoint.sizeInPix);
                    point->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    point->setSelectStyle(eStyle.selectedEntity);
                    curItem = point;
                    addItem(point);
                    connect(point, &Shape::sceneMoveableChanged, point, &Point::onSceneMoveableChanged);
                    connect(point, &Point::select, this, &Scene::onPointSelected);
                    break;
                }
                case Shape::Line:
                {
                    Line *line = new Line;
                    line->setShapeId(id+1);
                    line->setPenStyle(eStyle.perimeterLine);
                    line->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    line->setSelectStyle(eStyle.selectedEntity);
                    curItem = line;
                    addItem(line);
                    connect(line, &Shape::sceneMoveableChanged, line, &Line::onSceneMoveableChanged);
                    connect(line, &Line::select, this, &Scene::onLineSelected);
                    break;
                }
                case Shape::MiddleAxis:
                {
                    Line *line = new Line;
                    line->setShapeType(Shape::MiddleAxis);
                    line->setShapeId(id+1);
                    line->setPenStyle(eStyle.middleAxis);
                    line->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    line->setSelectStyle(eStyle.selectedEntity);
                    curItem = line;
                    addItem(line);
                    connect(line, &Shape::sceneMoveableChanged, line, &Line::onSceneMoveableChanged);
                    connect(line, &Line::select, this, &Scene::onLineSelected);
                    break;
                }
                case Shape::Direction:
                {
                    Line *line = new Line;
                    line->setShapeType(Shape::Direction);
                    line->setShapeId(id+1);
                    line->setPenStyle(eStyle.middleAxis);
                    line->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    line->setSelectStyle(eStyle.selectedEntity);
                    curItem = line;
                    addItem(line);
                    connect(line, &Shape::sceneMoveableChanged, line, &Line::onSceneMoveableChanged);
                    connect(line, &Line::select, this, &Scene::onLineSelected);
                    break;
                }
                case Shape::PolyLine:
                {
                    PolyLine *polyLine = new PolyLine;
                    polyLine->setType(PolyLine::cubic);
                    polyLine->setShapeId(id+1);
                    polyLine->setPenStyle(eStyle.perimeterLine);
                    polyLine->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    polyLine->setSelectStyle(eStyle.selectedEntity);
                    curItem = polyLine;
                    addItem(polyLine);
                    connect(polyLine, &Shape::sceneMoveableChanged, polyLine, &PolyLine::onSceneMoveableChanged);
                    connect(polyLine, &PolyLine::select, this, &Scene::onPolyLineSelected);
                    break;
                }
                case Shape::Rectangle:
                {
                    Rect *rect = new Rect;
                    rect->setShapeId(id+1);
                    rect->setPenStyle(eStyle.cut);
                    rect->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    rect->setSelectStyle(eStyle.selectedEntity);
                    curItem = rect;
                    addItem(rect);
                    connect(rect, &Shape::sceneMoveableChanged, rect, &Rect::onSceneMoveableChanged);
                    connect(rect, &Rect::select, this, &Scene::onRectSelected);
                    break;
                }
                case Shape::Ellipse:
                {
                    Ellipse *ellipse = new Ellipse;
                    ellipse->setShapeId(id+1);
                    ellipse->setPenStyle(eStyle.cut);
                    ellipse->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    ellipse->setSelectStyle(eStyle.selectedEntity);
                    curItem = ellipse;
                    addItem(ellipse);
                    connect(ellipse, &Shape::sceneMoveableChanged, ellipse, &Ellipse::onSceneMoveableChanged);
                    connect(ellipse, &Ellipse::select, this, &Scene::onEllipseSelected);
                    break;
                }
                case Shape::Circle:
                {
                    Circle *circle = new Circle;
                    circle->setShapeId(id+1);
                    circle->setFilled(false);
                    circle->setPenStyle(eStyle.cut);
                    circle->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    circle->setSelectStyle(eStyle.selectedEntity);
                    curItem = circle;
                    addItem(circle);
                    connect(circle, &Shape::sceneMoveableChanged, circle, &Circle::onSceneMoveableChanged);
                    connect(circle, &Circle::select, this, &Scene::onCircleSelected);
                    break;
                }
                case Shape::Hole:
                {
                    Circle *circle = new Circle;
                    circle->setShapeId(id+1);
                    circle->setFilled(true);
                    circle->setPenStyle(eStyle.cut);
                    circle->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    circle->setSelectStyle(eStyle.selectedEntity);
                    curItem = circle;
                    addItem(circle);
                    connect(circle, &Shape::sceneMoveableChanged, circle, &Circle::onSceneMoveableChanged);
                    connect(circle, &Circle::select, this, &Scene::onCircleSelected);
                    break;
                }
                case Shape::Arc:
                {
                    Arc *arc = new Arc;
                    arc->setShapeId(id+1);
                    arc->setPenStyle(eStyle.cut);
                    arc->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    arc->setSelectStyle(eStyle.selectedEntity);
                    curItem = arc;
                    addItem(arc);
                    connect(arc, &Shape::sceneMoveableChanged, arc, &Arc::onSceneMoveableChanged);
                    connect(arc, &Arc::select, this, &Scene::onArcSelected);
                    break;
                }
                case Shape::Arc2:
                {
                    Arc *arc = new Arc;
                    arc->setType(Arc::updated);
                    arc->setShapeId(id+1);
                    arc->setPenStyle(eStyle.cut);
                    arc->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    arc->setSelectStyle(eStyle.selectedEntity);
                    curItem = arc;
                    addItem(arc);
                    connect(arc, &Shape::sceneMoveableChanged, arc, &Arc::onSceneMoveableChanged);
                    connect(arc, &Arc::select, this, &Scene::onArcSelected);
                    break;
                }

                    case Shape::Polygon:
                    {
                        Polygon *polygon = new Polygon;

                        polygon->setLine_num(this->getPolygon_line_num());
                        polygon->setRadius(this->getPolygon_radius());
                        polygon->setAlpha(this->getPolygon_alpha());
                        polygon->setType(this->getPolygon_type());

                       /* Polygon_dialog *polygon_dialog = new Polygon_dialog;//正多边形绘制的对话框
                        polygon_dialog->exec();
                        //获得对话框中传递的数据
                        polygon->setLine_num(polygon_dialog->getLen_num());
                        polygon->setRadius(polygon_dialog->getRaduii());
                        polygon->setAlpha(polygon_dialog->getAngle());
                        */
                        polygon->setShapeId(id+1);
                        switch(this->getPolygon_type()){
                            case 0:polygon->setPenStyle(eStyle.generic);break;
                            case 1:polygon->setPenStyle(eStyle.mark);break;
                            case 2:polygon->setPenStyle(eStyle.perimeterLine);break;
                            case 3:polygon->setPenStyle(eStyle.cut);break;
                            case 4:polygon->setPenStyle(eStyle.stitch);break;
                        }
                        polygon->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                        polygon->setSelectStyle(eStyle.selectedEntity);
                        curItem = polygon;
                        addItem(polygon);
                        connect(polygon, &Shape::sceneMoveableChanged, polygon, &Polygon::onSceneMoveableChanged);
                        break;
                    }

                        case Shape::Trapezium:
                        {
                            Trapezium *trapezium = new Trapezium;

                            trapezium->setTrapezium_alpha1(this->getTrapezium_alpha1());
                            trapezium->setTrapezium_alpha2(this->getTrapezium_alpha2());
                            trapezium->setTrapezium_H(this->getTrapezium_H());
                            trapezium->setTrapezium_toplength(this->getTrapezium_toplength());
                            trapezium->setTrapezium_type(this->getTrapezium_type());

                            trapezium->setShapeId(id+1);
                            switch(this->getPolygon_type()){
                                case 0:trapezium->setPenStyle(eStyle.generic);break;
                                case 1:trapezium->setPenStyle(eStyle.mark);break;
                                case 2:trapezium->setPenStyle(eStyle.perimeterLine);break;
                                case 3:trapezium->setPenStyle(eStyle.cut);break;
                                case 4:trapezium->setPenStyle(eStyle.stitch);break;
                            }
                            trapezium->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                            trapezium->setSelectStyle(eStyle.selectedEntity);
                            curItem = trapezium;
                            addItem(trapezium);
                            connect(trapezium, &Shape::sceneMoveableChanged, trapezium, &Trapezium::onSceneMoveableChanged);
                            break;
                        }
                default:
                    break;
                }

                if(curItem && !drawing && !moveable) {
                    itemList.append(curItem);
                    curItem->startDraw(event);
                    curItem->setObjectSize(eStyle.objSize);
                    if(getCurShape() == Shape::Point){
                        drawing = false;
                        update();
                    } else {
                        drawing = true;
                    }
                    modified = true;
                    emit sceneItemsChanged();
                }
            }
        }

        // 移动标识为真，表示目前可移动图元
        if(moveable){
            qDebug() << "移动...";
            // 移动图元步骤：
            // 1. 选择图元，可多选
            // 2. 移动图元
            for(int i=0; i<itemList.length();i++){
                itemList.at(i)->setMoveable(true);
            }
        }
        if(editable){
            if(curItem && curItem->updateFlag(event)) {
                drawing = false;
                curItem->setEditable(false);
                curItem->setEditOverFlag(true);
            }
            editable = false;
        }
    }

    // 右键进行 取消及恢复操作：
    // 取消作图、取消选择、取消移动
    // 恢复作图、

    if(event->button() == Qt::RightButton) {
        qDebug() << "点击右键";
        if(drawing){
            drawing = false;
        }

        if(curItem){
            if(getCurShape() == Shape::PolyLine && !curItem->getOverFlag()){
                curItem->setOverFlag(true);
                return;
            } else{
                curItem->setOverFlag(true);
            }
        }
        if(moveable){
            moveable = false;
        }
        drawable = !drawable;
        for(int i=0; i<itemList.length();i++){
            itemList.at(i)->setSelectable(!drawable);
            itemList.at(i)->setSelected(false);
            itemList.at(i)->setMoveable(moveable);
        }
        clearSelection();
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(curItem && drawing) {
        curItem->drawing(event);
    }
    update();
    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dragEnterEvent(event);
}

void Scene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dragLeaveEvent(event);
}

void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dragMoveEvent(event);
}

void Scene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dropEvent(event);
}

void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();
    painter->setBrush(eStyle.backgroundColor);
    painter->drawRect(rect);

    if(axesGrid.grid.showGrid){
        // 画网格
        // 获取到当前的线宽，这里的线宽其实还是之前设置的线宽值;
        // 比如我们之前设置线宽为 2 ，这里返回的线宽还是 2 ，但是当前的缩放比例变了；
        // 其实当前的线宽就相当于 penWidth * scaleFactor;
        // 所以如果我们想要让线宽保持不变，那就需要进行转换，即 penWidth = penWidth / scaleFactor;
        // 重新设置画笔线宽;
        QPen pen = QPen();
        pen.setWidthF(0);
        pen.setColor(axesGrid.grid.gridColor);
        painter->setPen(pen);

        const double w = sceneRect().width();
        const double h = sceneRect().height();

        for(int i=0; i<h; i+=axesGrid.grid.yStep / scaleFactor){
            painter->drawLine(QPointF(-w,i),QPointF(w,i));
            painter->drawLine(QPointF(-w,-i),QPointF(w,-i));
        }
        for(int i=0; i<w; i+=axesGrid.grid.xStep / scaleFactor)
        {
            painter->drawLine(QPointF(i,-h),QPointF(i,h));
            painter->drawLine(QPointF(-i,-h),QPointF(-i,h));
        }
    }

    if(axesGrid.axes.showAxes){
        // 画x轴
        QPen pen = QPen();
        pen.setWidthF(0);
        pen.setColor(axesGrid.axes.xAxisColor);
        pen.setJoinStyle(Qt::MiterJoin);
        painter->setPen(pen);

        QBrush brush = QBrush();
        brush.setColor(axesGrid.axes.xAxisColor);
        brush.setStyle(Qt::SolidPattern);
        painter->setBrush(brush);

        QPointF xPos = QPointF(axesGrid.axes.axisSizeInPix / scaleFactor,0);
        QLineF lineX(QPointF(0,0), xPos);
        drawLineWithArrow(painter, lineX, axesGrid.axes.arrowSizeInPix / scaleFactor);
        painter->drawText(10, 0, 20, 20, Qt::AlignLeft | Qt::AlignTop, tr("x"));


        // 画y轴
        pen.setWidthF(0);
        pen.setColor(axesGrid.axes.yAxisColor);
        pen.setJoinStyle(Qt::MiterJoin);
        painter->setPen(pen);

        brush.setColor(axesGrid.axes.yAxisColor);
        brush.setStyle(Qt::SolidPattern);
        painter->setBrush(brush);

        QPointF yPos = QPointF(0,-axesGrid.axes.axisSizeInPix / scaleFactor);
        QLineF lineY(QPointF(0,0), yPos);
        drawLineWithArrow(painter, lineY, axesGrid.axes.arrowSizeInPix / scaleFactor);
        painter->drawText(-10, -20, 20, 20, Qt::AlignLeft | Qt::AlignTop, tr("y"));
    }
    painter->restore();
}

void Scene::onViewScaleChanged(qreal scaleFactor)
{
    this->scaleFactor = scaleFactor;
    update();
}

void Scene::onAxesChanged(bool show)
{
    axesGrid.axes.showAxes = show;
    update();
}

void Scene::onGridChanged(bool show)
{
    axesGrid.grid.showGrid = show;
    update();
}

void Scene::onPointSelected(Point *point)
{
    emit pointSelected(point);
}

void Scene::onLineSelected(Line *line)
{
    emit lineSelected(line);
    if(line->isEditable()){
        curItem = line;
        drawing = true;
        editable = true;
    }
}

void Scene::onArcSelected(Arc *arc)
{
    emit arcSelected(arc);
}

void Scene::onEllipseSelected(Ellipse *ellipse)
{
    emit ellipseSelected(ellipse);
}

void Scene::onCircleSelected(Circle *circle)
{
    emit circleSelected(circle);
}

void Scene::onRectSelected(Rect *rect)
{
    emit rectSelected(rect);
}

void Scene::onPolyLineSelected(PolyLine *polyline)
{
    emit polyLineSelected(polyline);
}
