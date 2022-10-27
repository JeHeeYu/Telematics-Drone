# Drone FC Project

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
