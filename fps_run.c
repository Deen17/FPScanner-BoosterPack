#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* XDC Module Headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Driver configuration */
//#include "ti_drivers_config.h"

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

    //sleep(1);

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


    fps_led_on(uart);

    // enroll
//    while(1){
//        if(fps_is_finger_pressed_uart(uart)){
//            fps_enroll_start(uart, 0);
//            result res;
//            res = fps_capture_finger(uart, 1);
//            res = fps_enrolln(uart, 1);
//            res = fps_capture_finger(uart, 1);
//            res = fps_enrolln(uart, 2);
//            res = fps_capture_finger(uart, 1);
//            res = fps_enrolln(uart, 3);
//            GPIO_write(LED_GREEN, res.res);
//            break;
//        }
//        sleep(1);
//    }

    // identify
    while(1){
        if(fps_is_finger_pressed_uart(uart)){
            int amt = fps_enroll_count(uart);
            result r = fps_check_enrolled(uart, 0);
            result res = fps_identify(uart);
            break;
        }
        sleep(1);

    }
}


