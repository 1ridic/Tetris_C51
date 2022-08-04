# 基于 STC89C58 的俄罗斯方块游戏

## 目录结构

```Bash
├── Code                # 代码目录
│   ├── inc             # 头文件目录
│   │   ├── block.h
│   │   ├── button.h    # 按键抽象枚举
│   │   ├── delay.h
│   │   ├── display.h
│   │   ├── draw.h
│   │   ├── eeprom.h
│   │   ├── img.h       # 屏幕显示资源文件
│   │   ├── main.h
│   │   ├── menu.h
│   │   └── mine.h
│   ├── src             # 原文件目录
│   │   ├── block.c     # 俄罗斯方块逻辑实现
│   │   ├── delay.c     # 延时函数
│   │   ├── display.c   # 屏幕显示初始化函数
│   │   ├── draw.c      # 屏幕绘制函数
│   │   ├── eeprom.c    # EEPROM读写
│   │   ├── main.c      # 中断函数，初始化与主循环
│   │   ├── menu.c      # 菜单逻辑实现以及各部分逻辑组合
│   │   └── mine.c      # 未完善的扫雷逻辑实现
│   ├── Tetris_C51.uvopt
│   └── Tetris_C51.uvproj
├── Hardware            # 硬件部分原理图与PCB
│   ├── PCB_Tetris_C51.pcbdoc
│   └── Schematic_Tetris_C51.pdf
├── LICENSE
└── README.md
```

## 一些必要说明

### 复位按键

PCB 封装错误，用的时候自行魔改或者换一个封装

### 屏幕型号

ST7920 驱动的 LCD12864 屏幕，使用 CS1 和 CS2 控制左右半屏选择，若使用不同屏幕，需要重写所有关于显示的代码。

### 右上角的四个排针

配合屏幕功能，跳接后可以通过电位器调节屏幕对比度

### 没有使用到的部分

- 硬件上： DFPlayer mini 的功能并未在代码里实现；存在没有使用的排针和排母；
- 软件上： 写了一点点的扫雷程序，最后时间不够没写完，后面也懒得再写了

## 关于项目

是我的课设，使用 GPL 协议进行开源，代码使用者务遵守协议内容。
