# Z-Uno FLiRS Wallswitch

This is a very simple, but effective FLiRS based wallswitch using the great little Zuno development board by Zwave.me.

A quick note, the one thing I cannot attest to is how the battery life will be over the long run. I have had it up for about 3 days, with multiple OTA firmware updates and so far battery is 100%. I am powering the zuno via 2400mAh 3.6V AA Lithium battery.

**Hardware**

PINS 17 - 20 : Connect to each of your switches.
GND : Connect to the "common" cable of your switches.

When pin 17/18/19/20 + GND are shorted, the device wakes up and detects a press of the wallswitch.

PIN 17 = Switch 1
PIN 18 = Switch 2
PIN 19 = Switch 3
PIN 20 = Switch 4

I set things up this way because for the life of me I could not get keyscan mode to work.

**Software / Z-Wave**

When Switch 1-4 is activated, it will send a BinarySwitch ON (255) or OFF (0) to Association group 1-4.

**FLiRS**

The reason I opted to go with FLiRS for this project is to keep the state of the wallswitch and gateway in sync.

The issue with many battery based wallswitches is that if you set for example switch 1 to ON using your gateway, the switch is sleeping and does not know about this change. So when you press the button on the wallswitch, it will attempt to turn the light ON even though its already ON.

In comes FLiRS with the save.

In my project, command parameters 64/65/66/67 map to switch 1/2/3/4. Yes, I should have done this using a binary switch command class, but I included this device with the parameter based setup and did not want to bother with an exclusion. Seeing as how quickly z-nuo development goes, this was just easier for me. I am guessing this may be a bit lighter on the battery as well as the packets are a tiny bit smaller, there is NO eeprom writing involved, etc. I use z-way-server as my gateway so I have a fair bit of customization available for me as far as gateway coding goes. But I am guessing most decent gateways should have the option to script in ACTION -> SET CONFIG PARAM logic.

So, to summarize:

Want to synchronize switch 1 to ON state?

Set parameter 64 to 255.

Want to syncronize switch 2 to OFF state?

Set parameter 65 to 0.

Etc...

**Inclusion / Exclusion**

I have also added inclusion / exclusion capability without having to use the service button on the zuno. I have not tested this, but the code looks like it should work. To activate learn mode (and thus inclusion / exclusion), press all 4 buttons at once and hold for AT LEAST 10 seconds. If you are excluding your associated switches will all toggle to ON to let you know exclusion has started. If you are including... Just keep holding for a MAXIMUM of 15 seconds.

I am putting this up online because maybe someone will find it useful. YMMV.

Enjoy!
