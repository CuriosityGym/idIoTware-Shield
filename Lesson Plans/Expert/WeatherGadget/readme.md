#idIoTWare Shield Weather Gadget

#####**Attention: Please install all libraries from our Github Repository to enable this example to run.**

In this example we are using ESP8266-01 Wifi Module with esp-link firmware and an I2C OLED display
to show weather conditions of particular city, fetched from OpenWeatherMaps http://www.openweathermap.org/appid

WeatherDataApi: Heroku Deployment Application that reduces data downloaded for OpenWeatherMap Api
Due to memory limits on the Arduino, an intermediate server filters data recieved from OpenWeatherMaps.
The python code can be accessed here https://github.com/CuriosityGym/WeatherDataApi



To get data from OpenWeatherMap Api, you need to create openweathermaps account and then generate API key, and use that API Key in this code.
You can read on how to do it here http://www.openweathermap.org/appid                

In this example we have used ELClient.h library to get data from WeatherDataApi. 
We are sending requests for location, current temperature, humidity and weather description. 
This code allows you to detect your location
1) Automatically by IP address (default behaviour)
2) To get weather conditions of particular city, change the variable cityID in the following code and change autoDetectLocation variable to false. cityID can be searched on openweathermaps

An i2c OLED is used to display this information.

We are sending request after every one hour to get weather information.To change this interval, change value of variable refreshRate. 
