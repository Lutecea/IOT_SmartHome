#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "../driver-lcd1602-i2c/lcd_screen_i2c.h"
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/adc.h>

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#define LED_YELLOW_NODE DT_ALIAS(led_yellow)
#define LCD_SCREEN_NODE DT_ALIAS(lcd_screen)
#define SW0_NODE DT_ALIAS(sw0)
#define SW1_NODE DT_ALIAS(sw1)

const struct gpio_dt_spec led_yellow_gpio = GPIO_DT_SPEC_GET_OR(LED_YELLOW_NODE, gpios, {0});
const struct i2c_dt_spec lcd_screen_dev = I2C_DT_SPEC_GET(LCD_SCREEN_NODE);
const struct device *const dht11 = DEVICE_DT_GET_ONE(aosong_dht);
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,{0});
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios,{0});

        //=========================================
        //                Fonctions
        //=========================================
    void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{

	//Action à réaliser à l'appui bouton
	printk("Appui bouton !");
}


int main(void) 
{
	gpio_pin_configure_dt(&led_yellow_gpio, GPIO_OUTPUT_HIGH);
    
    
    // Init LCD device
    init_lcd(&lcd_screen_dev);


    
}

void thread_lecture(void)
{
    

    //void thread0() 
    while (1)
    { 
		    //Variable de temperature et humidité
    	struct sensor_value temperature, humidity;
        //=========================================
        //Lecture Capteur de température & Humidité
        //=========================================


        sensor_sample_fetch(dht11);

        //Lecture des données 
        sensor_channel_get(dht11,  SENSOR_CHAN_AMBIENT_TEMP, &temperature);
        sensor_channel_get(dht11,  SENSOR_CHAN_HUMIDITY, &humidity);

        int temp = sensor_value_to_double(&temperature);
        int humi = sensor_value_to_double(&humidity);

        printf("Température : %d \n", temp);
        printf("Humidité : %d\n", humi);

        
        //=========================================
        //Lecture SteamSensor
        //=========================================


        #if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};

    int err;
	uint32_t count = 0;
	uint16_t buf;
	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
	};

	/* Configure channels individually prior to sampling. */
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		if (!adc_is_ready_dt(&adc_channels[i])) {
			printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);

		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			printk("Could not setup channel #%d (%d)\n", i, err);

		}
	}

		for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
			int32_t val_mv;

			printk("Lecture ADC STEAM SENSOR, channel %d: ",
		
			       adc_channels[i].channel_id);

			(void)adc_sequence_init_dt(&adc_channels[i], &sequence);

			err = adc_read_dt(&adc_channels[i], &sequence);
			if (err < 0) {
				printk("Could not read (%d)\n", err);
				continue;
			}

			/*
			 * If using differential mode, the 16 bit value
			 * in the ADC sample buffer should be a signed 2's
			 * complement value.
			 */
			if (adc_channels[i].channel_cfg.differential) {
				val_mv = (int32_t)((int16_t)buf);
			} else {
				val_mv = (int32_t)buf;
			}
			printk("%"PRId32, val_mv);
			err = adc_raw_to_millivolts_dt(&adc_channels[i],
						       &val_mv);
			/* conversion to mV may not be supported, skip if not */
			if (err < 0) {
				printk(" (value in mV not available)\n");
			} else {
				printk(" = %"PRId32" mV\n", val_mv);
			}
		}

		k_sleep(K_MSEC(1000));
	}


};

void thread_button(void)
{
    //=========================================
    //Appui bouton
    //=========================================

	int ret;
	static struct gpio_callback button_cb_data;

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button0.pin));
	gpio_add_callback(button0.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button0.port->name, button0.pin);




};


K_THREAD_DEFINE(thread_lecture_id, 521, thread_lecture, NULL, NULL, NULL, 9, 0, 0);
K_THREAD_DEFINE(thread_button_id, 521, thread_button, NULL, NULL, NULL, 9, 0, 0);