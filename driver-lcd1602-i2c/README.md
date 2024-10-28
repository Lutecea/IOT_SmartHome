# LCD 1602 I2C Driver for Zephyr

> [!WARNING]
> The program contains intentional errors for educational purposes!

## Usage

```c

#include "lcd_screen_i2c.h"

int main() {

    // Init device
    init_lcd(&dev_lcd_screen);

    // Display a message
    write_lcd(&dev_lcd_screen, HELLO_MSG, LCD_LINE_1);
    write_lcd_clear(&dev_lcd_screen, ZEPHYR_MSG LCD_LINE_2);
}
```
