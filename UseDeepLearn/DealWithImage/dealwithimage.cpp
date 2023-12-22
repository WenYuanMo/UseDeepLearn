#include "dealwithimage.h"

/***单列模式初始化****/
DealWithImage* DealWithImage::_deal_with_image=nullptr;

DealWithImage::DealWithImage(QObject *parent)
    : QObject{parent}
{
    _session=nullptr;
}

DealWithImage::~DealWithImage()
{
    if(_session!=nullptr)
    {
        _input_node_names.clear();
        _input_node_names.clear();
        _output_tensor.clear();

        delete _session;
    }
}

void DealWithImage::Init()
{

}

void DealWithImage::ClearSegmentationResult()
{
    if(_result_image.isNull()==false)
    {
        _result_image=QImage();
    }
}

void DealWithImage::AnalyzeImage()
{
    /***判断是否有图片***/
    if(_qimage.width()==0 &&_qimage.height()==0)
    {
        return;
    }

    /***判断是否有加载网络***/
    if(_session==nullptr)
    {
        return;
    }

    cv::Mat source_image_;
    cv::Mat deal_with_image_;
    cv::Mat result_image_;

    GetMat(_qimage).copyTo(source_image_);

    //    cv::namedWindow("sourceimage");
    //    /***显示一下原始图片***/
    //    cv::imshow("sourceimage",source_image_);

    if(source_image_.channels()>1)
    {
        cv::cvtColor(source_image_,deal_with_image_,cv::COLOR_BGR2GRAY);
    }
    else
    {
        source_image_.copyTo(deal_with_image_);
    }

    //    cv::namedWindow("test");
    //    /***显示一下处理的图片***/
    //    cv::imshow("test",deal_with_image_);

    /*****数据转换*****/
    int input_tensor_size_=deal_with_image_.cols*deal_with_image_.rows*1;
    size_t counter = 0;
    std::vector<float>input_data(input_tensor_size_);
    for (unsigned k = 0; k < 1; k++)
    {
        for (unsigned i = 0; i < deal_with_image_.rows; i++)
        {
            for (unsigned j = 0; j < deal_with_image_.cols; j++)
            {
                //  input_data[counter++] = (static_cast<float>(deal_with_image_.at<cv::Vec3b>(i, j)[k]));
                cv::Scalar sc_=deal_with_image_.at<uchar>(i, j);
                input_data[counter++] = (static_cast<float>(sc_[0]));
            }
        }
    }
    /***创建内存信息***/
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeCPU);
    vector<Ort::Value> input_tensors;
    input_tensors.push_back(Ort::Value::CreateTensor<float>(memory_info,/***处理内存的地方***/
                                                            input_data.data(),/***数据***/
                                                            input_data.size(),/****数据大小****/
                                                            _input_node_dims.data(),/****维度数据****/
                                                            _input_node_dims.size()));

    _output_tensor.clear();
    /***创建一个运行的选项***/
    Ort::RunOptions r_op;
    _output_tensor = _session->Run(r_op,/****运行*****/
                                _input_node_names.data(),/*****输入的名称******/
                                input_tensors.data(),/****输入的数据******/
                                _input_node_names.size(),/****输入的量******/
                                _output_node_names.data(),/***输出的名*****/
                                _output_node_names.size());/***输出的维度大小*****/

    /*****输出处理****/
    float* mask_ptr = _output_tensor[0].GetTensorMutableData<float>(); /***outtensor首地址*****/

    vector< unsigned char >results(input_tensor_size_);
    for (int i = 0; i <input_tensor_size_; i++)
    {
        results[i]=mask_ptr[i];
    }

    unsigned char* ptr = &results[0];
    result_image_ = cv::Mat(_output_node_dims[2], _output_node_dims[3], CV_8U, ptr);

    /****记录使用的图片***/
    int w_result_image_=result_image_.cols;
    int h_result_image_=result_image_.rows;
    _result_image=GetImage(result_image_).copy(0,
                                                 0,
                                                 w_result_image_,
                                                 h_result_image_);

//    cv::waitKey(0);

    source_image_.release();
    deal_with_image_.release();
}

QImage DealWithImage::GetImage(cv::Mat source_)
{
    /*****判断m的类型，可能是CV_8UC1  CV_8UC2  CV_8UC3  CV_8UC4****/
    switch(source_.type())
    {
    /*****QIamge 构造函数, ((const uchar *data, 宽(列),高(行), 一行共多少个（字节）通道，宽度*字节数，宏参数)****/
    case CV_8UC1:
    {
        QImage img((uchar *)source_.data,source_.cols,source_.rows,source_.cols * 1,QImage::Format_Grayscale8);
        return img;
    }
    break;
    case CV_8UC3:   /****一个像素点由三个字节组成*****/
    {
        //cvtColor(m,m,COLOR_BGR2RGB); BGR转RGB
        QImage img((uchar *)source_.data,source_.cols,source_.rows,source_.cols * 3,QImage::Format_RGB888);
        return img.rgbSwapped(); /******opencv是BGR  Qt默认是RGB  所以RGB顺序转换******/
    }
    break;
    case CV_8UC4:
    {
        //QImage img((uchar *)_target_image.data,_target_image.cols,_target_image.rows,_target_image.cols * 4,QImage::Format_RGBA8888);
        QImage img((uchar *)source_.data,source_.cols,source_.rows,source_.cols * 4,QImage::Format_RGB32);
        return img;
    }
    break;
    default:
    {
        QImage img; /****如果遇到一个图片均不属于这三种，返回一个空的图片****/
        return img;
    }
    }
}

cv::Mat DealWithImage::GetMat(QImage source_)
{
    /***初始化Mat****/
    cv::Mat mat = cv::Mat::zeros(source_.height(), source_.width(),source_.format());

    /****判断image的类型*****/
    switch(source_.format())
    {
     /****灰度图***/
    case QImage::Format_Grayscale8:
        mat = cv::Mat(source_.height(), source_.width(),
                      CV_8UC1,(void*)source_.constBits(),source_.bytesPerLine());
        break;

    case QImage::Format_RGB888:
        mat = cv::Mat(source_.height(), source_.width(),
                      CV_8UC3,(void*)source_.constBits(),source_.bytesPerLine());
        break;

    case QImage::Format_RGB32:
        mat = cv::Mat(source_.height(), source_.width(),
                      CV_8UC4,(void*)source_.constBits(),source_.bytesPerLine());
        break;

        /****4通道彩色 a通道为透明通道*****/
    case QImage::Format_ARGB32:
        mat =cv::Mat(source_.height(), source_.width(),
                      CV_8UC4,(void*)source_.constBits(),source_.bytesPerLine());
        break;

        /****单通道****/
    case QImage::Format_Indexed8:
        mat = cv::Mat(source_.height(), source_.width(),
                      CV_8UC1, (void*)source_.bits(), source_.bytesPerLine());
        break;

    default:
        ;
    }
    return mat;
}

bool DealWithImage::LoadModel(QString model_path_)
{
    bool ok_=true;
    /***创建一个环境****/
    _env=Ort::Env(ORT_LOGGING_LEVEL_ERROR, "OnnxModel");
    /***初始化Session选项 使用10个线程执行op,若想提升速度，增加线程数***/
    _session_options.SetInterOpNumThreads(1);
    /***ORT_ENABLE_ALL: 启用所有可能的优化***/
    //session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    _session_options.SetGraphOptimizationLevel( GraphOptimizationLevel::ORT_ENABLE_ALL);

    /****路径转换成c++底层****/
    //const ORTCHAR_T* session_model_path=L"I://PytorchBaseClass//qtUseOnnx//CellOnnxCpu.onnx";
    /***获取QString的UTF-16表示****/
    const ushort* utf16Data = model_path_.utf16();
    int length = model_path_.length();

    /***分配足够的空间来存储wchar_t字符串（包括空字符）***/
    std::vector<wchar_t> wcharArray(length + 1);
    /***复制数据到新的wchar_t数组****/
    std::copy(utf16Data, utf16Data + length, wcharArray.begin());
    wcharArray[length] = L'\0';

    ORTCHAR_T* session_model_path=wcharArray.data();

    /***创建一个回话***/
     _session=new Ort::Session(_env,session_model_path,_session_options);

    /***获取输入输出的批量***/
    _num_input_nodes = _session->GetInputCount();
    _num_output_nodes = _session->GetOutputCount();

    /***确定输入的维度,和输入的入口名****/
    _input_node_names={"input"};
    for (int i = 0; i < _num_input_nodes; i++)
    {
        Ort::TypeInfo type_info = _session->GetInputTypeInfo(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        ONNXTensorElementDataType type = tensor_info.GetElementType();
        std::vector<int64_t> in_node_shape = tensor_info.GetShape();
        int shape_index = 0;
        for (auto& shape: in_node_shape)
        {
            _input_node_dims[shape_index++] = shape;
        }
    }

    /***确定输出的维度,和输出的出口名称****/
    _output_node_names={"output"};
    for (int i = 0; i < _num_output_nodes; i++)
    {
        Ort::TypeInfo type_info = _session->GetOutputTypeInfo(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        std::vector<int64_t> out_node_shape = tensor_info.GetShape();
        int shape_index = 0;
        for (auto& shape : out_node_shape)
        {
            _output_node_dims[shape_index++] = shape;
        }
    }

    return ok_;
}
