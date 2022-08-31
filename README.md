
# Smart LED strip

This is my pet project for smart LED strip at home. Current status: **beta**. 


## What it is all about?

I've always wanted to make my own home smart backlight. Firstly I've bought LED strips but I didn't like the way they enabled - I had to manually plug them into the socket and unplug when I left. With this project I don't have to think anymore about how bright LED will be or time it will switch off.

## How it works?

The main project - *brains* - is running on Raspberry Pi 3B which is always connected to the router. There's also Arduino UNO which powers transistor's bases through it's 4 separate PWM. Arduino is connected to Raspberry Pi via I2C bus and receives different commands like health check or color set.

So, every 2 seconds RPi pings Arduino and if there's a response it sets color according to the part of the day - less brighter at morning and evening and full brightness at day. Concurrently it pings specified IPs in my local net and if there's no response it sends to arduino signal to switch off LED strips. If device responds again - LED strips are switched on again according to the days part. This way ensures the conservation of electricity when no one is at home and backlight is useless.


## Languages stack
The main challenge I faced with - is that iPhone responds to ICMP pings only when it not being in sleeping state so I've started to ping it in another way by sending ARP requests but there's another question. I did't find any C++ library which can be integrated with two lines of code so I search Rust libs and found one! Therefore the main languages for this project are C++ and Rust.

## What's next?
I don't know. I'd want to make backlight animation but it's a bit difficult as it needs to change transistor's voltage concurrently. However I've learned a lot along the current project state. It's been a cool week. 

## License
Project is licensed under GNU General Public License v3.0 and you can use it how you want. 

See [COPYING](COPYING) for the full text.
