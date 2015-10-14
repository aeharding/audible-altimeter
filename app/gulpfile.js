'use strict';

var gulp = require('gulp');
var sass = require('gulp-sass');

gulp.task('sass', function () {
  gulp.src('./sass/index.scss')
    .pipe(sass({
      includePaths: require('node-bourbon').includePaths
    }).on('error', sass.logError))
    .pipe(gulp.dest('./public'));
});

gulp.task('default', ['sass'], function () {
  gulp.watch('./sass/**/*.scss', ['sass']);
});