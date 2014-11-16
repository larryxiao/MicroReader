from flask import *
import numpy as np
import rec
import utils
import json


app = Flask(__name__)
gNumUser = 0
gNumArticle = 0
gRating = []
gPrediction = []

# intialization of server.
# the request should "application/json", the keys are
#   numUser -- number of user ids. (id start from 1)
#   numArticle -- number of article ides. (id start from 1)
#   rating -- response matrix
#   prediction -- predicted gRating matrix
@app.route('/', methods=['GET', 'POST', 'OPTIONS'])
def init():
  global gNumUser
  global gNumArticle
  global gRating 

  # obj = json.loads(request.json)
  try:
    obj = request.json

    gNumUser = obj['numUser']
    gNumArticle = obj['numArticle']
    gRating = obj['rating']

    if not obj.has_key('prediction'):
      gPrediction = rec.recommend(gRating, 
                                  utils.toStrAll(range(1, gNumUser+1)),
                                  utils.toStrAll(range(1, gNumArticle+1)))
    else:
      gPrediction = obj['prediction']

    return jsonify(status="OK", 
                 prediction=gPrediction)
  except Exception as e:
    print e

# update the gRating of a user.
# the request should "application/json", the keys are
#   user_id  -- user id
#   feed_id  -- article id
#   flag  -- response given by the user
@app.route('/update', methods=['GET', 'POST', 'OPTIONS'])
def update():
  try:
    if request.json == None:
      obj =  json.loads(request.data)
    else:
      obj = request.json
    userid = obj['user_id']
    articleid = obj['feed_id']
    resp = obj['flag']
    gRating[userid][articleid] += float(resp)
    gPrediction = rec.recommend(gRating, 
                                    utils.toStrAll(range(1, gNumUser+1)),
                                    utils.toStrAll(range(1, gNumArticle+1)))
    print gPrediction
    return jsonify(status="OK", 
                   prediction=gPrediction)
  except Exception as e:
    print e

# test of json.
@app.route('/json', methods=['GET', 'POST', 'OPTIONS'])
def testJson():
  if request.json == None:
    obj =  json.loads(request.data)
  else:
    obj = request.json
  print 'json', obj
  return jsonify(status="OK")  


if __name__ == "__main__":
  # app.run(debug = True)
  app.run(host='0.0.0.0', port=5001);
  client.close()
