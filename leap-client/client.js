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
var palmPosition = [0, 0, 0]; // [x, y, z]
controller.on('deviceFrame', function(frame) {
  var currNumFingers = frame.fingers.length;
  if (numberOfFingers != currNumFingers) {
    numberOfFingers = currNumFingers;
    socket.emit("client:server", {"num_fingers": numberOfFingers});
    console.log(numberOfFingers);
  }

  if (frame.hands.length > 0) {
    for (var i = 0; i < frame.hands.length; i++) {
      var hand = frame.hands[i];
      var currPalmPosition = hand.palmPosition;
      if (vectorDist(currPalmPosition, palmPosition) > 5) {
        palmPosition = currPalmPosition;
        console.log(currPalmPosition);
        socket.emit("client:server", {"hand_pos": vectorToString(palmPosition)});
      }
    }
  }
});

function vectorDist(v1, v2) {
  return Math.sqrt(Math.pow(v1[0] - v2[0], 2) + Math.pow(v1[1] - v2[1], 2) + Math.pow(v1[2] - v2[2], 2));
};

function vectorToString(vector, digits) {
  if (typeof digits === "undefined") {
    digits = 1;
  }
  return "(" + vector[0].toFixed(digits) + ", "
             + vector[1].toFixed(digits) + ", "
             + vector[2].toFixed(digits) + ")";
};

