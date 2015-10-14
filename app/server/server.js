var serialPort = require("serialport");

var express = require('express');
var bodyParser = require('body-parser');
var path = require('path');

var Connection = require('./test')
var connection = null;

var app = express();

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.use(express.static('public'));

app.get('/api/devices', function(request, response){
  serialPort.list(function (err, ports) {
    response.send(ports);
  });
});

app.post('/api/device', function(request, response) {
  if (connection) {
    response.send(connection.configuration);
  } else {
    connection = new Connection(request.body.comName);

    var timeout = setTimeout(() => {
      connection.serialPort.close(() => {
        response.status(500).send({
          error: 'Device not recognized, try again.'
        });
        connection = null;
      });
    }, 3000);
    connection.once('data', function(data) {
      clearTimeout(timeout);
      console.log(data.modes)
      response.send(data);
    });
  }
});

app.put('/api/device', function(request, response) {
  if (!connection) {
    response.send({
      error: 'You must POST /api/device to establish a connection first.'
    })
  }
  connection.program(request.body);
  connection.once('data', function(data) {
    response.send(data);
  });
});

app.get('/', function(req, res) {
  res.sendFile(path.join(__dirname, '../public', 'index.html'));
});

app.listen(8000, function () {
    console.log('Express server started!!!');
});
