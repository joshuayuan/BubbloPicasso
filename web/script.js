var socket = io();
socket.on("server:website", (data) => console.log(data));
