// Ionic Starter App

// angular.module is a global place for creating, registering and retrieving Angular modules
// 'starter' is the name of this angular module example (also set in a <body> attribute in index.html)
// the 2nd parameter is an array of 'requires'
// 'starter.services' is found in services.js
// 'starter.controllers' is found in controllers.js
angular.module('starter', ['ionic', 'starter.controllers', 'starter.services'])

.run(function($ionicPlatform) {
  $ionicPlatform.ready(function() {
    // Hide the accessory bar by default (remove this to show the accessory bar above the keyboard
    // for form inputs)
    if(window.cordova && window.cordova.plugins.Keyboard) {
      cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);
    }
    if(window.StatusBar) {
      // org.apache.cordova.statusbar required
      StatusBar.styleDefault();
    }
  });
})

// .config(function($routeProvider, $httpProvider) {
//   $httpProvider.defaults.headers.common = {};
//   $httpProvider.defaults.headers.post = {};
//   $httpProvider.defaults.headers.put = {};
//   $httpProvider.defaults.headers.patch = {};
//   $httpProvider.defaults.useXDomain = true;
//   delete $httpProvider.defaults.headers.common['X-Requested-With'];
//   $routeProvider.
//       when('/user', {
//         templateUrl: 'templates/user-login.html',
//         controller: 'UserLoginCtrl'
//       });
// });

.config(function($stateProvider, $urlRouterProvider, $httpProvider) {
  $httpProvider.defaults.headers.common = {};
  $httpProvider.defaults.headers.post = {};
  $httpProvider.defaults.headers.put = {};
  $httpProvider.defaults.headers.patch = {};
  $httpProvider.defaults.useXDomain = true;
  delete $httpProvider.defaults.headers.common['X-Requested-With'];

  // Ionic uses AngularUI Router which uses the concept of states
  // Learn more here: https://github.com/angular-ui/ui-router
  // Set up the various states which the app can be in.
  // Each state's controller can be found in controllers.js
  $stateProvider

    // returning user.
    .state('user', {
      url: '/user',
      views: {
        'user': {
          templateUrl: 'templates/user-login.html',
          controller: 'UserLoginCtrl'
        }
      }
    })

    // new user.
    .state('user.reg', {
      url: '/reg',
      views: {
        'user-reg': {
          templateUrl: 'templates/user-reg.html',
          controller: 'UserRegCtrl'
        }
      }
    })

    // setup an abstract state for the tabs directive
    .state('tab', {
      url: "/tab",
      abstract: true,
      templateUrl: "templates/tabs.html"
    })

    // Each tab has its own nav history stack:

    .state('tab.dash', {
      url: '/dash',
      views: {
        'tab-dash': {
          templateUrl: 'templates/tab-dash.html',
          controller: 'DashCtrl'
        }
      }
    })

    .state('tab.dash-detail', {
      url: '/dash/:articleId',
      views: {
        'tab-dash': {
          templateUrl: 'templates/dash-detail.html',
          controller: 'DashDetailCtrl'
        }
      }
    })

    .state('tab.personal', {
      url: '/personal',
      views: {
        'tab-personal': {
          templateUrl: 'templates/tab-personal.html',
          controller: 'FriendsCtrl'
        }
      }
    })

    .state('tab.setting', {
      url: '/setting',
      views: {
        'tab-setting': {
          templateUrl: 'templates/user-reg.html',
          controller: 'AccountCtrl'
        }
      }
    })

    .state('tab.user-topic', {
      url: '/setting/topic',
      views: {
        'tab-setting': {
          templateUrl: 'templates/user-topic.html',
          controller: 'RegisterTopicCtrl'
        }
      }
    })
    ;

  // if none of the above states are matched, use this as the fallback
  $urlRouterProvider.otherwise('/tab/dash');

});
