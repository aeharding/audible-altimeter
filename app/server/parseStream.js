var START = 0xE0;
var END = 0xE7;

var MODES = 4;
var ALARMS = 10;
var BYTES = 3;

function getAlarm(mode, alarm) {
  return mode * ALARMS * BYTES + alarm * BYTES + 3;
}


module.exports = {
  streamToJson: function parseStream(data) {
    if (data[0] !== START || data[123] !== END) {
      throw 'Device not recognized.';
    }
    if (data[1] !== 1 || data[2] !== 1) {
      throw 'Incorrect device';
    }

    var configuration = {};
    configuration.model = data[1];
    configuration.firmware = data[2];


    var modes = [];
    configuration.modes = modes;
    for (var i = 0; i < MODES; i++) {
      var mode = [];

      for (var j = 0; j < ALARMS; j++) {
        var index = getAlarm(i, j);

        mode.push({
          altitude: data[index] << 8 | data[index+1],
          type: (data[index+2] & 0x80) !== 0 ? 'freefall' : 'canopy',
          sound: data[index+2] & 0x07
        })
      }
      modes.push(mode);
    }


    return configuration;
  },
  jsonToStream: function(data) {
    if (data.model !== 1 || data.firmware !== 1) {
      throw 'Incorrect device.';
    }

    var arr = [START, 1, 1];

    for (var i = 0; i < MODES; i++) {
      for (var j = 0; j < ALARMS; j++) {
        var alarm = data.modes[i][j];
        arr.push(alarm.altitude >> 8); // msb
        arr.push(alarm.altitude & 0x1ff); // lsb

        arr.push(((alarm.type === 'freefall') << 7) + alarm.sound);
      }
    }

    arr.push(END)

    return new Buffer(arr);
  }
}