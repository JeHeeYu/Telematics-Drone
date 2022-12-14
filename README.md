# Drone FC Project

# Development Environment
OS : Windows10
<br>
IDE : STM32CUBEIDE
<br>
Language : C

# Hardware
MCU : STM32F405RGTX
<br>
9 Axis Sensor : BNO080
<br>
6 Axis Sensor : ICM20602
<br>
Temperature Pressure Sensor : LPS22HH
<br>
Receiver : FS-IA6B
<br>
EEPROM : AT24C08
<br>
GPS Module : M8N

# System Architecture
<img src="https://user-images.githubusercontent.com/87363461/199011597-8ebdf1a8-be01-415b-904c-4d886468da79.png" width="800" height="600">

# Pin Configuration
<img src="https://user-images.githubusercontent.com/87363461/199010660-83e7dab2-8182-4c74-bebb-4f830b0c76e9.JPG" width="600" height="600">

# LPS22HH 

## LPS2HH Pin Configuration
Channel : SPI3
<br>
SPI3_SCK : PB3
<br>
SPI3_MISO : PB4
<br>
SPI3_MOSI : PB5
<br>
GPIO_OUTPUT_CS : PB6
<br>
GPIO_INPUT_INT : PB7
<br>
Drive : LL Driver

## LPS22HH SPI Interface
LPS22HH는 I2C, SPI, MIPI I3C 통신 3가지를 지원
<br>
SPI 통신을 사용하기 위해 CS핀을 LOW로 설정 후 통신 시 SPI 통신 사용 가능
<br>
<br>
<img src="https://user-images.githubusercontent.com/87363461/198203592-e7e97b38-956e-4bb8-902c-8cf06dd8d950.JPG" width="500" height="300">
<br>
<br>
SPI Clock의 속도는 최대 10MHz를 사용할 수 있으며, 최대 8MHz를 추천
<br>
<br>
<img src="https://user-images.githubusercontent.com/87363461/198206921-affa7d47-5351-4d29-9513-dcbfa284582f.JPG" width="500" height="300">
<br>
<br>
Clock Polarity High, MSB가 먼저 전송됨
<br>
<br>
<img src="https://user-images.githubusercontent.com/87363461/198207235-74edb70c-92c7-4eeb-b209-8fea4348438e.JPG" width="500" height="300">
<br>
<br>
데이터의 길이는 8bit, Clock Rising Edge에서 데이터 전송이 이루어짐
<br>
최상위 비트로 Read, Write를 결정
<br>
<br>
<img src="https://user-images.githubusercontent.com/87363461/198207639-6bdd150a-b062-42f1-ae5f-96a36fc37b4b.JPG" width="500" height="300">


## LPS22HH Pressure Data Format
기압 데이터는 24bit(3bytes)로 출력됨
<br>
기압 데이터를 낮은 주소부터 한 바이트씩 3번 읽으면 각각 하위, 중간, 상위 바이트로 읽히고, 부호있는 정수 형태로 출력
<br>
<br>
<img src="https://user-images.githubusercontent.com/87363461/198204725-f02c1768-4489-4faa-b492-84e8ca928729.JPG" width="500" height="300">
<br>
읽은 데이터 중 하위 바이트는 그대로, 중간 바이트는 왼쪽으로 8 Shift, 상위 바이트는 왼쪽으로 16 Shift 한다.
<br>
Shift한 값들을 비트 단위로 OR 연산 후 4096으로 나누면 기압 단위 .hPa 값이 된다.
<br>
<br>
<img src="https://user-images.githubusercontent.com/87363461/198205084-09a8c54b-139d-4943-a245-42a65fd5fa4e.JPG" width="400" height="150">

## LPS22HH Temperature Data Format
온도 데이터는 16bit(2bytes)로 출력됨
<br>
온도 데이터를 낮은 주소부터 한 바이트씩 읽으면 각각 하위, 상위 바이트로 읽히고, 부호 있는 정수 형태로 출력
<br>
<br>
<img src="https://user-images.githubusercontent.com/87363461/198206462-530cfec0-1e62-4daa-a188-44ad57935f98.JPG" width="500" height="300">
<br>
읽은 데이터 중 하위 바이트는 그대로, 상위 바이트는 왼쪽으로 8 Shift
<br>
Shift한 값들을 비트 단위로 OR 연산 후 100으로 나누면 온도 단위 .C 값이 된다.

## LPS22HH Data Plotter
<img src="https://user-images.githubusercontent.com/87363461/198213027-3534ba12-ba8c-4729-a605-1f41b93899f7.JPG" width="600" height="200">

# GPS

## NMEA(The National Marine Electronics Association) Protocol Data Format
GPGGA(Global Positioning System Fix Data) : 시간, 위도, 경도, 고도 등 표시
<br>
GPGSV(GPS Satellites in View) : 현재 GPS Module이 수신할 수 있는 모든 위성의 정보
<br>
GPRMC(Recommended Minimum Data) : 추천되는 최소한의 데이터들
<br>
GPGSA(GNSS DOP and Active Satellite) : 데이터를 제공하는 위성들을 나열
## M8N UART Setting
Mode : Asynchronous
<br>
Word Length : 8bits
<br>
Baud Rate : 9600bps
<br>
Parity : None
<br>
Stop Bits : 1bit

## M8N Pin Configuration
Channel : UART4
<br>
UART4_TX : PC10
<br>
UART4_RX : PC11
<br>
Driver : LL Driver
<br>
Interrupt : Receive Interrupt

## UBX Protocol Message Frame Structure 
<ol>
  <li>SYNC CHAR1 : 0xB5(ASCII : 'u') 고정 값으로, 메시지 시작을 나타냄</li>
  <li>SYNC CHAR2 : 0x62(ASCII : 'b') 고정 값으로, 메시지 시작을 나타냄</li>
  <li>CLASS : 메시지의 종류를 나타내는 클래스</li>
  <li>ID : 메시지의 클래스 ID</li>
  <li>LENGTH Little Endian 1Byte : 실제 데이터(Payload)의 길이 LSB</li>
  <li>LENGTH Little Endian 2Byte : 실제 데이터(Payload)의 길이 MSB</li>
  <li>PAYLOAD : 메시지 정보가 담겨있는 데이터 (GPS 정보)</li>
  <li>CK_A : Checksum A (에러 검출)</li>
  <li>CK_B : Checksum B (에러 검출)</li>
</ol>
<img src="https://user-images.githubusercontent.com/87363461/198226740-ad1587e7-775e-45df-8c00-5ff5852941fc.JPG" height="500" width="700">

### Checksum Example Code
<pre>
n : Checksum 계산할 바이트 크기
buffer : 체크섬 계산할 필드의 데이터(Class, ID, Length , Payload)

CK_A = 0, CK_B = 0
for(int i = 0; i < n; i++) {
    CK_A = CK_A + buffer[i];
    CK_B = CK_B + CK_A
}
</pre>


# Receiver

## FS-IA68 Pin Configuration
Channel : UART5
<br>
UART5_TX : PC12
<br>
UART5_RX : PD2
<br>
Baud rate : 115200
<br>
Word Lengt : 8 Bits
<br>
Stop Bit : 1 bit
<br>
Driver : LL Driver
<br>
Interrupt : Receive Interrupt
<br>
Data Direction : Receive Only

## i-bus Protocol Message Frame Structure 
<ol>
  <li>SYNC CHAR1 : 0x20 고정 값으로, 메시지 시작을 나타냄</li>
  <li>SYNC CHAR2 : 0x40 고정 값으로, 메시지 시작을 나타냄</li>
  <li>CH1 : CH1 데이터의 LSB</li>
  <li>CH2 : CH1 데이터의 MSB</li>
  <li>CH3 ~ 13 : 메시지 정보가 담겨있는 데이터</li>
  <li>CHECKSUM : Checksum (에러 검출)</li>
</ol>

### Checksum Example Code
<pre>
Checksum : unsigned short
n : Checksum 계산할 바이트 크기 (30 고정)
buffer : 체크섬 계산할 필드의 데이터(Class, ID, Length , Payload)

Checksum = 0xffff

Loop(I M N) 
{
    Checksum = Cheksum - buffer[i];
}
</pre>

# ESC protocol

## Dshot
디지털 전송 방식 중 하나로, Dshot150m Dshot300, Dshot600, Dshot1200 등이 있음
<br>
뒤에 숫자는 bitrate를 의미함
<br>
<br>
ESC Calibration 작업이 필요 없이 디지털 값으로 0일 경우 모터 정지, 2047이면 최대 출력
<br>
각 비트마다 펄스의 폭으로 0과 1을 표현함

## Proshot
디지털 전송 방식 중 하나로 16bit의 Dshot 방식을 4개의 PWM Pulse 방식으로 표현함
<br>
각각의 Pulse는 Width에 따라 4bit(0 ~ 15)의 값을 표현

## Oneshot125
Duty Cycle : 125us ~ 250us(8kHz ~ 4kHz)
<br>
Comparison : x8Faster than Standard PWM
<br>
<br>
Pulse Period : 2Khz
<br>
pulse Width : 125us ~ 250us

## ESC Pin Configuration
Channel : TIM5
<br>
Count Period : 41999
<br>
Auto Reload Preload : Enable
<br>
Driver : LL Driver
<br>
<br>
PWM1 : PA0
<br>
Pulse : 10500
<br>
Mode : Fast Mode
<br>
<br>
PWM2 : PA1
<br>
Pulse : 21000
<br>
Mode : Fast Mode
<br>
<br>
PWM3 : PA2
<br>
Pulse : 31500
<br>
Mode : Fast Mode
<br>
<br>
PWM4 : PA3
<br>
Pulse : Non
<br>
Mode : Fast Mode

## Calibration 방법 
<ol>
<li>ESC Power Up</li>
<li>PWM을 최대 Pulse Width으로 설정(Max Pulse Width 설정 이후 7초 이상 대기)</li>
<li>PWM을 최소 Pulse Width으로 설정(MIN Pulse Width 설정 이후 8초 이상 대기)</li>
<li>ESC Calibration 완료 및 PWM 인식</li>
<li>Motor Ready</li>
</ol>

# EEPROM

## AT24C08 Description
<ul>
<li>비휘발성 메모리</li>
<li>AT24Cxx 뒤는 용량(kbits, 08 -> 8kbits = 1Kbytes)</li>
<li>내부적으로 64개의 pages로 영역을 구분</li>
<li>1page당 16bytes의 크기를 가짐</li>
<li>I2C I/F를 지원</li>
<li>쓰기 금지(WP) 핀 제공 (데이터 보호를 위함)</li>
<li>AT24C04 이상 부터는 Page Write Mode로 Write할 때 16byte 단위로 써야 함</li>
<li>내부 메모리 주소는 0번지 부터 1023번지 까지 존재</li>
<li>내부 메모리 주소를 표현하기 위해 10bit 필요</li>
</ul>

## AT24C08 Pin Description
A1, A0 : No Connects(or Connect to GND)
<br>
A2 : Device Address(Connect to VCC or GND)
<br>
SDA : I2C Serial Data
<br>
SCL : I2C Serial Clock
<br>
WP : Write Protection
<br>
 - High : Write Protection
 - Low : Normal R/W Operation
<br>
<img src ="https://user-images.githubusercontent.com/87363461/198865457-355590aa-99cc-40f7-98c5-065d7e7106a0.JPG" width="700" height="500">

## AT24C08 Device Address
AT24C08의 경우 장치 주소를 표현하는 7비트 중 상위 5비트만 실제 장치 주소를 표현하는데 사용
<br>
AT24C08의 경우 8K의 크기를 가짐
<br>
<br>
<img src="https://user-images.githubusercontent.com/87363461/198865613-5970c541-0302-4457-875b-8c201e18eee7.JPG" width="500" height="300">

## AT24C08 Pin Configuration
SCL : PB8
<br>
SDA : PB9
<br>
WP : PC13
<br>
Mode : I2C
<br>
Speed Mode : Fast Mode
<br>
Clock Speed : 400000Hz
<br>
Duty Cycle : Tlow / Thigh = 2
<br>
Driver : HAL Driver

## AT24C08 Page Address
<img src="https://user-images.githubusercontent.com/87363461/198865904-1bafba5c-fc0e-4887-99a4-648193d78fac.JPG" width="700" height="300">
