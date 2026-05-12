# CC2PWM - MIDI控制器转PWM信号

Arduino项目，将MIDI CC信息转为PWM输出，适用于Micro或Leonardo板。

**特性：**
- USB接收MIDI CC
- 将0-127映射为0-255 PWM
- 支持7个PWM引脚（3,5,6,9,10,11,13）

**引脚对应：**
- 引脚3→CC20，5→CC21，6→CC22，9→CC23，10→CC24，11→CC25，13→CC26

**使用：** 上传代码→连接电脑→MIDI软件识别设备→发送CC20-26即可。

