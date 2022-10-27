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
