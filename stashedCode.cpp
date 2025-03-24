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
    // if (event.getSource() == XEventSource::BUTTON_B && event.isButtonPressed()) {
    //     std::cout << "Switching motor direction\n";
    //     if(motorDirectionLeft == 1)
    //     {
    //         motorDirectionLeft = 0;
    //         motorDirectionRight = 1;
    //         digitalWrite(M1_DIR_PIN, motorDirectionLeft);
    //         digitalWrite(M2_DIR_PIN, motorDirectionLeft);
    //         digitalWrite(M3_DIR_PIN, motorDirectionRight);
    //         digitalWrite(M4_DIR_PIN, motorDirectionRight);
    //     }
    //     else
    //     {
    //         motorDirectionLeft = 1;
    //         motorDirectionRight = 0;
    //         digitalWrite(M1_DIR_PIN, motorDirectionLeft);
    //         digitalWrite(M2_DIR_PIN, motorDirectionLeft);
    //         digitalWrite(M3_DIR_PIN, motorDirectionRight);
    //         digitalWrite(M4_DIR_PIN, motorDirectionRight);
    //     }
        
    //     // if(motorDirection == 0)
    //     // {
    //     //     motorDirection = 1;
    //     //     digitalWrite(M1_DIR_PIN, motorDirection);
    //     //     digitalWrite(M2_DIR_PIN, motorDirection);
    //     //     digitalWrite(M3_DIR_PIN, motorDirection);
    //     //     digitalWrite(M4_DIR_PIN, motorDirection);

    //     // }
    //     // else
    //     // {
    //     //     motorDirection = 0;
    //     //     digitalWrite(M1_DIR_PIN, motorDirection);
    //     //     digitalWrite(M2_DIR_PIN, motorDirection);
    //     //     digitalWrite(M3_DIR_PIN, motorDirection);
    //     //     digitalWrite(M4_DIR_PIN, motorDirection);

    //     // }
    // }
//}


            // //Analog
            // const XEventSource Sources[] = {
            //         XEventSource::ANALOG_LEFT_JOYSTICK_X,
            //         XEventSource::ANALOG_LEFT_JOYSTICK_Y,

            // };

            


            // for (const auto &joystickSource: Sources) {
            //     if (event.getSource() == joystickSource) {
            //         std::cout << "\t[Joystick] Position: " << event.getJoystickValue() << "\n";
            //     }
            // }

            // Trigger
            // const XEventSource triggerSources[] = {
            //         XEventSource::ANALOG_LEFT_TRIGGER,
            //         XEventSource::ANALOG_RIGHT_TRIGGER,
            // };
            // for (const auto &triggerSource: triggerSources) {
            //     if (event.getSource() == triggerSources[0]) {
            //         std::cout << "\t[Left Trigger] Position: " << event.getTriggerValue() << "\n";

            //        // softPwmWrite(M1_PWM_PIN, 50 + (event.getTriggerValue()/2));

            //     }
            //     else
            //     {
            //         int currentTrigger = event.getTriggerValue();
            //         //float scaleFactor = abs(steeringValue) / 100.0; // Convert -100 to 100 range into 0.0 to 1.0
            //         float scaleFactor = (steeringValue == 0) ? 0.0 : abs(steeringValue) / 100.0;

            //         int pwmLeft = 0;
            //         int pwmRight = 0;

            //         std::cout << "\t[Right Trigger] Position: " << event.getTriggerValue() << "\n";
            //         // Ramp to smooth trigger inputs
            //         if(currentTrigger >= 5 && abs(steeringValue) <= 10)
            //         {
            //             pwmLeft = currentTrigger;
            //             pwmRight = currentTrigger;
            //         }
            //         else if(currentTrigger >= 5 && abs(steeringValue) > 10)
            //         {
            //             if(steeringValue < 0)
            //                 pwmLeft = currentTrigger * (1.0 - scaleFactor);
            //             else
            //                 pwmRight = currentTrigger * (1.0 - scaleFactor);
            //         }
            //         else
            //         {
            //             pwmLeft = 0;
            //             pwmRight = 0;
            //         }

            //         softPwmWrite(M1_PWM_PIN, pwmLeft);
            //         softPwmWrite(M2_PWM_PIN, pwmLeft);
            //         softPwmWrite(M3_PWM_PIN, pwmRight);
            //         softPwmWrite(M4_PWM_PIN, pwmRight);
            //     }
            // }

            // // DPAD
            // if (event.getSource() == XEventSource::DPAD_Y || event.getSource() == XEventSource::DPAD_X) {
            //     std::cout << "\t[DPAD] Direction: " << event.getDpadDirection() << "\n";
            // }