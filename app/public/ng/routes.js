angular.module('app').config(function($routeProvider, $locationProvider) {
  $routeProvider
   .when('/', {
    templateUrl: 'ng/partials/loading.html',
    controller: 'LoadingController'
  })
  .when('/configure', {
    templateUrl: 'ng/partials/configure.html',
    controller: 'ConfigureController'
  });

  // configure html5 to get links working on jsfiddle
  // $locationProvider.html5Mode(true);
});