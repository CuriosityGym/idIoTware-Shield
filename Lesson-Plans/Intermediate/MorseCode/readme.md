  **MORSE CODE**
    
    Morse code is a method of sending text messages by keying in a series of electronic pulses,
    usually represented as a short pulse (called a "dot") and a long pulse (a "dash"). The code 
    was devised by Samuel F. B. Morse in the 1840s
    
    In this example, we are using two NRF24L01 radio modules to transmit and receive Morse Code.
    On Transmitter side, Interpret presses on button (connected to digital pin 7) as Morse Code
    inputs and send it to receiver side using NRF24L01 radio.
    On Receiver side, NRF24L01 radio will receive the message and arduino will convert the signal 
    pattern consist of DOTs and DASHes to that particular letter and also print that letter to 
    OLED with its morse code.
