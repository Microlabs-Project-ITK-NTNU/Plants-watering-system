# VS Code with Platform IO

How-to:
1. Install Visual Studio Code
2. Create new profile for Platform IO
3. Install extension "PlatformIO IDE" from the extensions tab
4. Choose "Create new project" in the PlatformIO tab on the left
    - Board: NodeMCU-32S
    - Framework: Arduino (or ESP-IDF*)
5. Write your code in src/main.cpp
6. Choose "build" in the PlatformIO tab on the left
7. Choose "Upload and Monitor"
    - Sometimes you need to press it a few times for it to work

Then you can use Arduino functions: https://docs.arduino.cc/language-reference/en/functions/analog-io/analogRead/

* Difference between Arduino framework and ESP-IDF framework:
In Arduino you program in C++. You can use many predefined functions. Programming is convenient and fast. Arduino functions are usually less efficient, but it should not matter for a system like this.

In ESP-IDF you program in C. You program much "closer" to the computer. This is usually more tedious, but the results are more efficient (depending on your personal skills). Many also claim that you achieve a better understanding of how the microcontroller works by working at this level.

[ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html)
