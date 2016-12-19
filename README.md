# Doctor-Clock
An Arduino-powered digital alarm clock that tells the time using preselected images, with an emphasis on Doctor Who.

Hardware components used:
* Arduino MEGA 2560 microcontroller board
* Adafruit RA8875 graphics driver board
* Adafruit 5.0" touchscreen LCD
* Adafruit microSD card reader
* SanDisk 2GB microSD card
* Arduino UNO microcontroller
* SparkFun MP3 player shield
* Adafruit NeoPixel ring (12 RGB LEDs)
* SanDisk 8GB microSD card
* Donop DS3231 RTC (real-time clock) with CR2032 battery
* Speaker

YouTube video demonstrating the project:
https://www.youtube.com/watch?v=YkW0XUGsP00

NOTE: This video was made before the RTC and alarm features were added. These features were as of April 24, 2016. A second video filmed on June 15, 2016 was scrapped, as new updates to the software had rendered it outdated.

Timeline of development:
• First parts ordered: February 5, 2016
• First operation of parts: February 13, 2016
• Operation of first prototype: February 15, 2016
• Construction of wooden case: February 28, 2016 ¹
• Production/release of video linked above: April 24, 2016
• Addition of RTC: May 7, 2016
• Replacement and upgrade of main controller: May 11, 2016
• Addition of date display: May 13, 2016
• Began development of alarm feature: May 20, 2016
• Completion of alarm feature: approx. June 15, 2016 ²
• Completion of project: approx. July 20, 2016

¹ ² Followed by a gradual development of the clock's software.

Main controller (Arduino UNO Feb 5-May 11 and Arduino MEGA May 11-) is connected to power through a USB-B cable. The RA8875 graphics driver board is connected to 5" LCD through a 40-pin ribbon cable and to the main controller through SPI. Additionally connected through SPI is the microSD card reader, which has a microSD card inserted into it. RTC is connected to main controller through I2C, while NeoPixel ring is connected through a single digital pin. [Following May 20] Arduino UNO is connected to MP3 player shield, which is connected through a standard 3.5mm headphone jack to a speaker mounted to the case. Arduino UNO is connected to power through a separate USB-B cable, and the MP3 shield has its own microSD card containing multiple audio files.

Upon plugging in the main controller, an interface will appear on the screen. The design of this interfact depends on the software version. If using software before May 7, the interface will appear as five vertical bars; from left to right, they are red, green, cyan, yellow and purple. The purple bar is slightly wider than the rest. The red bar, by default, displays a "12," he green and cyan display a "0" and the yellow will display "AM." Tapping a bar will either increment it by one and reset once it has passed 12 or toggle between AN and PM. The purple bar confirms the time set by the user and starts the time display at that time. If using software after May 7, the screen will be divided into four equal-sized vertical bars, retaining the same color scheme but canceling out the purple bar. The red bar displays its normal hour setting at the top and the word "Alarm" at the bottom. The green bar displays its normal minute setting at the top and "None" at the bottom. The cyan bar displays its normal minute setting at the top and the word "Set" and the bottom. The yellow bar displays its normal "AM/PM" setting at the top and the word "Load" at the bottom. Tapping the top half of a bar will perform its normal function and tapping the bottom will execute a separate function. "Alarm" sets the alarm time to the time currently shown on the screen, "None" disables the alarm feature, "Set" changes the time and date on the RTC to the user's choice and "Load" loads the time and date from the RTC. Once the time has been set, the main controller and the RA8875 will draw the images respective to the current time from right to left, ending with the colon. This process takes approximately 20 seconds, but only appears that long if the hour is shifting, as images are not erased - only overwritten. The date can be seen below the time, previously in a "M/D/YYYY" format but currently in a "DOW, M D" format.
