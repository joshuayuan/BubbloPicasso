var socket = io();

var right_points = [];
var left_points = [];
socket.on("server:website", (data) => {
  var hands = data["hand"];
  if (typeof hands !== "undefined") {
    let hand_type = hands["hand_type"];
    if (hand_type == "right") {
      // Scale from [-300, 300] to [0, 600]
      // Canvas size is 400x400
      let x = (parseInt(hands["hand_pos"]["x"]) + 300); // side to side
      let y = (parseInt(hands["hand_pos"]["y"]) + 300); // height
      let z = (parseInt(hands["hand_pos"]["z"]) + 300); // front to back
      right_points.push([x, y, z]);

    } else if (hand_type== "left") {
      // Scale from [-300, 300] to [0, 600]
      // Canvas size is 400x400
      let x = (parseInt(hands["hand_pos"]["x"]) + 300);
      let y = (parseInt(hands["hand_pos"]["y"]) + 300); // height
      let z = (parseInt(hands["hand_pos"]["z"]) + 300);
      left_points.push([x, y, z]);
    }
  }
});

var opaque_black;
var canvas;
function setup() {
  canvas = createCanvas(600, 600);
  canvas.parent("frame");
}

function draw() {
  background(255);

  strokeWeight(10);
  stroke(0, 0, 0, 50);

  for( let i = 1; i < right_points.length; i++) {
    if (dist(right_points[i][0], right_points[i][2], right_points[i-1][0], right_points[i-1][2]) < 20) {
      line(right_points[i-1][0], right_points[i-1][2], right_points[i][0], right_points[i][2]);
    }
  }
  for (let i = 1; i < left_points.length; i++ ) {
    if (dist(left_points[i][0], left_points[i][2], left_points[i-1][0], left_points[i-1][2]) < 20) {
      line(left_points[i-1][0], left_points[i-1][2], left_points[i][0], left_points[i][2]);
    }
  }
}

function stringCoords(ob) {
  return "(" + ob["hand_pos"]["x"] + ", " + ob["hand_pos"]["y"] + ", " + ob["hand_pos"]["z"] + ")";
}

$("#title").click(function() {
  console.log("tittt");
});

$("#download").click(function () {
  console.log("clicked");
});

function download() {
  saveCanvas("BubblePicasso", "png");
}

function restart() {
  right_points = [];
  left_points = [];
}
