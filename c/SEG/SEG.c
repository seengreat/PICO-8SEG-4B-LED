#include "SEG.h"
#include "stdio.h"
#include "hardware/spi.h"
#include "hardware/pwm.h"

const uint8_t SEG_Code[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // 10.A
    0x7C, // 11.b
    0x39, // 12.C
    0x5E, // 13.d
    0x79, // 14.E
    0x71, // 15.F
};

void LED_Init()
{
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}

void SEG_Init()
{
    /* Select spix  Set spi baud rate */
    spi_init(spi1, 10000 * 1000);
    gpio_set_function(SEG_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SEG_MOSI_PIN, GPIO_FUNC_SPI);

    gpio_init(SEG_RCLK_PIN);
    gpio_set_dir(SEG_RCLK_PIN, GPIO_OUT);
    gpio_put(SEG_RCLK_PIN, 1);
}

/*!
 *  @brief  Display a code in the location
 *  @param  location
 *          code
 *  @return None
 */
void SEG_Write(uint8_t location, uint8_t code)
{
    gpio_put(SEG_RCLK_PIN, 1);
    spi_write_blocking(spi1, &location, 1);
    spi_write_blocking(spi1, &code, 1);
    gpio_put(SEG_RCLK_PIN, 0);
    sleep_ms(2);
    gpio_put(SEG_RCLK_PIN, 1);
}

float n = 000.0;
void SEG_Demo()
{
    int temp;
    temp = (int)(n * 10);
    if (n >= 0.0)
    {
        sleep_us(500);
        SEG_Write(TENS, SEG_Code[temp % 100 / 10] | Dot);
        sleep_us(500);
        SEG_Write(UNITS, SEG_Code[temp % 10]);
        if (n >= 10.0)
        {
            sleep_us(500);
            SEG_Write(HUNDREDS, SEG_Code[(temp % 1000) / 100]);
            if (n >= 100.0)
            {
                sleep_us(500);
                SEG_Write(KILOBIT, SEG_Code[temp / 1000]);
            }
        }
    }
}

volatile bool timer_fired = false;
int64_t alarm_callback(alarm_id_t id, void *user_data)
{
    printf("Timer %d fired!\n", (int)id);
    timer_fired = true;
    // Can return a value here in us to fire in the future
    return 0;
}

bool repeating_timer_callback(struct repeating_timer *t)
{
    static int LED_level = 0;
    n += 0.1;
    if (n >= 1000.0)
    {
        n = 0.0;
    }
    LED_level = !LED_level;
    LED(LED_level);
    printf("n:%f\r\n", n);
    return true;
}

int main()
{
    stdio_init_all();
    LED_Init();
    SEG_Init();

    /* Timer set */
    add_alarm_in_ms(2000, alarm_callback, NULL, false);
    while (!timer_fired)
    {
        tight_loop_contents();
    }
    struct repeating_timer timer;
    add_repeating_timer_ms(150, repeating_timer_callback, NULL, &timer);

    while (1)
    {
        SEG_Demo();
    }
    return 0;
}