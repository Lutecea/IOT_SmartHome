#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "../driver-lcd1602-i2c/lcd_screen_i2c.h"
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>

#define LED_YELLOW_NODE DT_ALIAS(led_yellow)
#define LCD_SCREEN_NODE DT_ALIAS(lcd_screen)

const struct gpio_dt_spec led_yellow_gpio = GPIO_DT_SPEC_GET_OR(LED_YELLOW_NODE, gpios, {0});
const struct i2c_dt_spec lcd_screen_dev = I2C_DT_SPEC_GET(LCD_SCREEN_NODE);
const struct device *const dht11 = DEVICE_DT_GET_ONE(aosong_dht);

int main(void) 
{
    gpio_pin_configure_dt(&led_yellow_gpio, GPIO_OUTPUT_HIGH);
    
    
    // Init LCD device
    init_lcd(&lcd_screen_dev);

    //Variable de temperature et humidité
    struct sensor_value temperature, humidity;
    
    sensor_sample_fetch(dht11);

    //Lecture des données 
    sensor_channel_get(dht11,  SENSOR_CHAN_AMBIENT_TEMP, &temperature);
    sensor_channel_get(dht11,  SENSOR_CHAN_HUMIDITY, &humidity);

    int temp = sensor_value_to_double(&temperature);
    int humi = sensor_value_to_double(&humidity);
    
    printf("Température : %d \n", temp);
    printf("Humidité : %d", humi);

    // Display a message
    //write_lcd(&lcd_screen_dev, ZEPHYR_MSG, LCD_LINE_1);
    //write_lcd_clear(&lcd_screen_dev, ZEPHYR_MSG LCD_LINE_2);
}

