""" Parts of the U-Net model """
"""https://github.com/milesial/Pytorch-UNet/blob/master/unet/unet_parts.py"""

import torch
import torch.nn as nn
import torch.nn.functional as F

#继承卷积模块 创建卷积模块
class DoubleConv(nn.Module):
    """(convolution => [BN] => ReLU) * 2"""
#初始化
    def __init__(self, in_channels, out_channels):
        super().__init__()
        self.double_conv = nn.Sequential(
            #加入一个卷积层 输入的通道数 输出的通道数 卷积核 垫料的步
            nn.Conv2d(in_channels, out_channels, kernel_size=3, padding=1),
            #标准化操作
            nn.BatchNorm2d(out_channels),
            #激活函数
            nn.ReLU(inplace=True),
            # 加入一个卷积层 输入的通道数 输出的通道数 卷积核 垫料的步
            nn.Conv2d(out_channels, out_channels, kernel_size=3, padding=1),
            # 标准化操作
            nn.BatchNorm2d(out_channels),
            # 激活函数
            nn.ReLU(inplace=True))

    def forward(self, x):
        return self.double_conv(x)

#向下取样模块
class Down(nn.Module):
    """Downscaling with maxpool then double conv"""
#初始化
    def __init__(self, in_channels, out_channels):
        super().__init__()
        self.maxpool_conv = nn.Sequential(
            #池化层
            nn.MaxPool2d(2),
            DoubleConv(in_channels, out_channels))

    def forward(self, x):
        return self.maxpool_conv(x)


#向上取样模块
class Up(nn.Module):
    """Upscaling then double conv"""
    def __init__(self, in_channels, out_channels, bilinear=True):
        super().__init__()
        # if bilinear, use the normal convolutions to reduce the number of channels
        if bilinear:
            self.up = nn.Upsample(scale_factor=2, mode='bilinear', align_corners=True)
        else:
            self.up = nn.ConvTranspose2d(in_channels // 2, in_channels // 2, kernel_size=2, stride=2)
        self.conv = DoubleConv(in_channels, out_channels)

    def forward(self, x1, x2):
        x1 = self.up(x1)
        # input is CHW
        diffY = torch.tensor([x2.size()[2] - x1.size()[2]])
        diffX = torch.tensor([x2.size()[3] - x1.size()[3]])

        x1 = F.pad(x1, [diffX // 2, diffX - diffX // 2,
                        diffY // 2, diffY - diffY // 2])

        x = torch.cat([x2, x1], dim=1)
        return self.conv(x)

class OutConv(nn.Module):
    def __init__(self, in_channels, out_channels):
        super(OutConv, self).__init__()
        self.conv = nn.Conv2d(in_channels, out_channels, kernel_size=1)

    def forward(self, x):
        return self.conv(x)