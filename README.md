# XDrive 
Stepper motor with multi-function interface and closed loop function.  
具有多功能接口和闭环功能的步进电机。
![appearance icon](\Picture\appearance.jpg)

## 版本更新日志
### 2021/01/03
1.避开了上电后第一次读取编码器错误数据造成的启动抖动（本质问题未解决）  
2.修正了控制器初始化错误造成的启动抖动（本质还是因为第一条的原因）  
3.XDrive_BL升级到F1.2版本，仅使用UP按键作为升级按键  
4.修正了一些讲不清道不明的BUG  
5.XDrive项目讨论可加群（知家1群：857046846）（艾家1群：557214000 已满）（艾家2群：1105489563）

## 目录简介
Firmware_APP：主要的控制固件（XDrive_APP.uvprojx 为主应用工程） 
Firmware_BL：引导固件（用于引导USB更新）  
Hardware：电路图纸  
Machinery：外壳图纸  
Picture：XDrive照片  
Specification：使用及入门介绍

