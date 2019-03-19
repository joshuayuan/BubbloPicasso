var express = require("express");
var app = express();
var server = require("http").Server(app);
var io = require("socket.io")(server);
server.listen(8080, () => {console.log("website connected")});

app.use(express.static('web'));
app.get('/', function (req, res) {
  res.sendFile(__dirname + "/web/index.html");
});
app.get("/script.js", function(req, res) {
  res.sendFile(__dirname + "/web/script.js");
});
app.get("/p5.min.js", function(req, res) {
  res.sendFile(__dirname + "/web/p5.min.js");
});
app.get("/styles.css", function(req, res) {
  res.send(__dirname + "/web/styles.css");
});

io.on("connection", function(socket) {
  console.log("a client connected"); // website, or leap client
  socket.on("client:server", (data) => {
    console.log("server received \'", data, "\'");
    io.emit("server:website", data);
  });

  socket.on("disconnect", () => console.log("a client disconnected"));
});
