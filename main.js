var app = require("express")();
var server = require("http").Server(app);
var io = require("socket.io")(server);
server.listen(8080, () => {console.log("website connected")});

app.get('/', function (req, res) {
  res.sendFile(__dirname + "/web/index.html");
});
app.get("/script.js", function(req, res) {
  res.sendFile(__dirname + "/web/script.js");
});

io.on("connection", function(socket) {
  console.log("a client connected"); // website, or leap client
  socket.on("client:server", (data) => {
    console.log("server received \'", data, "\'");
    io.emit("server:website", data);
  });
});
