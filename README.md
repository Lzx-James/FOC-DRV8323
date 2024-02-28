# FOC-DRV8323

**基于DRV8323HRTAR的FOC驱动器**

DRV8323HRTAR芯片从属于德州仪器TI公司的DRV832x芯片系列

相较于DRV8323、DRV8323R和DRV8323S，DRV8323H系列通过外部引脚的1-level、4-level和7-level式的外接电阻决定部分芯片功能，具体如下：

<img title="" src="Img/hardware_interface_img.png" alt="" width="323" data-align="center">

### 4-level

![4-level](Img\4-level.png)

[*手册 Page 40*](Doc/drv8323.pdf)

本项目使用的DRV8323HRTAR芯片的采样增益引脚GAIN和PWM模式控制引脚MODE均采用4-level接线，由图中可知，Vi1,Vi2,Vi3,Vi4分别对应引脚直连AGND、接47kΩ(±5%)电阻到AGND、高阻态（接500kΩ以上的大电阻到AGND）以及直连DVDD；而Vi1,Vi2,Vi3,Vi4对于MODE引脚和GAIN引脚有又分别对应其四个不同的模式：

| 4-level            | MODE        | GAIN  |
| ------------------ | ----------- | ----- |
| 直连DVDD             | Independent | 放大40倍 |
| 高阻态/接>500kΩ电阻到AGND | 单路PWM       | 放大20倍 |
| 接47kΩ电阻到AGND       | 3路PWM       | 放大10倍 |
| 直连AGND             | 6路PWM       | 放大5倍  |

#### PWM MODE

![6xPWM](Img\6xPWM.png)

[*手册 Page 31*](Doc/drv8323.pdf)

在6路PWM模式下，通过外部控制器能够实现每路PWM独立控制一个MOS管；由于PWM输入存在死区，为保证实现精确控制，应当在微控制器中加入死区补偿代码。

需要将MODE引脚连接到AGND实现6路PWM控制。

![3xPWM](Img\3xPWM.png)

[*手册 Page 31*](Doc/drv8323.pdf)

在3路PWM模式下，每路PWM可以控制一组半桥（即两个MOS管），当INLx设置为高电平时，通过INHx的PWM输入能够使得半桥上下两个MOS管的交替开关；这一模式能够较好地适用于FOC电机控制，即能够便捷的实现SVPWM，同时节省了IO口。

需要将MODE引脚连接47kΩ电阻到AGND

![1xPWM](Img\1xPWM-1.png)

![六步换向](Img\1xPWM-2.png)

[*手册 Page 31-32*](Doc/drv8323.pdf)

单PWM模式适用于对无刷电机的六步换向控制，仅需INHA提供PWM输入，余下5个接口使用GPIO输入即可；在Figure19中，通过霍尔传感器的电平变化作为INLA、INHB和INLB的输入，有效简化了控制输入。

#### GAIN

<img title="" src="Img/bidir_adc_1.png" alt="" width="333">![电流求解公式](Img\adc_equation.png)

[*手册 Page 41-42*](Doc/drv8323.pdf)

DRV8323H的电流采样如图，通过改变GAIN引脚的外接电阻可以改变采样增益。

### 7-level

![7-level](Img\7-level.png)level

[*手册 Page 40*](Doc/drv8323.pdf)

IDRIVE和VDS引脚采用7-level接线，通过芯片内的比较器选择不同模式：

| 7-level            | IDRIVE                  | VDS         |
| ------------------ | ----------------------- | ----------- |
| 直连DVDD             | IG peak/sink：1/2A       | 不使能         |
| 接18kΩ电阻到DVDD       | IG peak/sink：570/1140mA | D-S阈值：1.88V |
| 接75kΩ电阻到DVDD       | IG peak/sink：260/520mA  | D-S阈值：1.13V |
| 高阻态/接>500kΩ电阻到AGND | IG peak/sink：120/240mA  | D-S阈值：0.6V  |
| 接75kΩ电阻到AGND       | IG peak/sink：60/120mA   | D-S阈值：0.26V |
| 接18kΩ电阻到AGND       | IG peak/sink：30/60mA    | D-S阈值：0.13V |
| 直连AGND             | IG peak/sink：10/20mA    | D-S阈值：0.06V |

IDRIVE用于设置MOS管的源极驱动电流，VDS用于检测MOS管漏极和栅极之间的电压。

### 1-level

![1-level](Img\1-level.png)

[*手册 Page 39*](Doc/drv8323.pdf)

1-level适用于芯片的数字引脚，经过比较器后得到逻辑高电平和低电平输入到芯片。



## *硬件已开源*
