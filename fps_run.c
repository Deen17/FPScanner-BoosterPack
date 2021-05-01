#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* for sleep and usleep */
#include <unistd.h>

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

static useconds_t POLL_PERIOD = 200000;

void waitUntilFingerReplaced(UART_Handle *handle);

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
//    char        input;
//    const char  echoPrompt[] = "Echoing characters:\r\n";
    UART_Handle uart;
    UART_Params uartParams;



    /* Call driver init functions */
    GPIO_init();
    UART_init();

    //while(1);

    usleep(100000);

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
    //GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    // delete db just for testing purposes
    fps_delete_all(uart);

    // enroll

    dword id = 8    ; // let's start with an id of 300

    fps_led_on(uart);
    while(1){
        if(fps_is_finger_pressed_uart(uart)){
            fps_enroll_start(uart, id);
            result res;

            // First enrollment
            fps_led_on(uart);
            res = fps_capture_finger(uart, 1);
            res = fps_enrolln(uart, 1);
            fps_led_off(uart);

            // Second enrollment

            waitUntilFingerReplaced(&uart);

            fps_led_on(uart);
            res = fps_capture_finger(uart, 1);
            res = fps_enrolln(uart, 2);
            fps_led_off(uart);

            // Third and last enrollment

            waitUntilFingerReplaced(&uart);

            fps_led_on(uart);
            res = fps_capture_finger(uart, 1);
            res = fps_enrolln(uart, 3);
            fps_led_off(uart);

            GPIO_write(CONFIG_GPIO_LED_0, !res.res);
            break;
        }
        usleep(POLL_PERIOD);
    }

    // identify
    fps_led_on(uart);
    while(1){
        if(fps_is_finger_pressed_uart(uart)){
            int amt = fps_enroll_count(uart);
            result res = fps_check_enrolled(uart, 0);
            waitUntilFingerReplaced(&uart);
            fps_led_on(uart);
            res = fps_capture_finger(uart, 0);
            res = fps_identify(uart);
            GPIO_write(LED_GREEN, res.parameter == id);
            break;
        }
        usleep(POLL_PERIOD);

    }
    fps_led_off(uart);
}

void waitUntilFingerReplaced(UART_Handle *handle){
    int pressed = 1;

    // blink to let the user know it is time to remove finger
    fps_led_off(*handle);
    sleep(1);
    fps_led_on(*handle);

    while (pressed){
        pressed = fps_is_finger_pressed_uart(*handle);
        usleep(POLL_PERIOD);
    }

    // blink to let the user know it is time to place finger
    fps_led_off(*handle);
    sleep(1);
    fps_led_on(*handle);

    while(!pressed){
        pressed = fps_is_finger_pressed_uart(*handle);
    }
    return;
}

