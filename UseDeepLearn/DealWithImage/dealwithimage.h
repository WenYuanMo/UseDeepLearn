#ifndef DEALWITHIMAGE_H
#define DEALWITHIMAGE_H

#include <QObject>
#include <QList>
#include <QApplication>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QImage>
#include <windows.h>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

#include <onnxruntime_c_api.h>
#include <onnxruntime_cxx_api.h>
#include <tensorrt_provider_factory.h>
#include <cpu_provider_factory.h>
#include <onnxruntime_run_options_config_keys.h>
#include <onnxruntime_session_options_config_keys.h>

using namespace Ort;

using std::cin;
using std::cout;
using std::endl;

using namespace std;


class DealWithImage : public QObject
{
    Q_OBJECT
public:
    explicit DealWithImage(QObject *parent = nullptr);
     ~DealWithImage();

    static DealWithImage* getStatic()
    {
        if(_deal_with_image==nullptr)
        {
            _deal_with_image=new  DealWithImage();
        }
        return _deal_with_image;
    }

    static void deletStatic()
    {
        if(_deal_with_image!=nullptr)
        {
            delete _deal_with_image;
        }
    }

signals:

public:
    /****初始化*****/
    void Init();

    /***清空结果***/
    void ClearSegmentationResult();
    /***分析图片***/
    void AnalyzeImage();
    /***获取qimage*****/
    QImage GetImage(cv::Mat);
    /****获取mat***/
    cv::Mat GetMat(QImage);
    /***加载模板***/
    bool LoadModel(QString);

    /***qt控件显示的图片******/
    QImage _qimage;
    /***结果图片***/
    QImage _result_image;

    /***模板路径***/
    QString _model_path;

    /***输入输出的维度***/
    std::array<int64_t,4> _input_node_dims;
     /***输入输出的维度***/
    std::array<int64_t,4> _output_node_dims;

    /***输入输出的节点名称***/
    std::vector<const char*> _input_node_names;
    std::vector<const char*> _output_node_names;
    /***输出的结果***/
    std::vector<Ort::Value> _output_tensor;
    /****环境*****/
    Ort::Env _env;
    /***绘画的选项****/
    Ort::SessionOptions _session_options;
    /***一个绘画***/
    Ort::Session* _session;
    /***输入输出的比量***/
    size_t _num_input_nodes;
    /***输入输出的比量***/
    size_t _num_output_nodes;

    /***显示结果图片***/
    bool _show_result_image;




private:
    /****静态处理控件*****/
    static DealWithImage* _deal_with_image;

};

#endif // DEALWITHIMAGE_H
