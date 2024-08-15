# IoT Self-watering system
My first IoT project implemented for presentation at University.

## Hardware materials: 
  - NodeMCU ESP8266 
  - Breadboard
  - Arduino I2C screen
  - Relay module 5V
  - Capacitive soil moisture sensor
  - Mini water pump
  - Water pipe
  - Battery case
  - 4 AA battery
  - Jumper wires (Male to Female and Male to Male)

## Software components: 
  - Visual Code
  - PlatformIO (extension for VS Code)
  - Libraries on PlatformIO (for LCD screen and Firebase database)
  - Firebase account and project with realtime database

## Directory explanation:
  - Frontend - web application interface (HTML, CSS and JavaScript)
  - NodeMCU ESP8266 code - folder with all requirement components including libraries and .cpp file 

## Usage
Soil moisture sensor loads value and if a value is under the humidity limit, the pump starts on. Users can follow all changes on a web application (information about the value of the soil sensor, percentage of soil moisture and state of pump).

##Goal of the project
Using this solution, the project's main goal is to save water and increase the process of watering plants in different areas.
  
## Find a bug?
Feel free to contact me if you find an issue or would like to submit an improvement to this project. 
