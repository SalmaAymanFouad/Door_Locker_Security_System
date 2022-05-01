# Door_Locker_Security_System

Developing a system to unlock a door using a password through 2 ATmega16 microcontrollers (HMI: Human Module Interface, and control). 
This project is created using layered architecture model concept(MCAL, HAL, APP) with dynamic configuration.

This HMI-ECU is the microcontroller responsible of interfecing with the user. The connected prepherals are keypad 4x4 and LCD 2x16.
When we first run the code on a simulator, "Enter Password" appears and the microcontroller will be waiting for the user to enter a 5 digits password which will appear 
on the LCD in form of asterisk '*' to maintain user's confidentiality. 
Then "Reenter Password" appears and he must the same entered 5 digits.
Compare both entered passwords and if they are matched HMI send the password to control using UART communication protocol, then store it in EEPROM using I2C communication protocol.
If the password doesn't match, the user is asked to enter the passwordand then reenter it to check the match...etc.

After the password is set, a main menu is displayed on the LCD: '+' operator is used to open the door, '-' operator is used to change password.





