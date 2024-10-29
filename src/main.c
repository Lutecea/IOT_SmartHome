#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "../driver-lcd1602-i2c/lcd_screen_i2c.h"

#define LED_YELLOW_NODE DT_ALIAS(led_yellow)

const struct gpio_dt_spec led_yellow_gpio = GPIO_DT_SPEC_GET_OR(LED_YELLOW_NODE, gpios, {0});

int main(void) 
{
    gpio_pin_configure_dt(&led_yellow_gpio, GPIO_OUTPUT_HIGH);


    // Init LCD device
    init_lcd(lcd-screen);

    // Display a message
    write_lcd(lcd-screen, HELLO_MSG, LCD_LINE_1);
    //write_lcd_clear(&dev_lcd_screen, ZEPHYR_MSG LCD_LINE_2);
}


//Ben ça marche pas faut corriger