angular.module('app').controller('ConfigureController', function($scope, Devices, $location) {
  $scope.comName = Devices.getComName();

  $scope.configuration = Devices.getConfiguration();
  $scope.initialConfiguration = angular.copy($scope.configuration);

  console.log($scope.comName)
  console.log($scope.configuration)

  $scope.activeTabIndex = 0;
  $scope.selectTab = function(index) {
    $scope.activeTabIndex = index;
  }

  $scope.program = function() {
    Devices.program($scope.configuration).then(function success(r) {
      $scope.configuration = r.data;
      $scope.initialConfiguration = angular.copy($scope.configuration);
    });
  }

  $scope.diff = function() {
    return angular.equals($scope.configuration, $scope.initialConfiguration);
  }
});