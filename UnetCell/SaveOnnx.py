import glob
import numpy as np
import torch
import os
import cv2
import onnx
import onnxruntime
import torchvision
from model.unet_model import UNet

def SaveOnnx():
    # device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    device = torch.device('cpu')
    net = UNet(n_channels=1, n_classes=1)
    # 将网络拷贝到deivce中
    net.to(device=device)
    # 加载模型参数
    net.load_state_dict(torch.load('best_model_cpu.pth', map_location=device))
    net.eval()
    data = torch.randn(1, 1, 512, 512,device=device)
    onnx_save_path = 'CellOnnxCpu.onnx'
    input_names = ["input"]
    output_names = ["output"]
    torch.onnx.export(net,
                      data,
                      onnx_save_path,
                      verbose=True,
                      input_names=input_names,
                      output_names=output_names,
                      opset_version=11)

    # # torch.onnx.export(net,
    # #                   data,
    # #                   onnx_save_path,
    # #                   verbose=True,
    # #                   input_names=["input"],  # 输入名
    # #                   output_names=["output"],  # 输出名
    # #                   )
    # torch.onnx.export(net,
    #                   data,
    #                   onnx_save_path,
    #                   verbose=True,
    #                   opset_version=11,
    #                   export_params=True,
    #                   do_constant_folding=True)
    # # # Load the ONNX model
    model = onnx.load("CellOnnxCpu.onnx")
    # Check that the IR is well formed
    onnx.checker.check_model(model)
    # Print a human readable representation of the graph
    print(onnx.helper.printable_graph(model.graph))

    # x = torch.randn(1, 1, 512, 512, device="cpu")
    # model = torch.load('best_model_cpu.pth', map_location=torch.device('cpu'))
    # model.eval()
    # input_names = ["input"]
    # output_names = ["output"]
    # torch.onnx.export(model, x, "CellOnnxCPU.onnx", verbose=True, input_names=input_names, output_names=output_names,
    #                   opset_version=11)




def LoadOnnx():
    #检测onnx能不能用
    onnx_save_path = 'CellOnnx.onnx'
    #onnx_save_path='I:\\PytorchBaseClass\\chengxu\\catAndDogCheck\\cat_dog_cnn.onnx'
    #image_path='‪C://Users//Administrator//Desktop//0.png'
    tests_path = glob.glob('data/test/*.png')
    # 遍历素有图片
    test_path=tests_path[0]
    img_ = cv2.imread(test_path)
    img_standurn_ = cv2.resize(img_, (512, 512))
    cv2.imshow('src', img_)
    # # opencv dnn加载
    # net = cv2.dnn.readNetFromONNX(onnx_save_path)
    net = cv2.dnn.readNetFromONNX(onnx_save_path)
    #设置运行在gpu上面
    # net.setPreferableBackend(cv2.dnn.DNN_BACKEND_CUDA)
    # net.setPreferableTarget(cv2.dnn.DNN_TARGET_CUDA)
    blob = cv2.dnn.blobFromImage(img_standurn_)
    # Run a model
    net.setInput(blob)
    out = net.forward()
    print(out)  # 得到推理的结果
    pass

if __name__ == "__main__":
    # print(torch.onnx.producer_version)
    # print(onnxruntime.__version__)
    # print(onnx.__version__)
    # print(torch.__version__)
    # print(torchvision.__version__)
    # print(cv2.__version__)
    SaveOnnx()
    #LoadOnnx()
