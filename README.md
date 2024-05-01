# _ESP-IDF Examples & concepts_

My personal lists of concepts I feel that need commented examples for easier understanding.

## Requirements

Often just the setup and trying out examples turns off people using ESP-IDF. So to simplify the requirements the setup assumes and requires **VSCode** and the **ESP-IDF** extension already set up. There are plenty of guides on how to get to this stage.

## Instructions

* Clone the repo `` git clone https://github.com/ucflumm/esp_idf_examples ``

* Open VSCode

* Click on File->Open Folder...

#### Important

* Select the **example directory** which you wish to flash. **Do not open the root directory** otherwise when you try to flash it will break.

## project folder contents

The project **esp_idf_examples** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both).

Below is short explanation of remaining files in the project folder.

```
esp_idf_examples/
├─ traffic_light_esp32/
│  ├─ main/
│  │  ├─ CMakeLists.txt
│  ├─ CMakeLists.txt
│  ├─ README.md
│  ├─ components/
├─ serve_htmx_idf/
│  ├─ main/
│  │  ├─ CMakeLists.txt
│  ├─ CMakeLists.txt
│  ├─ README.md
│  ├─ components/
├─ README.md
```

#### More detailed explanation coming soon
