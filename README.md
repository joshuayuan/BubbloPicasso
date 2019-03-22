![Bubblo Picasso](/web/assets/title.png)
![Sparking creativity from the womb!](/web/assets/tagline.png)
This is the code repository for _Bubblo Picasso_, the second design provocation for Critical Making (New Media C203) Taught by Eric Paulos in Spring 2019.
We are Fang Fang, Rashad Timmons, Sonia Uppal, Joshua Yuan, and Debbie Yuen.

## Setup ##
1. Install [Nodejs](https://nodejs.org/en/download/)
2. Install the [LeapSDK](https://developer.leapmotion.com/sdk/v2) for the desktop controller.
  - Note that at the time of this writing, the LeapSDK must be installed on hardware with amd64 architecture, so not a Raspberry PI or ODROID XU4.
3. Clone this repository ```git clone https://github.com/joshuayuan/leapingbaby.git```
4. From inside this repository:
  - ```npm install socket.io socket.io-client express leapjs```
5. Make sure your Leap Motion Controller is plugged in and won't have errors running!

## Run ##
1. On your server: ```node main.js```
2. In [leap-client/client.js](/leap-client/client.js), change line 2 to point to your server and the appropriate port.
  - i.e. ```var socket = io('http://myserver.com:8080');```
3. On the machine which has the Leap connected to, run ```node leap-client/client.js```
4. Open the website in a browser. (e.g. http://myserver.com:8080)
5. Wave your hands around and create art!
