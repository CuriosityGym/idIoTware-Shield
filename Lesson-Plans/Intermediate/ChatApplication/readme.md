#Chat Application Using Idiotware Shield

What if, we could share data without wires or connecting to the Internet?


How cool would it be if you could communicate with people inside your home or work place
without a net pack or Internet Connection? 


A Private wireless Chat Room maybe?


What if we could customize our chat-screen with our own imaginations?


All this is possible with a arduino, idIoTware shield and the Wireless module nRF24L01. 


This Arduino Chat Room uses nRF24L01 to set up a low cost Chat Room in your local area.
You can extend the chat coverage by using nrf24l01 radio module with an external antenna.












#Working


In this example we are using NRF24L01 radio module and arduino to communicate to the other arduino and Nrf24L01 radio module using the serial monitor.


Here we are creating local mesh between two radio modules(Nrf24L01).


The two sets of  Arduino and nRF24L01 are connected together to establish a wireless  communication link so that they could talk to each other.


The nRF24L01 are half duplex transceiver modules, hence they can send and receive data.


The data is sent from the user using arduino Serial monitor , by typing on his keyboard,and this data can be received and displayed on the receivers  oled display,  which  is on the idiotware shield, and vice- a versa.


This project is limited to just adding two members to the Chat room, But the nRF24L01 has capability to handle 6 connections,  and so there could be a maximum of 6 members in our chat room, each talking or chatting with each other.


This chat room coverage could be upto 100 meters depending upon the nRF24L01 Modules range.