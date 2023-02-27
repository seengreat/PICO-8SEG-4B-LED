from machine import Pin,SPI,Timer
import framebuf
import time

KILOBIT = 0xFE  # KiloBit
HUNDREDS = 0xFD # hundreds
TENS = 0xFB     # tens
UNITS = 0xF7    # units
Dot = 0x80      # decimal separator

SEG_Code = [
    0x3F,  # 0
    0x06,  # 1
    0x5B,  # 2
    0x4F,  # 3
    0x66,  # 4
    0x6D,  # 5
    0x7D,  # 6
    0x07,  # 7
    0x7F,  # 8
    0x6F,  # 9
    0x77,  # A
    0x7C,  # b
    0x39,  # C
    0x5E,  # d
    0x79,  # E
    0x71   # F
]


class SEG:
    def __init__(self):
        self.num = 0.0
        self.rclk = Pin(4,Pin.OUT)
        self.rclk(1)
        self.spi = SPI(1,10000*1000,polarity = 0, phase = 0, sck = Pin(10), mosi = Pin(11), miso = None)
        
     #  @brief  Display a code in the location
     #  @param  location
     #          code
     #  @return None
    def Write(self,Location,Code):
        self.rclk(1)
        self.spi.write(bytearray([Location]))
        self.spi.write(bytearray([Code]))
        self.rclk(0)
        time.sleep(0.002)
        self.rclk(1)
    def SEG_Demo(self):
        temp = int(self.num * 10)
        if self.num >= 0.0:
            time.sleep(0.0005)
            self.Write(TENS,SEG_Code[(temp%100)//10]|Dot)
            time.sleep(0.0005)
            self.Write(UNITS,SEG_Code[temp%10])
            if self.num >= 10.0:
                time.sleep(0.0005)
                self.Write(HUNDREDS,SEG_Code[(temp%1000)//100])
                if self.num >= 100.0:
                    self.Write(KILOBIT,SEG_Code[(temp%10000)//1000])
    
    def HandleInterrupt(self,timer):
        self.num += 0.1
        if self.num >= 1000.0:
            self.num = 0.0
        LED.toggle()

if __name__=='__main__':
    seg = SEG()
    LED = Pin(25,Pin.OUT)
    tim = machine.Timer()
    tim.init(period = 150, mode = machine.Timer.PERIODIC, callback = seg.HandleInterrupt)
    #color BRG
    while True:
        seg.SEG_Demo()
        
