angular.module('starter.services', [])

/**
 * A simple example service that returns some data.
 */
.factory('Articles', function() {
  // Some fake testing data
  var articles = [
    { id: 0, date: 'December', title: 'Butter', abstract: 'These buttery, bacony cheesy scones are heaven in one easy bite. Perfect for brunch, they would also make for a great lunch alongside a bountiful salad. To take these scones over the top, add a pinch of grated cheese to the top of each one before you slide them into the oven.'},
    { id: 1, date: 'December', title: 'Butter', abstract: 'These buttery, bacony cheesy scones are heaven in one easy bite. Perfect for brunch, they would also make for a great lunch alongside a bountiful salad. To take these scones over the top, add a pinch of grated cheese to the top of each one before you slide them into the oven.'},
    { id: 2, date: 'December', title: 'Butter', abstract: 'These buttery, bacony cheesy scones are heaven in one easy bite. Perfect for brunch, they would also make for a great lunch alongside a bountiful salad. To take these scones over the top, add a pinch of grated cheese to the top of each one before you slide them into the oven.'},
    { id: 3, date: 'December', title: 'Butter', abstract: 'These buttery, bacony cheesy scones are heaven in one easy bite. Perfect for brunch, they would also make for a great lunch alongside a bountiful salad. To take these scones over the top, add a pinch of grated cheese to the top of each one before you slide them into the oven.'}
  ];

  return {
    all: function() {
      return articles;
    },
    get: function(articleId) {
      return articles[articleId];
    }
  };
})


.factory('Categories', function() {
  // Some fake testing data
  var categories = [
    {id: 0, name:"food"},
    {id: 1, name:"game"},
    {id: 2, name:"media"}
  ];

  return {
    all: function() {
      return categories;
    },
    get: function(id) {
      return categories[id];
    }
  };
});



