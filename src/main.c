#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "../driver-lcd1602-i2c/lcd_screen_i2c.h"
#include <zephyr/drivers/i2c.h>
#include <stdio.h>

#define LED_YELLOW_NODE DT_ALIAS(led_yellow)
#define LCD_SCREEN_NODE DT_ALIAS(lcd_screen)

const struct gpio_dt_spec led_yellow_gpio = GPIO_DT_SPEC_GET_OR(LED_YELLOW_NODE, gpios, {0});
const struct i2c_dt_spec lcd_screen_dev = I2C_DT_SPEC_GET(LCD_SCREEN_NODE);

int main(void) 
{
    gpio_pin_configure_dt(&led_yellow_gpio, GPIO_OUTPUT_HIGH);


    // Init LCD device
    init_lcd(&lcd_screen_dev);

    // Display a message
    //write_lcd(&lcd_screen_dev, ZEPHYR_MSG, LCD_LINE_1);
    //write_lcd_clear(&lcd_screen_dev, ZEPHYR_MSG LCD_LINE_2);
}


//Ben ça marche pas faut corriger