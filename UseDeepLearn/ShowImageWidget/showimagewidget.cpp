#include "showimagewidget.h"

ShowImageWidget::ShowImageWidget(QWidget *parent)
    : QWidget{parent}
{
    _scale=1;
    _dy=0;
    _dx=0;
    _start_x=0;
    _start_y=0;
    _mouse_down=false;
    _board_width=this->width();
    _board_height=this->height();

    _font.setFamily("Microsoft YaHei");
    _font.setPointSize(20);
    _font.setBold(true);
    _font.setLetterSpacing(QFont::AbsoluteSpacing, 3);
    _pen.setBrush(Qt::green);
    _pen.setWidth(10);
    this->setMouseTracking(true);
    _labBoardCoor=new QLabel(this);
    _labBoardCoor->setGeometry(10,0,300,30);
    _labBoardCoor->setText("0,0");
    _ope_image_type._ope_image=OpeImageEnum::_mover_board;
    _ope_image_type._move_roi_type=MoveRoiEnum::_none;
    _ope_image_type._ope_roi_index=-1;
}

void ShowImageWidget::fitWindow()
{
    /***获取当前的长度****/
    qreal win_width_=this->width();
    qreal win_height_=this->height();

    if(DealWithImage::getStatic()->_qimage.width()>0&&DealWithImage::getStatic()->_qimage.height()>0)
    {
        _board_width=DealWithImage::getStatic()->_qimage.width();
        _board_height=DealWithImage::getStatic()->_qimage.height();
    }

    /****获取窗口与画板比例***/
    qreal width_scale_=win_width_/_board_width;
    qreal height_scale_=win_height_/_board_height;

    /***计算缩放****/
    if(width_scale_<height_scale_)
    {
        _scale=width_scale_;
    }
    else
    {
        _scale=height_scale_;
    }

    /***偏移为零****/
    _dx=0;
    _dy=0;

    /****计算适合的长宽*****/
    qreal fitWidth_=_board_width*_scale;
    qreal fitHeight_=_board_height*_scale;

    /******计算开始坐标*******/
    _start_x=(win_width_-fitWidth_)/2;
    _start_y=(win_height_-fitHeight_)/2;
    /*****更新图片*****/
    this->update();

}

void ShowImageWidget::setMove(qreal dx_, qreal dy_)
{
    _dx=dx_;
    _dy=dy_;
    this->update();
}

void ShowImageWidget::refleshShow()
{
    this->update();
}

void ShowImageWidget::setScale(qreal scale_)
{
    _scale*=scale_;
    this->update();
}

void ShowImageWidget::setBoardSize(qreal width_, qreal height_)
{
    _board_width=width_;
    _board_height=height_;
}

QPointF ShowImageWidget::getPointInBoard(QPointF point_)
{
    QPointF po_=point_;
    po_.setX((point_.x()-this->_start_x-this->_dx)/_scale);
    po_.setY((point_.y()-this->_start_y-this->_dy)/_scale);
    return  po_;
}

bool ShowImageWidget::getPointInBoard(double x_,double y_,QPointF* point_)
{
    point_->setX((x_-this->_start_x-this->_dx)/_scale);
    point_->setY((y_-this->_start_y-this->_dy)/_scale);
    return  true;
}

QPointF ShowImageWidget::getPointInWin(QPointF point_)
{
    QPointF po_=point_;
    po_.setX(point_.x()*_scale+this->_start_x+this->_dx);
    po_.setY(point_.y()*_scale+this->_start_y+this->_dy);
    return  po_;
}

bool ShowImageWidget::getPointInWin(double x_,double y_,QPointF* point_)
{
    point_->setX(x_*_scale+this->_start_x+this->_dx);
    point_->setY(y_*_scale+this->_start_y+this->_dy);
    return  true;
}

void ShowImageWidget::wheelEvent(QWheelEvent *event)
{
    if(DealWithImage::getStatic()->_qimage.width()==0
        && DealWithImage::getStatic()->_qimage.height()==0)
    {
        return;
    }

    /******获取鼠标窗口坐标****/
    QPointF win_point1_=event->position();
    /****鼠标点在画板中没有缩放的坐标****/
    qreal original_x_=(win_point1_.x()-this->_start_x-this->_dx)/_scale;
    qreal original_y_=(win_point1_.y()-this->_start_y-this->_dy)/_scale;

    if(event->angleDelta().y()>0)//缩放
    {
        _scale*=1.2;
    }
    else
    {
        _scale*=0.8;
    }

    QPointF win_point2_;
    /******缩放后鼠标原来画板中的坐标对应窗口坐标******/
    win_point2_.setX(original_x_*_scale+this->_start_x+this->_dx);
    win_point2_.setY(original_y_*_scale+this->_start_y+this->_dy);
    /****更新开始坐标*****/
    _start_x=_start_x+((win_point1_.x()-win_point2_.x()));
    _start_y=_start_y+((win_point1_.y()-win_point2_.y()));

    this->update();
}

void ShowImageWidget::mousePressEvent(QMouseEvent *event)
{
    if(DealWithImage::getStatic()->_qimage.width()==0
        && DealWithImage::getStatic()->_qimage.height()==0)
    {
        return;
    }

    if ( event->button() == Qt::LeftButton )
    {
        _mouse_down=true;
        _down_point=event->pos();
        _last_point=event->pos();

        switch (_ope_image_type._ope_image)
        {
        case OpeImageEnum::_mover_board:
            break;
        case OpeImageEnum::_move_roi:
        {
            _ope_image_type._move_roi_type=MoveRoiEnum::_none;
            _ope_image_type._ope_roi_index=-1;

            /***转换这个坐标点****/
            QPointF po_= getPointInBoard(_down_point);

        }
        break;
        case OpeImageEnum::_delete_roi:
        {
            _ope_image_type._move_roi_type=MoveRoiEnum::_none;
            _ope_image_type._ope_roi_index=-1;

            QPointF po_= getPointInBoard(_down_point);

            this->update();
        }
        break;
        }

    }
}

void ShowImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(DealWithImage::getStatic()->_qimage.width()==0
        && DealWithImage::getStatic()->_qimage.height()==0)
    {
        return;
    }

    if(_mouse_down==true)
    {
        _last_point=event->pos();

        switch (_ope_image_type._ope_image)
        {
        case OpeImageEnum::_mover_board:
        {
            _dx+=_last_point.x()-_down_point.x();
            _dy+=_last_point.y()-_down_point.y();
        }
        break;

        case OpeImageEnum::_move_roi:
        {
        }
        break;

        case OpeImageEnum::_delete_roi:
            break;
        }

        _down_point=_last_point;
        this->update();
    }
    else
    {

        QPointF point_=event->pos();
        qreal x_=(point_.x()-this->_start_x-this->_dx)/_scale;
        qreal y_=(point_.y()-this->_start_y-this->_dy)/_scale;

        if(x_<0||x_>_board_width
            ||y_<0||y_>_board_height)
        {
            x_=0;
            y_=0;
        }

        QColor rgb_=DealWithImage::getStatic()->_qimage.pixel(x_,y_);

        _labBoardCoor->setText(QString::number(x_)
                               +QString::fromStdString(",")
                               +QString::number(y_)
                               +QString::fromStdString("-Rgb:")
                               +QString::number(rgb_.red())
                               +QString::fromStdString(",")
                               +QString::number(rgb_.green())
                               +QString::fromStdString(",")
                               +QString::number(rgb_.blue()));

    }
}

void ShowImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    /****去掉警告**/
    Q_UNUSED(event);

    if(DealWithImage::getStatic()->_qimage.width()==0
        && DealWithImage::getStatic()->_qimage.height()==0)
    {
        return;
    }

    if(_mouse_down==true)
    {
        _mouse_down=false;
    }
}

void ShowImageWidget::paintEvent(QPaintEvent *event)
{
    if(DealWithImage::getStatic()->_qimage.width()==0&&DealWithImage::getStatic()->_qimage.height()==0)
    {
        return;
    }

    int image_width_=DealWithImage::getStatic()->_qimage.width();
    int image_height_=DealWithImage::getStatic()->_qimage.height();

    QPainter painter(this);
    QRect blackboard_=event->rect();
    painter.fillRect(blackboard_,Qt::black);
    qreal moveX_=this->_start_x+this->_dx;
    qreal moveY_=this->_start_y+this->_dy;

    /***根据参数创建坐标系****/
    painter.translate(moveX_,moveY_);
    painter.scale(_scale,_scale);

    painter.drawImage(0,0,DealWithImage::getStatic()->_qimage);

    if(DealWithImage::getStatic()->_show_result_image==true)
    {
        if(DealWithImage::getStatic()->_result_image.width()!=0
            &&DealWithImage::getStatic()->_result_image.height()!=0)
        {
            painter.drawImage(image_width_+10,0,DealWithImage::getStatic()->_result_image);
        }
    }
    painter.end();
}



















