# AM2302 sensor
This is a quick modern C++ library to read the am2302 in a bit banged fashion. Currently it still needs some work before it can be used in production.

## 2DO
  * Make a good abstraction for oscarIO
  * Do checksum checks
  * Fault handling (what if the sensor is disconnected halfway a read)
  * Make it so it's not depended on hwlib (a university library).