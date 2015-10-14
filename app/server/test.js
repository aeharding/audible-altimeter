var events = require('events');

var com = require("serialport");

var parseStream = require("./parseStream");

function Connection(comName) {
  var self = this;

  events.EventEmitter.call(this);

  self.serialPort = new com.SerialPort(comName, {
    baudrate: 9600,
    parser: com.parsers.byteLength(124)
  });

  self.serialPort.on('open',function() {
    console.log('Port open');
  });

  self.serialPort.on('data', function(data) {
    self.rawConfiguration = data;
    console.log(data)
    self.configuration = parseStream.streamToJson(data);
    self.emit('data', self.configuration);
  });

  function done() {
      console.log('done');
      // serialPort.write(0xE0);
      // serialPort.write(0x01);
      // serialPort.write(0xff);
      // for (var i = 0; i < 120; i++) {
      //   serialPort.write(i);
      // }
      // serialPort.write(0xE7);
  }
}

Connection.prototype.program = function(configuration) {
  console.log('sending', parseStream.jsonToStream(configuration))
  this.serialPort.write(parseStream.jsonToStream(configuration));
}

Connection.prototype.__proto__ = events.EventEmitter.prototype;

module.exports = Connection;