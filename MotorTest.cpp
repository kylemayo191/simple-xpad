#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

#define PWM_PIN 26  // GPIO18 (BCM) corresponds to wiringPi pin 1

int main() {
    // Initialize wiringPi
    if (wiringPiSetup() == -1) {
        std::cerr << "Setup failed!" << std::endl;
        return 1;
    }

    // Set up PWM output
    softPwmCreate(PWM_PIN, 0, 100);
    
    while (true) {
        int speed;
        std::cout << "How fast? (20-100): ";
        std::cin >> speed;
        
        if (speed < 20 || speed > 100) {
            std::cout << "Invalid input. Enter a value between 20 and 100." << std::endl;
            continue;
        }
        
        softPwmWrite(PWM_PIN, speed);
    }
    
    return 0;
}
