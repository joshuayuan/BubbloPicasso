var socket = io();
socket.on("server:website", (data) => {
  $("#num_fingers").text(data);
});
