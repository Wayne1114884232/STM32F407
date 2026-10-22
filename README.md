# STM32F407 嵌入式实验工程集

汇集 STM32F4 小车控制、WIT/JY901 姿态采集、TFT/触摸显示及线性 CCD 实验。各目录是独立的 Keil 工程，进入对应子目录选择工程后再编译。

## 项目导航

| 工程 | 主要内容 |
| --- | --- |
| [STM32F407 基础小车与姿态采集](STM32F407/STM32F407/STM32F407_ProjectTemplate/README.md) | 主程序初始化 OLED、编码器、TB6612 电机驱动、WIT I²C 姿态传感器、TIM7 和超声波模块，并在 OLED 上显示姿态角与角速度。 |
| [JY901 串口与小车控制实验](STM32F407%20-%20%E4%B8%B2%E5%8F%A3jy901/STM32F407_ProjectTemplate/README.md) | 主程序接入 WIT 串口协议，处理按键状态，按时间间隔刷新 OLED 上的姿态角、按键计数和编码器数据，并调用目标距离设置函数。 |
| [JY901 串口实验副本](STM32F407%20-%20%E4%B8%B2%E5%8F%A3jy901%20-%20%E5%89%AF%E6%9C%AC/STM32F407_ProjectTemplate/README.md) | 主程序以 115200 波特率初始化 USART3，注册 WIT 串口发送、数据更新和延时回调，并初始化 OLED、编码器、部分电机驱动、按键和超声波模块。 |
| [ATK-MD0430 TFT 与触摸演示](STM32F407%20-%20TFT/STM32F407_ProjectTemplate/README.md) | 主程序初始化 ATK-MD0430 显示模块，显示实验信息，并循环绘制旋转立方体。 |
| [ZC30D-text 定时器与 LED 测试](ZC30D-text/STM32F407_ProjectTemplate/README.md) | 当前启用的主程序初始化板级功能、TIM7 和 LED，在 TIM7 中断中翻转 LED4。 |
| [软件 SPI LCD 与触摸实验](STM32F407-Softwave-SPI/README.md) | 主程序调用 LCD、触摸和按键初始化，并进入 `Temp_Humi_Alarm_GUI()` 界面函数。 |
| [线性 CCD 采集测试](STM32F405RGT6%E7%BA%BF%E6%80%A7CCD%E6%B5%8B%E8%AF%95%E4%BB%A3%E7%A0%81/2.STM32F405RGT6%E7%BA%BF%E6%80%A7CCD%E6%B5%8B%E8%AF%95%E4%BB%A3%E7%A0%81/README.md) | 主程序定义 128 点采样缓冲区，初始化串口、LED、按键、OLED、ADC 和 TIM3，并在主循环刷新 OLED。 |

## 快速开始

```bash
git clone https://github.com/Wayne1114884232/STM32F407.git
```

1. 根据上表选择实验，并查看子工程 README。
2. 安装 Keil MDK 和对应 STM32F4 器件包。当前工程均选择 Arm Compiler 5，保存版本为 5.06 update 6 或 update 7。
3. 使用 Keil 打开子工程的 `.uvprojx` 文件，核对目标器件、编译器、晶振、内存和外设连接。
4. 执行 Rebuild，再根据实际调试器配置下载。含电机驱动的实验首次上电先架空车轮。

## 目录与版本说明

- 多数工程使用 `app/`、`bsp/`、`libraries/`、`project/MDK(V5)/` 结构。
- CCD 和软件 SPI 工程使用 `USER/`、`CORE/`、`FWLIB/` 等目录，入口见对应 README。
- 根目录 ZIP 文件与同名源码目录均保留；两者是否完全一致未校验。继续开发时优先从明确选定的源码目录开始。
- “副本”目录不代表最新版本或稳定版本，不应仅按名称合并。
- CCD 目录名称含 STM32F405RGT6，但工程配置为 STM32F407VGTx；软件 SPI 工程配置为 STM32F407VETx，其余已核对工程为 STM32F407VGTx。烧录前按实际芯片核对。

## 当前验证范围

文档依据已提交的目录、主程序和 Keil 配置整理。未在本次整理中执行编译、烧录或实板测试，因此不将现存 HEX、编译产物或历史目录名称作为可运行证明。

## 第三方代码

仓库包含 CMSIS、ST 标准外设库和外设示例代码；使用、修改或再分发时应遵守对应文件头与随附许可证。当前未为整个工程集声明统一许可证。
