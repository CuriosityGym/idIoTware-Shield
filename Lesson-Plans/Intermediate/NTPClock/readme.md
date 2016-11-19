#NTP Clock

Most computers today use the NTP Clock to synchronise their internal clocks to an accurate atomic clock, 
which presents its time on the web.

#Working
In this example, we are going to display the Time, Date and Temperature readings on OLED display.
We need to setup the timezone first. 
By using idiotware.herokuapp.com, we are able to get the current zone  of the device, by using IP address. 
Once we get the timezone, we request timeapi.org to get the current time and date.
Once we get the data, we are ready to display it on OLED display.


Upload the code and you will be able to see the clock on the display with the current Time,date and temperature readings.

