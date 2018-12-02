# Summary 

Gentle Wakeup starts with the ESP32 SNTP example and will build a test-first 
alarm clock that gradually turns on a light.

# Testing
Gentle Wakeup uses a ceedling based test suite to run host-based unit tests. 
This is separate from the esp-idf unit test application, which appears to run on
the coprocessor.

## Running the Tests
Default command is `tests:all` so to run all the tests just run `ceedling`


