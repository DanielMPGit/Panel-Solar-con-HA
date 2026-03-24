This project was not developed entirely from scratch.  
The original Arduino code, provided by the Keyestudio kit,  
was modified and expanded by adding new functions and additional improvements.

<br>

The system uses a solar tracker with four LDR sensors that detect light and adjust  
the panel’s position using servomotors to always stay at the optimal angle. It also includes sensors such as the DHT11, which measures temperature and humidity, and an LCD screen to display basic data. To make the project more complete, we added an Orange Pi, which collects all the information through an ESP32 and displays it in Home Assistant, allowing easy and organized access from any device on the network.  

<br>
<p align="center">
  <img src="readme/gif1.webp" width="400">
</p>
<br>

To keep everything running smoothly, we also included Portainer, which  
lets us manage the Docker containers where the applications run, and  
Nextcloud, which acts as a private cloud to store files. All of this can be accessed from a web page we set up, which provides quick links to Home Assistant, Portainer, and Nextcloud, making the system easy to use.  

<br>
<p align="center">
  <img src="readme/gif2.webp" width="800">
</p>
<br>
