# ESP-IDF Button Example

This project demonstrates how to handle button press events using the ESP-IDF framework.

It shows basic things such as **semaphore** usage, **RTOS task creation** and **ISR handling**.

When the button is pressed, an LED blinks in a predefined traffic sequence with blue being the yellow.

## Hardware diagram

![Fritzing Diag](../../esp_idf_examples/traffic_light_esp32/docs/images/esp_32_rgb_led_bb.png)

## Getting Started

To get started with this project, follow these steps:

1. Clone this repository to your local machine:

   ```bash
   git clone https://github.com/ucflumm/esp_idf_examples.git

2. Set up your ESP-IDF development environment by following the [official documentation](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html).

3. Open the folder of your chosen example in VSCode. File -> Open Folder (the example folder not the root folder)

4. If you have **ESP-IDF extension** setup properly then all you need to do is locate the **flame icon** on the bottom menu. (Build, Flash, Monitor)
