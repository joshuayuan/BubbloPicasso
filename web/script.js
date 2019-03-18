var socket = io();

var paths = [];
socket.on("server:website", (data) => {
  $("#num_fingers").text(data["num_fingers"]);
  if (typeof data["hand_pos"] !== "undefined") {
    $("#hand_pos").text(stringCoords(data));
    let x = data["hand_pos"]["x"]/1.5;
    let y = data["hand_pos"]["y"]/1.5;
    console.log(x, y);
    paths.push([x, y]);
    console.log(paths);
  }
});

function setup() {
  createCanvas(400, 400);
}

function draw() {
  background(200);

  for( let i = 1; i < paths.length; i++) {
    line(paths[i-1][0], paths[i-1][1], paths[i][0], paths[i][1]);
  }
}

function stringCoords(ob) {
  return "(" + ob["hand_pos"]["x"] + ", " + ob["hand_pos"]["y"] + ", " + ob["hand_pos"]["z"] + ")";
}
