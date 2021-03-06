## 步进电机PWM失步修复

> 通过一个光电编码器连接到步进电机上，通过Arduino控制转发PWM控制电机。
> 期望当电机失步的时候，进行纠正。
> 在高速打印下，总是会发生一些奇怪的失步，如果你希望和我一样即保持高速，又降低失误率。
> 那么你可以尝试这个方案.

#### 器材清单

    1.arduino nano x 2（如果你需要控制xy，你需要arduino 2560，它有4个中断可以在一个板子上）
    2.光电编码器 x 2，（与需要监控的步进电机连接）
    3.你可能需要联轴器，或者同步轮
    4.4899驱动拦截板 x 2
    
#### 原理

    arduino通过中断，读取原本输入的pwm信号，再输送到4899驱动板子上。
    实现PWM转发。
    
    > 经过个人测试，pwm在16细分的情况下，arduino会发生一些奇怪的事情，初步判断，
    > 如果在pwm转发上如果有其他条件的话，可能无法赶上时钟频率，所以延迟,经过测试在8细分下工作的不错
    > 在2560上跑两个轴，你可能需要1.8°的电机，或者4细分才能跑到200mm/s以上。
    > 还是比较推荐两个板子来完成这件事情。

    光电编码器则进行监控，假设获得的记步，和实际收到的pwm计步，有一定的偏差。
    如果信号没有发现问题，则直接转发。
    如果有一些偏差，则跳过部分pwm信号，尝试引导到正确的位置。
    理论上即便丢失一小部分，或者因外力影响走到了一个比较奇怪的位置。
    依然可以纠正并返回到正确的坐标。
    （丢失部分层，或者一两个小零件，总比毁掉整个作品强）

#### 电路图


#### 其他


    
    