var serverUrl = 'http://172.27.221.134:3000';

angular.module('starter.controllers', [])


.controller('UserLoginCtrl', function($scope) {
  
})

.controller('UserRegCtrl', function($scope) {

})

.controller('RegisterController', function($scope, $http, $rootScope) {
  $scope.username = "";
  $scope.password = "";
  $scope.log = "";
  $scope.register = function() {
    console.log($scope.username);
    $http({
      url: serverUrl + '/get_all_categories.json',
      // url:'http://0.0.0.0:5001/json',
      method: "GET",
      params: {json:'{"username":"'+$scope.username+'"}'},
    }).success(function (data, status, headers, config) {
      // $scope.log = data['user_id'];
      $rootScope.user_id = data['user_id'];
      $rootScope.categories = data['category'];
      window.location = '#/tab/setting/topic';
    }).error(function (data, status, headers, config) {
      $scope.log = "";
      alert("fail");
    });
  };
})

.controller('RegisterTopicCtrl', function($scope, $rootScope, $http, $filter) {
  $scope.categories = $rootScope.categories;
  $scope.isChecked = new Object();
  $scope.cats = [];
  for(var key in $scope.categories) {
    $scope.cats[$scope.cats.length] = key;
    $scope.isChecked.key = 0;
  }

  $scope.setup = function() {
    var newcat = new Object();
    for(var key in $scope.categories) {
      var name = $scope.categories[key];
      console.log(name);
      if($scope.isChecked[name] == 1) {
        newcat[key] = name;
      }
    }
    console.log(newcat);
    $http({
      url: serverUrl + '/initialize_with_categories.json',
      // url:'http://0.0.0.0:5001/json',
      method: "GET",
      params: {
        json: JSON.stringify({"user_id": $rootScope.user_id,
                              "categories": newcat})
      },
    }).success(function (data, status, headers, config) {
      window.location = '#/tab';
    }).error(function (data, status, headers, config) {
      $scope.log = "";
      alert("fail");
    });
  };
})

.controller('DashCtrl', function($scope, $rootScope, $http, Articles) {
  $rootScope.articles = [];
  if(typeof $rootScope.active_feed_id == "undefined")
    $rootScope.active_feed_id = -1;
  $scope.articles = $rootScope.articles;
  var get_feed = function() {
    $http({
      url: serverUrl + '/get_best_feeds.json',
      method: "GET"
    }).success(function (data, status, headers, config) {
      console.log(data);
      for(var d in data.array) {
        $rootScope.articles[$rootScope.articles.length] = data.array[d];
        angular.element(document.querySelector('#abstract_0')).html(data.array[d].abstract);
      }
      console.log($scope.articles.length);
      console.log($rootScope.active_feed_id);
      if(data.is_active === true && $rootScope.active_feed_id != data.active_feed.id) {
        window.location = '#/tab/dash/' + data.active_feed.id;
        $rootScope.active_feed_id = data.active_feed.id;
      }
    }).error(function (data, status, headers, config) {
      $scope.log = "";
      alert("fail");
    });
  };
  get_feed();
  $scope.likeArticle = function(id) {
    $http({
      url: serverUrl + '/like_or_dislike.json',
      params: {
        json: JSON.stringify({"flag": 1,
                              "feed_id": id+1})
      },
      method: "GET",
    }).success(function (data, status, headers, config) {
      console.log(data);
      angular.element(document.querySelector('#main_html')).html(data.content);
      $scope.main_html = data.content;
    }).error(function (data, status, headers, config) {
      $scope.log = "";
      alert("fail");
    });
  };

  $scope.dislikeArticle = function(id) {
    $http({
      url: serverUrl + '/like_or_dislike.json',
      params: {
        json: JSON.stringify({"flag": -1,
                              "feed_id": id+1})
      },
      method: "GET",
    }).success(function (data, status, headers, config) {
      console.log(data);
      angular.element(document.querySelector('#main_html')).html(data.content);
      $scope.main_html = data.content;
    }).error(function (data, status, headers, config) {
      $scope.log = "";
      alert("fail");
    });
  };
})

.controller('DashDetailCtrl', function($scope, $rootScope, $http, $stateParams, Articles) {
  $scope.article = $rootScope.articles[$stateParams.articleId];
  console.log($scope.article);
  $http({
      url: 'http://www.readability.com/api/content/v1/parser?url='+$scope.article.url+'&token=60082179dbe5bc0e7a7d38b8478ea9e90298cd1a',
      method: "GET",
    }).success(function (data, status, headers, config) {
      console.log(data);
      angular.element(document.querySelector('#main_html')).html(data.content);
      $scope.main_html = data.content;
    }).error(function (data, status, headers, config) {
      $scope.log = "";
      alert("fail");
    });

})

.controller('DashPageCtrl', function($scope) {
  $scope.gotoArticle = function(id) {
    window.location = '#/tab/dash/' + id;
  };
})

.controller('FriendsCtrl', function($scope) {

})

.controller('AccountCtrl', function($scope) {
})

// .controller('ButtonCtrl', function($scope, $http) {
//   // $http({ url:'http://localhost:5001/json',
//   //         method: "GET",
//   //         headers: {'Content-Type': 'application/json',
//   //         msg: '123'}})
//   // .success(
//   //   function(data, status, headers, config) {
//   //     alert(data);
//   // })
//   // .error(
//   //   function(data, status, headers, config) {
//   // });
//   $http({
//     url: 'http://localhost:5001/json',
//     method: "POST",
//     data: '{"hi":"hi"}',
//     headers: {'Content-Type': 'application/x-www-form-urlencoded'}
//     }).success(function (data, status, headers, config) {
//         console.log(data);

//     }).error(function (data, status, headers, config) {});
//   $scope.hello = function() {
//   };
// })

.controller('PersonalTopicCtrl', function($scope, $http) {
  $scope.$watch(function(){
    return window.innerWidth;
  }, function(value) {
    $scope.innerWidth = value;
  });

  $scope.$watch(function(){
    return window.innerHeight;
  }, function(value) {
    $scope.innerHeight = value;
  });
})

.directive('ghVisualization', function ($http) {

  // constants
  var margin = 20,
    width = 960,
    height = 500 - .5 - margin,
    color = d3.interpolateRgb("#f77", "#77f");

  return {
    restrict: 'E',
    scope: {
    },
    link: function (scope, element, attrs) {
      var vis = d3.select(element[0])
        .append("svg")
          .attr("width", width)
          .attr("height", height + margin + 100);
      
      var diameter = innerWidth,
        format = d3.format(",d"),
        color = d3.scale.category20c();

      var bubble = d3.layout.pack()
          .sort(null)
          .size([diameter, diameter])
          .padding(1);


      $http({
        url: serverUrl + '/visualization.json',
        method: "GET",
      }).success(function (data, status, headers, config) {
        // d3.json("file:///Users/strin/Hack\!/PebbleRead/HackShanghai/ios/www/temp/bubble/flare.json", function(error, root) {
        // jdata = {
        //    "name": "flare",
        //    "children": [
        //       {"name": "CommunityStructure", 
        //        "children": [
        //       {"name": "HierarchicalCluster", "size": 6714}]},
        //     {"name": "AgglomerativeCluster", "size": 3938}]
        //   };
        jdata = data;
        console.log(data);

        var node = vis.selectAll(".node")
            .data(bubble.nodes(classes(jdata))
            .filter(function(d) { return !d.children; }))
            .enter().append("g")
            .attr("class", "node")
            .attr("transform", function(d) { return "translate(" + d.x + "," + d.y + ")"; });

        node.append("title")
            .text(function(d) { return d.className + ": " + format(parseInt(d.value)); });

        node.append("circle")
            .attr("r", function(d) { return d.r; })
            .style("fill", function(d) { return color(d.packageName); });

        node.append("text")
            .attr("dy", ".3em")
            .style("text-anchor", "middle")
            .text(function(d) { return d.className.substring(0, d.r / 3); });
        // });
        // Returns a flattened hierarchy containing all leaf nodes under the root.
        function classes(root) {
          var classes = [];

          function recurse(name, node) {
            if (node.children) node.children.forEach(function(child) { recurse(node.name, child); });
            else classes.push({packageName: name, className: node.name, value: parseInt(node.size)});
          }

          recurse(null, root);
          return {children: classes};
        }

      }).error(function (data, status, headers, config) {
        alert("fail");
        return;
      });

      
    }
   }
  });



