var io = require('socket.io-client');
var socket = io('http://joshuayuan.me:8080');
var leapjs = require("leapjs");
const readline = require('readline');

const r1 = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

var controller = new leapjs.Controller();

controller.on("connect", () => console.log("Successfully connected."));
controller.on("deviceStreaming", () => console.log("A Leap device has been connected."));
controller.on("deviceStopped", () => console.log("A Leap device has been disconnected."));

controller.connect();

var numberOfFingers = 0;
controller.on('deviceFrame', function(frame) {
  var currNumFingers = frame.fingers.length;
  if (numberOfFingers != currNumFingers) {
    numberOfFingers = currNumFingers;
    socket.emit("client:server", numberOfFingers);
    console.log(numberOfFingers);
  }
});


