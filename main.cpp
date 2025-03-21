#include <iostream>
#include "src/XPad.hpp"
#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

#define DIR_PIN 22
#define PWM_PIN 26  // GPIO12 (BCM) corresponds to wiringPi pin 26
// these gpios are mapped stupid, see https://pinout.xyz/pinout/wiringpi


int main() {

    int motorOneDir = 1;
    // Instantiate control pad object
    XPad pad;
    
    // Initialize wiringPi
    if (wiringPiSetup() == -1) {
        std::cerr << "Setup failed!" << std::endl;
        return 1;
    }
    
    // Set up PWM output
    softPwmCreate(PWM_PIN, 0, 100);
    
    // Set up DIR output
    pinMode(DIR_PIN, 1);

    // Event loop
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        // Resolve joystick device if disconnected
        if (!pad.isActive()) {
            try {
                auto device = XPad::getAvailableControllerPath();
                pad.connect(std::get<0>(device), std::get<1>(device));
                std::cout << "Succesfully connected to device " << std::get<0>(device) << " ("
                          << (std::get<1>(device) == XPadType::ONE_WIRED ? "Wired" : "Wireless") << ")\n";
            } catch (...) {
                std::cerr << "Unable to connect to pad...\n";
                sleep(1);
                continue;
            }
        }

        // Skip if no event was read
        if (!pad.readEvent()) {
            continue;
        }

        // Read & validate event
        auto event = pad.getEvent();
        if (!event.isInputType()) {
            continue;
        }

        std::cout << "Event: " << event.getSourceName() << "\n";

        if (event.getNativeType() == XEventType::EVENT_BUTTON) {
            // std::cout << "\t[Button]: Pressed: " << (event.isButtonPressed() ? "true" : "false")
            //           << ", Released: " << (event.isButtonReleased() ? "true" : "false") << "\n";

            // if (event.getSource() == XEventSource::BUTTON_X) {
            //     std::cout << "Trigger rumble (1)\n";
            //     pad.rumble(1);
            // }

            // if (event.getSource() == XEventSource::BUTTON_Y) {
            //     std::cout << "Trigger rumble (3)\n";
            //     pad.rumble(3);
            // }
            if (event.getSource() == XEventSource::BUTTON_B && event.isButtonPressed()) {
                std::cout << "Switching motor direction\n";
                if(motorOneDir == 0)
                {
                    motorOneDir = 1;
                    digitalWrite(DIR_PIN, motorOneDir);
                }
                else
                {
                    motorOneDir = 0;
                    digitalWrite(DIR_PIN, motorOneDir);
                }
            }
        }

        if (event.getNativeType() == XEventType::EVENT_AXIS) {
            // Analog
            const XEventSource Sources[] = {
                    XEventSource::ANALOG_LEFT_JOYSTICK_X,
                    XEventSource::ANALOG_LEFT_JOYSTICK_Y,
                    XEventSource::ANALOG_RIGHT_JOYSTICK_X,
                    XEventSource::ANALOG_RIGHT_JOYSTICK_Y,
            };
            if(event.getSource() == Sources[1]) // Y axis of left joystick
            {
                float leftJoystickValueY = event.getJoystickValue();
                float leftJoystickValueY = event.getJoystickValue();
                softPwmWrite(PWM_PIN, leftJoystickValue);
                //else
                   // softPwmWrite(PWM_PIN, -leftJoystickValue);
 
            }
            // for (const auto &joystickSource: Sources) {
            //     if (event.getSource() == joystickSource) {
            //         std::cout << "\t[Joystick] Position: " << event.getJoystickValue() << "\n";
            //     }
            // }

            // Trigger
            const XEventSource triggerSources[] = {
                    XEventSource::ANALOG_LEFT_TRIGGER,
                    XEventSource::ANALOG_RIGHT_TRIGGER,
            };
            for (const auto &triggerSource: triggerSources) {
                if (event.getSource() == triggerSources[0]) {
                    std::cout << "\t[Left Trigger] Position: " << event.getTriggerValue() << "\n";
                    
                   // softPwmWrite(PWM_PIN, 50 + (event.getTriggerValue()/2));

                }
                else
                {
                    std::cout << "\t[Right Trigger] Position: " << event.getTriggerValue() << "\n";
                   // softPwmWrite(PWM_PIN, (event.getTriggerValue()/2));
                }
            }

            // DPAD
            if (event.getSource() == XEventSource::DPAD_Y || event.getSource() == XEventSource::DPAD_X) {
                std::cout << "\t[DPAD] Direction: " << event.getDpadDirection() << "\n";
            }
        }

        std::cout << "\n";
    }
#pragma clang diagnostic pop
}