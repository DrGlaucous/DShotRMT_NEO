// ...some very simple DShot example generating a DShot300 signal.

#include <Arduino.h>
#include <DShotRMT.h>

// ...clearly name usb port
#ifdef SERIAL
#define USB_Serial Serial
constexpr auto USB_SERIAL_BAUD = 115200;
#endif // SERIAL

DShotRMT motor01(GPIO_NUM_4, RMT_CHANNEL_0);

volatile uint16_t throttle_value = 0x30; // ...sending "48", the first throttle value
constexpr auto FAILSAVE_THROTTLE = 0x3E7;

void setup()
{
    // ...always start the onboard usb support
    USB_Serial.begin(USB_SERIAL_BAUD);

    // ...start the dshot generation
    motor01.begin(DSHOT300);
}

void loop()
{
    readSerialThrottle();

    motor01.sendThrottleValue(throttle_value);

    // ...print to console
    USB_Serial.println(throttle_value);
}

//
//
uint16_t readSerialThrottle()
{
    if (USB_Serial.available() > 0)
    {
        auto throttle_input = (USB_Serial.readStringUntil('\n')).toInt();
        return throttle_input;
    }
    else
    {
        return FAILSAVE_THROTTLE;
    }
}
