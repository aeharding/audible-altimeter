angular.module("app").directive("knob", function() {
  return {
    template: '<input class="knob" data-width="200" data-displayPrevious=false data-fgColor="white" data-step="50" data-max="7000" data-min="1000" data-skin="tron" data-cursor=true value="2000" data-thickness=".2">',
    link: function(scope, element, attrs) {
      element.find('.knob').knob({
          change : function (value) {
              //console.log("change : " + value);
          },
          release : function (value) {
              //console.log(this.$.attr('value'));
              console.log("release : " + value);
          },
          cancel : function () {
              console.log("cancel : ", this);
          },
          format : function (value) {
              return value + 'ft';
          },
          draw : function () {
              // "tron" case
              if(this.$.data('skin') == 'tron') {
                  this.cursorExt = 0.3;
                  var a = this.arc(this.cv)  // Arc
                      , pa                   // Previous arc
                      , r = 1;
                  this.g.lineWidth = this.lineWidth;
                  if (this.o.displayPrevious) {
                      pa = this.arc(this.v);
                      this.g.beginPath();
                      this.g.strokeStyle = this.pColor;
                      this.g.arc(this.xy, this.xy, this.radius - this.lineWidth, pa.s, pa.e, pa.d);
                      this.g.stroke();
                  }
                  this.g.beginPath();
                  this.g.strokeStyle = r ? this.o.fgColor : this.fgColor ;
                  this.g.arc(this.xy, this.xy, this.radius - this.lineWidth, a.s, a.e, a.d);
                  this.g.stroke();
                  this.g.lineWidth = 2;
                  this.g.beginPath();
                  this.g.strokeStyle = this.o.fgColor;
                  this.g.arc( this.xy, this.xy, this.radius - this.lineWidth + 1 + this.lineWidth * 2 / 3, 0, 2 * Math.PI, false);
                  this.g.stroke();
                  return false;
              }
          }
      });
    }
  }
});




