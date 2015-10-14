angular.module('app').factory('Devices', function($http) {
  var comName = null,
      configuration = null;

  return {
    find: function() {
      return $http.get('/api/devices');
    },
    connect: function(device) {
      return $http.post('/api/device', {
        comName: device.comName
      }).then(function(r) {
        comName = device.comName;
        configuration = r.data;
      });
    },
    program: function(configuration) {
      return $http.put('/api/device', configuration);
    },
    getComName: function() {
      return comName;
    },
    getConfiguration: function() {
      return configuration;
    }
  }

})

