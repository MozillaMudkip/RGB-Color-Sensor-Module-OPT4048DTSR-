# RGB-Color-Sensor-Module-OPT4048DTSR-
Schematics, PCB and Code

This repository contains a collection of files that detail the creation of a RGB Color Sensor.

This color sensor module utilizes an OPT4048DTSR from Texas Instruments. This sensor is a High Speed High Precision Tristimulus XYZ Color Sensor. This sensor is able to perform high precision color to light digital conversion over an I2C interface. This sensor is able to measure 4 channels, 3 of which match and recognize the CIE tristimulus spectra with the 4th being a wide band spectral responce. This sensor is paired with an LTW-C191TS5 from Lite-On to provide sufficient ambient white light so the OPT4048DTSR can measure color values from reflected light of the object to be sorted.

This particular sensor module was created to be a part of an automatic sorter, built as my Engineering Capstone Design project. Particularly, this sensor will be a part of the primary sensor array which, paired with an inductive sensor and an OpenCV camera system, will be able to look at an input object and determine aspects relating to its color, shape and material. The sorter will then sort the object with parameters specified by the user into one of many containers. 

The OPT4048DTSR and and LED system takes in just a 3.3V input for power. Please note the the sensor has a minimum open air voltage range from 1.6V to 3.6V. 3.3V proved to be a convenient value for us as we use a 3.3 voltage source in numerous other portions of out project. For more information, please refer to the OPT4048DTSR datasheet which gives more specific allowances and typical setup and application.

https://www.ti.com/lit/ds/symlink/opt4048.pdf?ts=1698653299594&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FOPT4048 
