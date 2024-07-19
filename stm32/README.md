BUILD
======
- Run **git submodule update --init** to checkout _libopencm3_ lib.
- Run **make TARGETS='stm32/f1' -C libopencm3**. You can omit targets to build all but it takes time.
- Go to project directory. For example - _blink_.
- Add **DEVICE** in _Makefile_. For example - _DEVICE=STM32F103C8T6_.
- Run **make**.
- All the built outout are placed inside project's _bin_ folder.

FLASH
======
[Caution] Only MacOS is tested.

Use st-link to flash the firmware to device. For that you need to install the _st-flash_ tool. This needs to be done once.
- Run **git clone _git@github.com:stlink-org/stlink.git**.
- For MacOS you need to check out tag version 1.7.0. **git checkout 179650**.
- To fix linker error edit **CMakeLists.txt** and add the following line inside **if (APPLE)**.

    **find_library(SECURITY_FRAMEWORK Security)**

    Add **${SECURITY_FRAMEWORK}** in **target_link_libraries(...)** at both places.
- Run **mkdir build**.
- Run **cp build**.
- Run "**cmake ..**".
- Run **make**.
- Copy **build/bin/st-flash** to **/usr/local/bin**.
- Run **make flash**.
    
    Optionally you can erase flash before reflashing using the following cmd.

    **make erase**

