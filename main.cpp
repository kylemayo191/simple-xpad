#include <iostream>
#include "src/XPad.hpp"
#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <cmath>

//compile using:
// $cmake --build build
// run using:
// $./build/simple-xpad

//Motor One
#define M1_DIR_PIN 22  // GPIO6  (RPi) corresponds to wiringPi pin 22
#define M1_PWM_PIN 26  // GPIO12 (RPi) corresponds to wiringPi pin 26

//Motor Two
#define M2_DIR_PIN 24  // GPIO19  (RPi) corresponds to wiringPi pin 24
#define M2_PWM_PIN 27  // GPIO16  (RPi) corresponds to wiringPi pin 27

//Motor Three
#define M3_DIR_PIN 25  // GPIO26  (RPi) corresponds to wiringPi pin 25
#define M3_PWM_PIN 28  // GPIO20  (RPi) corresponds to wiringPi pin 28

//Motor Four
#define M4_DIR_PIN 3  // GPIO22  (RPi) corresponds to wiringPi pin 3
#define M4_PWM_PIN 4  // GPIO23  (RPi) corresponds to wiringPi pin 4

// these gpios are mapped stupid, see https://pinout.xyz/pinout/wiringpi

// State Variables
float steeringValue = 0;  // Smoothed steering input (-100 to 100)
float currentTrigger = 0; // Smoothed throttle input (0 to 100)
float previousSteeringValue = 0;
float previousTriggerValue = 0;

const float alpha = 0.2;  // Smoothing factor (0.1 = very smooth, 0.5 = more responsive)
const int deadzone = 5;   // Ignore small joystick movements
const int triggerThreshold = 2; // If below this, force trigger to zero
bool isReversed = false;  // Track drive direction

int main() {

    int steeringValue = 0;

    int motorDirectionLeft = 1;
    int motorDirectionRight = 1;

    // Instantiate control pad object
    XPad pad;
    
    // Initialize wiringPi
    if (wiringPiSetup() == -1) {
        std::cerr << "Setup failed!" << std::endl;
        return 1;
    }
    
    // Set up PWM output
    softPwmCreate(M1_PWM_PIN, 0, 100);
    softPwmCreate(M2_PWM_PIN, 0, 100);
    softPwmCreate(M3_PWM_PIN, 0, 100);
    softPwmCreate(M4_PWM_PIN, 0, 100);

    // Set up DIR output
    pinMode(M1_DIR_PIN, OUTPUT);
    pinMode(M2_DIR_PIN, OUTPUT);
    pinMode(M3_DIR_PIN, OUTPUT);
    pinMode(M4_DIR_PIN, OUTPUT);

    //Set initial direction of left and right sides
    digitalWrite(M1_DIR_PIN, !isReversed);
    digitalWrite(M2_DIR_PIN, !isReversed);
    digitalWrite(M3_DIR_PIN, isReversed);
    digitalWrite(M4_DIR_PIN, isReversed);

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

        // **Toggle Reverse Mode (Button B)**
        if (event.getSource() == XEventSource::BUTTON_B && event.isButtonPressed()) {
            if (currentTrigger < triggerThreshold) { // Safe to switch only when stopped
                isReversed = !isReversed;
                digitalWrite(M1_DIR_PIN, isReversed);
                digitalWrite(M2_DIR_PIN, isReversed);
                digitalWrite(M3_DIR_PIN, !isReversed);
                digitalWrite(M4_DIR_PIN, !isReversed);
                std::cout << "Drive Mode: " << (isReversed ? "REVERSED" : "FORWARD") << std::endl;
            }
        }

        if (event.getNativeType() == XEventType::EVENT_AXIS && (event.getSource() == XEventSource::ANALOG_RIGHT_TRIGGER || event.getSource() == XEventSource::ANALOG_LEFT_JOYSTICK_X))
        {

            if(event.getSource() == XEventSource::ANALOG_LEFT_JOYSTICK_X) // Y axis of left joystick
            {
                int rawSteering = event.getJoystickValue();
                // Apply deadzone
                if (abs(rawSteering) < deadzone)
                {
                    rawSteering = 0;
                }
                steeringValue = alpha * rawSteering + (1 - alpha) * steeringValue;
            }
            if (event.getSource() == XEventSource::ANALOG_RIGHT_TRIGGER) {
                int rawTrigger = event.getTriggerValue();
    
                // Apply smoothing, but force to 0 if below threshold
                if (rawTrigger < triggerThreshold) {
                    currentTrigger = 0; // Reset to zero when released
                }
                else
                {
                    currentTrigger = alpha * rawTrigger + (1 - alpha) * currentTrigger;
                }
            }

            // Process Motor Power Adjustments
            float scaleFactor = (steeringValue == 0) ? 0.0 : abs(steeringValue) / 100.0; 

            int pwmLeft = currentTrigger;  
            int pwmRight = currentTrigger;

            if (steeringValue < 0) {  
                pwmLeft = currentTrigger * (1.0 - scaleFactor);  
            } 
            else if (steeringValue > 0) {  
                pwmRight = currentTrigger * (1.0 - scaleFactor); 
            }

            // Apply PWM values only if change is significant
            if (abs(steeringValue - previousSteeringValue) > 1 || abs(currentTrigger - previousTriggerValue) > 1) {
                softPwmWrite(M1_PWM_PIN, pwmLeft);
                softPwmWrite(M2_PWM_PIN, pwmLeft);
                softPwmWrite(M3_PWM_PIN, pwmRight);
                softPwmWrite(M4_PWM_PIN, pwmRight);

                previousSteeringValue = steeringValue;
                previousTriggerValue = currentTrigger;
            }
        }

        std::cout << "\n";
    }
#pragma clang diagnostic pop
}