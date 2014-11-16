from scikits.crab import datasets
from scikits.crab.recommenders.knn.item_strategies import AllPossibleItemsStrategy, ItemsNeighborhoodStrategy
from scikits.crab.recommenders.svd.classes import MatrixFactorBasedRecommender
from scikits.crab.models.classes import  MatrixPreferenceDataModel, MatrixBooleanPrefDataModel

movies = datasets.load_sample_movies()
data = movies['data']

def recommend(data, users, items):
  # print 'data = ', data
  matrix_model = MatrixPreferenceDataModel(data)

  items_strategy = AllPossibleItemsStrategy()
  # items_strategy = ItemsNeighborhoodStrategy()
  recsys = MatrixFactorBasedRecommender(
          model=matrix_model,
          items_selection_strategy=items_strategy,
          n_features=2)

  score = dict()
  for user in users:
    row = dict()
    for item in items:
      try:
        if data[user][item] != 0:     # the user has already rated.
          row[item] = data[user][item]
        else:
          row[item] = recsys.estimate_preference(user, item)
      except:
        row[item] = 0
    score[user] = row

  # print 'score = ', score
  return score

if __name__ == "__main__":
  data = {u'1': {u'11': 1, u'13': 1, u'12': 1, u'1': 2, u'3': 1, u'2': 2, u'4': 1}, u'2': {u'13': 2, u'12': 2, u'5':2, u'1': 2, u'3': -1, u'2': 1, u'4': 1}}
  # print data
  print 'rec = ', recommend(data, ['1'], ['5'])


