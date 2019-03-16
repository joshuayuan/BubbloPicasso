var socket = io();
socket.on("server:website", (data) => {
  $("#num_fingers").text(data["num_fingers"]);
  $("#hand_pos").text(data["hand_pos"]);
});

