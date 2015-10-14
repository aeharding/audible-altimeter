angular.module('app').controller('LoadingController', function($scope, Devices, $location) {
  $scope.data = {};

  Devices.find().then(function success(r) {
    $scope.data.devices = r.data;
    $scope.data.selected = r.data[0];
  });

  $scope.connect = function success(device) {
    $scope.loading = true;
    Devices.connect(device).then(function() {
      $location.path('/configure');
    }, function error(r) {
      $scope.loading = false;
      $scope.error = r.data.error;
    });
  }
});