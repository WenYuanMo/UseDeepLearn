#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(DealWithImage::getStatic()->_show_result_image==true)
    {
        ui->checkbox_show_result_image->setChecked(true);
    }
    else
    {
        ui->checkbox_show_result_image->setChecked(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    DealWithImage::deletStatic();
    QMainWindow::closeEvent(event);
}

void MainWindow::showEvent(QShowEvent *event)
{
    if(_frist_show)
    {

    }
    QMainWindow::showEvent(event);
}

void MainWindow::on_pbtn_fit_image_clicked()
{
    ui->show_image_widget->fitWindow();
}

void MainWindow::on_pbtn_load_one_image_clicked()
{
    QString filename;
    filename=QFileDialog::getOpenFileName(this, tr("选择图像"), "", tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        if(!DealWithImage::getStatic()->_qimage.load(filename))
        {
            QMessageBox::information(this, tr("打开图像失败"), tr("打开图像失败!"));
        }
        else
        {
            ui->show_image_widget->fitWindow();
        }
    }
}

void MainWindow::on_pbtn_analyze_image_clicked()
{
    DealWithImage::getStatic()->ClearSegmentationResult();
    DealWithImage::getStatic()->AnalyzeImage();

    QMessageBox::information(this, "图片分析成功", "图片分析成功!");

}

void MainWindow::on_checkbox_show_result_image_clicked(bool checked)
{
    if(checked==true)
    {
        DealWithImage::getStatic()->_show_result_image=true;
    }
    else
    {
        DealWithImage::getStatic()->_show_result_image=false;
    }
}

void MainWindow::on_pbtn_load_neual_net_clicked()
{
    QString filename;
    filename=QFileDialog::getOpenFileName(this, "选择网络", "", "network (*.onnx)");
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        if(DealWithImage::getStatic()->LoadModel(filename)==true)
        {
            /***网络加载成功*****/
            ui->tedit_neural_net_message->append("网络加载成功\n");

            ui->tedit_neural_net_message->append(QString("输入的节点数:")+
                                                            QString::number(DealWithImage::getStatic()->_num_input_nodes)
                                                 +"\n");

            ui->tedit_neural_net_message->append(QString("输出的节点数:")+
                                                            QString::number(DealWithImage::getStatic()->_num_output_nodes)
                                                 +"\n");

            QMessageBox::information(this, "网络加载成功", "网络加载成功!");
        }
    }
}

