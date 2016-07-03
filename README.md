# Temperature Based Fan Control For Time Capsule

An Arduino based controller board to automatically adjust
a fan to cool down an Apple Time Capsule.
Because: There is hope, that a cooled hard drive lives longer ;-)

See also Google's study [Failure Trends in a Large Disk Drive Population](http://static.googleusercontent.com/media/research.google.com/en/archive/disk_failures.pdf)

### Bill Of Material

* 1x Arduino Nano (Atmege 328P, 16MHz)
* 2x Temperature Sensor LM35, To-92 package e.g. [LM35DZ](http://www.ebay.com/sch/i.html?_nkw=LM35DZ) (0°C..100°C)
   * used together with a R-C Damper, see [manual's Figure 4](http://www.watterott.com/download/LM35.pdf)
* 1x Mosfet (which works at logic level), e.g. IRLR/U120NPbF (LU120N, MOSFET N-LogL TO-251AA 100V 10A)
* 2x Resistor: 75 Ohm
* 1x Resistor: 220 Ohm
* 1x Resistor: 10k Ohm
* 2x Condensator: 1 microFarad
* 1x Diode: e.g. 1N4001
* 1x 3-pin Molex female connector (fit's for fan)
* 1x 5V to 12V DC-DC boost module (StepUp converter), e.g. [XL6009](http://www.ebay.com/sch/i.html?_nkw=XL6009)
* 1x 40x40mm Fan, 12V, e.g. Noiseblocker BlackSilentFan XM1

##### Remarks, what fan to choose

During testing I used an ordinary 12V fan (which I had on spare).
The PWN control worked very well.
BUT when I used the Noiseblocker BlackSilentFan XM1, the fan made 'piep' noise.
I think this is related to the XM1's internal electronic, which somehow affects the PWM signal.
My bottom line: For the XM1 speed controll, I just use on/off semantics and no intermediate speed.


### Schematics

![PCB](temperature-controller_pcb.png?raw=true)


### Pictures

![PCB and Time Capsule](20151025_235245.jpg?raw=true)


##### License

[The MIT License (MIT)](https://opensource.org/licenses/MIT)