# Project overview
This is a smart solution to control home appliances through two options :
* Web server through HTML page.
* Mobile app through UDP protocol.

# Features
* Each device can control up to 5 appliances.
* Unlimited number of devices can be installed as they connect with eachother over udp protocol.
* Each device can connect to a local area network (LAN) in station mode such as home wifi router and can generate its own LAN network in access point mode.
* The devices can be controlled from anywhere in the world through **static IP** address as long as they're connected to the internet.
* You can reboot, reset to defaults, change mode to ST or AP mode, change SSID & PW of the networks and change the IP address all from the mobile app.
# Components
* ESP8266
* Relays
* Resistors
* Diodes
* BC547 transistors
* Terminal blocks
* LEDs
* PCB
* 220v-5v adaptor
* 1 A fuse
# Tools and languages
* Android Studio (Flutter / Dart ).
* Arduino IDE (Arduino C).
* Proteus (PCB design).
# Code
* Due to the size of the code, feel free to check the ESP8266 code  [here](https://github.com/Eslam-Rizk/IOT-Smart-Home-system-/blob/main/Smart%20Home/Esp8266_code.ino/Esp8266_code.ino.ino)
* Due to the size of the code, feel free to check the flutter app [here]()
# PCB

Here is the process of making the PCB :

<img src="https://github.com/Eslam-Rizk/IOT-Smart-Home-system-/blob/main/Smart%20Home/board%20making%201.png" width=300>

and here is the final look:

<img src="https://github.com/Eslam-Rizk/IOT-Smart-Home-system-/blob/main/Smart%20Home/board%20making%202.png" width=300>

here is a 1-channel mini version of the device :

<img src="https://github.com/Eslam-Rizk/IOT-Smart-Home-system-/blob/main/Smart%20Home/1ch%20board%201.png" width=300>
<img src="https://github.com/Eslam-Rizk/IOT-Smart-Home-system-/blob/main/Smart%20Home/1ch%20board%202.png" width=300>

# New version !!!
This is a newer version that requires less wiring which makes the installation process faster and easier.
<img src="https://github.com/Eslam-Rizk/IOT-Smart-Home-system-/blob/main/Smart%20Home/board%20version2.png" width=300>

# User Interface
  There are 2 options to control the smart home device:
* First Through a **web server** hosted by the esp8266 IP address as shown below:
<img src="https://github.com/Eslam-Rizk/IOT-Smart-Home-system-/blob/main/Smart%20Home/20230725_214500.jpg" width=300>

* The other option is a **Flutter**-based mobile App that controls the devices over **UDP/IP**.
# Mobile App
This app displays location-based weather forcasts and time along with 8 custom-designed buttons to control the projects 8 devices.The App can be edited to add more buttons to control more devices.The App is designed on Android Studio **Flutter** project using **Dart** language.The App UI is shown below:

<img src="https://github.com/Eslam-Rizk/IOT-Smart-Home-system-/blob/main/SmartphoneApp/bbbb/Screenshot_20230725_162644.png" width=300>

# Code
* After creating a new flutter project i edited the **dependencies** in **pubsec.yaml** file to add the libraries i need to import as shown:
  
  ```
  dependencies:
  flutter:
    sdk: flutter
  udp: ^3.0.3
  get_ip: ^0.4.0
  path_provider: ^1.6.14
  http: ^0.12.2
  intl: ^0.15.8
  flutter_icons: ^1.0.0+1
  weather: ^1.2.3
  ```

* Then created two dart files inside **lib/pages** folder, [design.dart](https://github.com/Eslam-Rizk/IOT-Smart-Home-system-/blob/main/SmartphoneApp/bbbb/lib/pages/design.dart) to custom-design cool buttons and [designTest.dart](https://github.com/Eslam-Rizk/IOT-Smart-Home-system-/blob/main/SmartphoneApp/bbbb/lib/pages/designText.dart) to cutom-design buttons fonts.
* And here is the main [code](https://github.com/Eslam-Rizk/IOT-Smart-Home-system-/blob/main/SmartphoneApp/bbbb/lib/main.dart)
* The whole project can be found [here](https://github.com/Eslam-Rizk/IOT-Smart-Home-system-/tree/main/SmartphoneApp/bbbb).
# Notes
* Change IP address written as "xx.xx.xx.xx" in lines 38, 82 and 84 with your esp8266 IP address as shown:
  
  ```dart
  String destination = 'xx.xx.xx.xx';
  ```
* Change the **location code** and **city name** in line 39 with yours to get precise weather forcast as shown:

  ```dart
  String roomName, key = 'YourLocationKey', city = 'YourCityName';
  ```
* Change devices names D1,D2,..etc as desired.
  



