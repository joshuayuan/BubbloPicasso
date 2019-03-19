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
socket.on("disconnect", function(resp) {
  console.log("socket disconnected:", resp);
});
socket.on("connect", function(resp) {
  console.log("socket connected");
});

var numberOfHands = 0;
var palmPosition = [0, 0, 0]; // [x, y, z]
controller.on('deviceFrame', function(frame) {
  var currNumHands = frame.hands.length;
  if (numberOfHands != currNumHands) {
    numberOfHands = currNumHands;
    socket.emit("client:server", {"num_hands": numberOfHands});
    console.log(numberOfHands);
  }

  if (numberOfHands > 0) {
    for (var i = 0; i < numberOfHands; i++) {
      var hand = frame.hands[i];
      var currPalmPosition = hand.palmPosition;
      if (vectorDist(currPalmPosition, palmPosition) > 10) {
        palmPosition = currPalmPosition;
        console.log(currPalmPosition);
        socket.emit("client:server", {"hand": {"hand_type": hand.type, "hand_pos": vectorToJson(palmPosition)}});
        // socket.emit("client:server", {"hand_pos": vectorToJson(palmPosition)});
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

function vectorToJson(vector) {
  return {"x": vector[0].toFixed(0), "y": vector[1].toFixed(0), "z": vector[2].toFixed(0)};
};


controller.on("streamingStopped", function(resp) {
  console.log("leap disconnected: ", resp);
});
