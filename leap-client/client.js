var io = require('socket.io-client');
var socket = io('http://joshuayuan.me:8080');
const readline = require('readline');

const r1 = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

var readlineLoop = function () {
  r1.question("type something: ", (value) => {
    console.log("value is \'" + value + "\'");
    socket.emit("client:server", value);
    readlineLoop();
  });

};

socket.on('connect', function() {
  readlineLoop();
});
