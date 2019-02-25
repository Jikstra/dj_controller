# About

This is a arduino mega2560 project for a diy dj controller aimed to be compatible with the open source Mixxx Software. 

# Build Instruction

1. You need platformio installed. A `pip install platformio` should do the job for you.
2. Clone & cd into this repository, run `platformio run` to build this project. It should automatically download needed dependencies.
3. With `platformio run -t upload` you can upload the compiled firmeware to your connected arduino.
4. With `platformio run -t clean` you can clean the build.
5. By uncommenting the `#define DEBUG` line in `debug.h` you can enable debug output.  To get a serial monitor you can run `platformio device monitor`.

