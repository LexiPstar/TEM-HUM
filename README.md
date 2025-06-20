# STM32 智能温湿度监测系统

该项目基于 STM32F103C8T6 微控制器，集成 DHT11 传感器、OLED 显示模块、ESP8266 WiFi 模块以及 Flash 存储，实现温湿度采集、显示、网页发布、超限报警及历史数据存储功能。

---

## 🔧 硬件组成

- STM32F103C8T6 核心板
- DHT11 温湿度传感器
- SSD1306 OLED 显示屏（I2C）
- ESP8266（ESP-01S）WiFi 模块
- CH340G USB-TTL 模块（调试）
- 电源：USB 或外部供电

---

## 🧩 软件模块

- **DHT11 采集模块**：读取温湿度数据
- **OLED 显示模块**：显示当前环境参数
- **ESP8266 网页模块**：通过 AT 指令生成网页，展示实时温湿度
- **USART 通信模块**：双串口通信，分别调试输出和 WiFi 通信
- **报警系统模块**：支持阈值设置和报警触发（可扩展中断）
- **Flash 历史记录模块**：将温湿度数据写入内部 Flash 储存并保留断电数据

---


## 📦 Flash 存储说明

- 地址：0x0801FC00（最后一页 1KB）
- 每条记录：2 字节（温度 + 湿度）
- 最大记录条数：根据页大小动态计算（默认 255 条）
- 启动时判断是否初始化，未初始化则清空并写入标志位

---

## 🚨 报警功能

- 设置温湿度上下限值
- 超限后 OLED 显示报警提示
- 预留中断 / 蜂鸣器 / 网络推送接口

---

## ⚙️ 开发环境

- IDE：Keil uVision5
- 库：STM32 StdPeriph Library
- 编程语言：C

---

## 📈 后续拓展

- 增加 MQTT / Blynk 云平台接入
- 网页端实时曲线图显示（温湿度变化趋势）
- 使用 EEPROM 保存大规模历史数据
- 移植至 FreeRTOS 多任务系统
- 添加蓝牙通信或移动端 App 支持

---
