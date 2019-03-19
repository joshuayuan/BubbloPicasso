var socket = io();

var paths = [];
socket.on("server:website", (data) => {
  $("#num_fingers").text(data["num_fingers"]);
  if (typeof data["hand_pos"] !== "undefined") {
    $("#hand_pos_0").text(stringCoords(data));
    // Scale from [-300, 300] to [0, 600], then to [0, 400]
    // Canvas size is 400x400
    let x = (parseInt(data["hand_pos"]["x"]) + 300)/1.5;
    let z = (parseInt(data["hand_pos"]["z"]) + 300)/1.5;
    console.log(x, z);
    paths.push([x, z]);
  }
});

var opaque_black;
function setup() {
  createCanvas(400, 400);
}

function draw() {
  background(255);

  strokeWeight(10);
  stroke(0, 0, 0, 50);
  for( let i = 1; i < paths.length; i++) {
    // point(paths[i][0], paths[i][1]);
    line(paths[i-1][0], paths[i-1][1], paths[i][0], paths[i][1]);
  }
}

function stringCoords(ob) {
  return "(" + ob["hand_pos"]["x"] + ", " + ob["hand_pos"]["y"] + ", " + ob["hand_pos"]["z"] + ")";
}
