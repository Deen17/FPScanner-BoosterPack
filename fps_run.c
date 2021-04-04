#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/* Fingerprint Scanner Packet library */
#include <fps.h>

// Timer for sleep
#include <ti/drivers/Timer.h>



/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
//    char        input;
//    const char  echoPrompt[] = "Echoing characters:\r\n";
    UART_Handle uart;
    UART_Params uartParams;

    //while(1);

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    sleep(1);

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 9600; // fingerprint scanner baud rate

    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    uart = UART_open(CONFIG_UART_0, &uartParams);

    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }

    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
    if (fps_open(uart) == -1){
        return(-1);
    }
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    fps_check_usb(uart);
//    fps_check_usb(uart);

    int success=0;
    while (success == 0){
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
        success = fps_led_on(uart);
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
        sleep(1);
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
        success = fps_led_off(uart);
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
        sleep(1);
    }

}


