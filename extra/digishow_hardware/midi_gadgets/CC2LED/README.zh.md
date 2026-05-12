# CC2LED (MIDI CC to WS2812 LED Controller)

这是一个基于 Arduino 的 MIDI 互动灯带控制程序。它允许通过 USB 接收 MIDI Control Change (CC) 信号，并实时控制 WS2812 (NeoPixel) LED 灯带的亮度、速度、颜色和显示模式。

该程序非常适合用于音乐演出、互动艺术装置或通过 DAW（如 Ableton Live, Logic Pro 等）及互动控制软件（如 DigiShow）来同步控制实体灯效。

## 🛠 硬件需求

*   **开发板**：必须使用支持原生 USB 并且兼容 `MIDIUSB` 库的 Arduino 开发板（例如：Arduino Leonardo, Micro, Pro Micro, Zero, Due 等）。
*   **LED 灯带**：WS2812B / NeoPixel 或兼容的 RGB 灯带。

## 📦 软件依赖

在编译和上传代码之前，请确保在 Arduino IDE 中搜索并安装了以下库：

1.  **MIDIUSB**：用于通过 USB 接收和发送 MIDI 数据。
2.  **WS2812FX**：用于提供丰富的 WS2812 灯带动态效果。

## ⚙️ 引脚与参数配置

你可以在 `CC2LED.ino` 文件的顶部修改以下参数以适配你的硬件：

*   `LED_COUNT`：灯珠数量（默认：`180`）
*   `LED_PIN`：LED 数据控制引脚（默认：`A3`）

*如果你的 WS2812 灯带色彩排列不是 GRB 或者频率不是 800 KHz，也可以在初始化 `ws2812fx` 对象的代码行进行修改。*

## 🎛 MIDI CC 控制映射

程序会监听 USB 端口传入的控制变化（Control Change, CC）消息。通过发送特定的 CC 编号和数值（0~127），可以动态改变灯带的状态：

| CC 编号 | 控制参数 | 数值范围 (MIDI 0-127) | 实际映射范围 / 效果 |
| :--- | :--- | :--- | :--- |
| **CC 20** | 亮度 (Brightness) | 0 ~ 127 | 映射为 1 ~ 255 的亮度值 |
| **CC 21** | 速度 (Speed) | 0 ~ 127 | 映射为 0 ~ 3000 的动画速度 |
| **CC 22** | 红色 (Red) | 0 ~ 127 | 映射为 0 ~ 255 的红色通道值 |
| **CC 23** | 绿色 (Green) | 0 ~ 127 | 映射为 0 ~ 255 的绿色通道值 |
| **CC 24** | 蓝色 (Blue) | 0 ~ 127 | 映射为 0 ~ 255 的蓝色通道值 |
| **CC 25** | 动画模式 (Mode) | 0 ~ 127 | 切换 WS2812FX 库内置的各种动画模式 |

> **注**：默认上电启动时，默认颜色为天蓝色（R:0, G:123, B:255），默认动画模式为 `FX_MODE_TWINKLEFOX`。

## 🚀 使用方法

1. 按照引脚配置，将 WS2812 灯带的数据线连接到开发板的 `A3` 引脚（请注意共地和灯带独立供电）。
2. 将 Arduino 开发板连接到电脑。
3. 在 Arduino IDE 中打开 `CC2LED.ino`。
4. 确认已安装 `MIDIUSB` 和 `WS2812FX` 库。
5. 编译并上传代码到开发板。
6. 上传成功后，电脑会将开发板识别为一个 USB MIDI 设备。
7. 打开你的 MIDI 宿主软件或 DigiShow，将 MIDI 输出目标选为该开发板。
8. 触发或发送 CC20 ~ CC25 信号，即可实时看到灯带变化！
