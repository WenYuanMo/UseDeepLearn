#ifndef SHOWIMAGEWIDGET_H
#define SHOWIMAGEWIDGET_H

#include <QWidget>
#include <QObject>
#include <QFont>
#include<QPen>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <DealWithImage/dealwithimage.h>
#include <QLabel>
#include <QMetaMethod>

/***操作图片***/
enum OpeImageEnum
{
    /***移动roi***/
    _move_roi,
    /****删除roi*****/
    _delete_roi,
    /***移动图片****/
    _mover_board
};

enum MoveRoiEnum
{
    /***移动边缘***/
    _move_edge,
    /****移动坐标点****/
    _move_position,
    /***没有操作***/
    _none
};

class OpeImageType
{
public:
    /***操作图片***/
    OpeImageEnum _ope_image;
    /***roi的操作方式***/
    MoveRoiEnum _move_roi_type;
    /***操作的roi***/
    int _ope_roi_index;
};

class ShowImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowImageWidget(QWidget *parent = nullptr);
    /***适应画板****/
    void fitWindow();
    /****设置画板大小*****/
    void setBoardSize(qreal width_,qreal height_);
    /*****缩放当前画板******/
    void setScale(qreal scale_);
    /****移动当前画板*****/
    void setMove(qreal dx_,qreal dy_);
    /***刷新显示*****/
    void refleshShow();
    /*****计算点在画板里面的坐标******/
    QPointF getPointInBoard(QPointF);
    /*****计算点在画板里面的坐标******/
    bool getPointInBoard(double,double,QPointF*);
    /******点在窗口里面的坐标*******/
    QPointF getPointInWin(QPointF);
    /*******点在窗口里面的坐标*******/
    bool getPointInWin(double,double,QPointF*);

signals:

protected:
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    /****绘制的开始点*****/
    qreal _start_x;
    /***绘制的开始点*******/
    qreal _start_y;
    /*****画板的宽*****/
    qreal _board_width;
    /******画板的高*****/
    qreal _board_height;
    /******鼠标按下的坐标******/
    QPointF _down_point;
    /*******鼠标移动的坐标******/
    QPointF _last_point;
    /******鼠标按下******/
    bool _mouse_down;
    /******* 窗口除板子的缩放值*****/
    qreal _scale;
    /*******偏移*******/
    qreal _dy;
    /******偏移*******/
    qreal _dx;
    /******绘制字体的格式**********/
    QFont _font;
    /*****画笔******/
    QPen _pen;
    /***显示板的坐标****/
    QLabel *_labBoardCoor;
    /***图片的操作方式***/
    OpeImageType _ope_image_type;
};

#endif // SHOWIMAGEWIDGET_H
