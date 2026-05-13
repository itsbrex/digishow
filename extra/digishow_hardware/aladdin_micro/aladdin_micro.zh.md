### Aladdin Micro 开发板简介

**Aladdin Micro** 是一款核心兼容 Arduino Micro（以及 Arduino Leonardo）的开发板。

---

### 数字与模拟管脚重新封装

#### X0 - X7：8路数字输入端口
- 每个端口包含三个引脚：**+24V / IN / GND**
- 支持连接 **NPN 方式工业传感器** 或 **干触点输入**（短接 IN 与 GND）
- 对应 Arduino Micro 的引脚映射如下：

| 输入端口 | 对应 Arduino Micro 引脚 |
|----------|--------------------------|
| X0       | 23 (A5)                  |
| X1       | 22 (A4)                  |
| X2       | 21 (A3)                  |
| X3       | 20 (A2)                  |
| X4       | 4                        |
| X5       | 12                       |
| X6       | 8                        |
| X7       | 7                        |

---

#### Y0 - Y5：6路数字输出端口
- 每个端口包含两个引脚：**+24V / OUT**（共阳设计）
- 单路驱动能力：**500mA**
- 支持 **PWM 输出**
- 对应 Arduino Micro 的引脚映射如下：

| 输出端口 | 对应 Arduino Micro 引脚 |
|----------|--------------------------|
| Y0       | 9                        |
| Y1       | 11                       |
| Y2       | 6                        |
| Y3       | 10                       |
| Y4       | 5                        |
| Y5       | 13                       |

---

### 板载 5V GPIO 端口（Grove 连接器）

Aladdin Micro 还保留了 **6 个 5V IO 引脚**，分布在 3 个 **Grove 接口** 的 GPIO 端口中：

#### GPIO0
- 对应引脚：**18 (A0)**、**19 (A1)**
- 可连接：
  - 2 路 5V 数字 I/O
  - 2 路 5V 模拟传感器输入

#### GPIO1
- 对应引脚：**2 (SDA)**、**3 (SCL)**
- 可连接：
  - 2 路 5V 数字 I/O
  - I2C 接口传感器

#### GPIO2
- 对应引脚：**0 (RX)**、**1 (TX)**
- 可连接：
  - 2 路 5V 数字 I/O
  - 外接 UART 串口

---

### 软件支持与扩展应用

- **DigiShow RIOC 程序**  
  通过 Arduino IDE 烧录后，Aladdin 板子可在 **DigiShow 软件** 中作为 **Arduino 接口设备** 使用。

- **AladdinMIDI 程序**  
  烧录后，可被电脑或手机识别为 **USB MIDI 设备**，将板上的 I/O 映射为 **MIDI CC** 或 **MIDI Note** 信号。

- **自定义 Arduino 程序**  
  你也可以写入自己的 Arduino 程序，将 Aladdin Micro 打造为 **定制应用的控制器**。

---

### Aladdin Micro + DigiShow 使用指南

Aladdin Micro 搭配 DigiShow 软件，无需编程即可将工业级输入输出端口转化为互动控制信号。

**快速上手：**

1. 将 Aladdin Micro 通过 USB 连接电脑
2. 在 Arduino IDE 的库管理器中搜索并安装 **DigiShow RIOC**
3. 打开示例：`DigiShow RIOC / RiocArduino`
4. 选择开发板类型 **Arduino Micro**，烧录程序
5. 在 DigiShow 软件的接口管理器中新建 Arduino 接口，型号选择 **Aladdin Micro**，串口设为自动

完成以上步骤后，即可开始使用。
