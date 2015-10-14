angular.module('app').directive('altitudeRange', function() {
  return {
    templateUrl: 'ng/partials/altitudeRange.html',
    scope: {
      ngModel: '=',
      title: '@',
      type: '='
    },
    link: function(scope, element) {
      if (scope.type === 'freefall') {
        scope.min = 1000;
        scope.max = 7000;
        scope.step = 100;
      } else {
        scope.min = 0;
        scope.max = 1500;
        scope.step = 25;
      }

      scope.$watch('ngModel', function(newV) {
        element.find('input')[0].value = newV;
      })

      // ngModel doesn't work on input[type="range"]...
      element.find('input').on('input', function(e) {
        scope.ngModel = element.find('input')[0].value;
        scope.$apply();
      })
    }
  }
})