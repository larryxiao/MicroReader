# encoding: utf-8
class CategoriesController < ApplicationController
  skip_before_filter :verify_authenticity_token, :only => [:get_all_categories, :initialize_with_categories]  

  def get_all_categories
    data = JSON.parse(params["json"]) 
    all_usernames = User.all.map { |u| u.username} 
    unless all_usernames.include? data["username"]
      User.create({username: data["username"]})
      user_id = User.find_by_username(data['username']).id
      Feed.all.each do |f|
        Record.create({user_id: user_id, feed_id: f.id, action:0, predict: 0}) 
      end
      Category.all.each do |t|
        UserCat.create({user_id: user_id, category_id: t.id, avg_predict:0, avg_action: 0})
      end
    end
    ActiveUser.first.update_attributes(user_id: User.find_by_username(data["username"]).id)
    all_cats = Category.all
    response = Hash.new
    response[:user_id] = User.find_by_username(data["username"]).id
    category_hash = Hash.new
    all_cats.each do |c|
      category_hash[c.id] = c.label
    end
    response[:category] = category_hash 
    respond_to do |format|
      format.json { render json: JSON.generate(response) }
    end
  end

  def initialize_with_categories
    data = JSON.parse(params["json"])
    data["categories"].each do |c|
      cat = Category.find(c[0].to_i)
      like_feeds = cat.feeds.sample((cat.feeds.length * 0.3).to_i)
      like_feeds.each do |f|
        Record.find_by_user_id_and_feed_id(active_user_id, f.id).update_attributes({action: 1})
      end
    end
    recompute_scores
    post_action_and_save_predict
    response = Hash.new
    response[:msg] = "success"
    respond_to do |format|
      format.json { render json: JSON.generate(response) }
    end
  end

  def post_action_and_save_predict
    init_hash = Hash.new
    init_hash[:numUser] = User.all.length
    init_hash[:numArticle] = Feed.all.length 
    action_hash = Hash.new { Hash.new }
    User.all.each_with_index do |u, i|
      action_hash_temp = Hash.new
      Feed.all.each_with_index do |f, j|
        r = Record.find_by_user_id_and_feed_id(u.id, f.id) 
        action_hash_temp[j] = r.action
      end
      action_hash[i] = action_hash_temp
    end
    init_hash[:rating] = action_hash
    json_headers = {"Content-Type" => "application/json",
                "Accept" => "application/json"}
    uri = URI.parse('http://172.27.221.133:5001/')
    http = Net::HTTP.new(uri.host, uri.port)
    response = http.post(uri.path, init_hash.to_json, json_headers)
    data = JSON.parse(response.body)
    data["prediction"].each_pair do |user_id, feed_id_and_prediction|
      feed_id_and_prediction.each_pair do |feed_id, prediction|
        Record.find_by_user_id_and_feed_id(user_id, feed_id).update_attributes({predict: prediction})
      end
    end
  end

  def recompute_scores
    Category.all.each do |cat|
      records = cat.feeds.map { |f| Record.find_by_user_id_and_feed_id(active_user_id, f.id) }
      total_action = records.map { |r| r.action}.reduce(0) { |accum, i| accum + i }
      total_predict = records.map { |r| r.predict}.reduce(0) { |accum, i| accum + i }
      uc = UserCat.find_by_user_id_and_category_id(active_user_id, cat.id)
      uc.update_attributes({avg_action: total_action.to_f / records.length, avg_predict: total_predict / records.length })
    end
  end

  def pebble_get_feed
    feeds = User.find(active_user_id).feeds.select { |f| Record.find_by_user_id_and_feed_id(active_user_id, f.id).action == 0 } 
    best_feed = feeds.zip(feeds.map { |f| Record.find_by_user_id_and_feed_id(active_user_id, f.id).predict}).sort_by(&:last).map(&:first).last
    response = Hash.new
    response[:summary] = handle_string(best_feed.summary)
    if active_feed_id
      ActiveFeed.first.update_attributes({feed_id: best_feed.id})
    else
      ActiveFeed.create({feed_id: best_feed.id})
    end
    respond_to do |format|
      format.json { render json: JSON.generate(response) }
    end
  end

  def active_user_id
    ActiveUser.first.id
  end

  def active_feed_id
    ActiveFeed.first ? ActiveFeed.first.id : false
  end

  def handle_string(s)
	  s = HTMLEntities.new.decode(s);
	  ss = "";
	  num = s.length - 1;
	  state = 0;
	  for i in 0..num
		  if state == 0
			  if s[i] == "<"
				  state = 1
			  elsif s[i] == "\\"
				  state = 2
			  else
				  if s[i] != "\n" && s[i] != "\t" && s[i] != "\r"
					  ss = ss + s[i]
				  end
			  end
		  elsif state == 1
			  if s[i] == ">"
				  state = 0
			  end
		  else
			  if s[i] != "n" && s[i] != "\\" && s[i] != "t" && s[i] != "r" && s[i] != "\""
				  ss = ss + "\#{s[i]}"
			  end
			  state = 0;
		  end				
	  end
	  return ss;
  end 

  def get_best_feeds
    feeds = User.find(active_user_id).feeds.select { |f| Record.find_by_user_id_and_feed_id(active_user_id, f.id).action == 0 } 
    sorted_feeds = feeds.zip(feeds.map { |f| Record.find_by_user_id_and_feed_id(active_user_id, f.id).predict}).sort_by(&:last).map(&:first)
    response = Hash.new
    response[:array] = []
    sorted_feeds.last(10).each do |f|
      temp_hash = Hash.new
      temp_hash[:id] = f.id
      temp_hash[:title] = f.title
      temp_hash[:abstract] = f.summary
      temp_hash[:url] = f.html_url
      temp_hash[:thumbnail] = f.thumbnail_url
      temp_hash[:numLike] = f.records.map { |r| r.action }.reduce(0) { |accum, i| accum + i }      
      temp_hash[:category] = f.category.label
      response[:array] << temp_hash
    end
    response[:is_active] = active_feed_id ? true : false
    if active_feed_id
      temp_hash = Hash.new
      temp_hash[:id] = active_feed_id
      f = Feed.find(active_feed_id)
      temp_hash[:title] = f.title
      temp_hash[:abstract] = f.summary
      temp_hash[:url] = f.html_url
      temp_hash[:thumbnail] = f.thumbnail_url
      temp_hash[:numLike] = f.records.map { |r| r.action }.reduce(0) { |accum, i| accum + i }      
      temp_hash[:category] = f.category.label
      response[:active_feed] = temp_hash
    end
    respond_to do |format|
      format.json { render json: JSON.generate(response) }
    end
  end

  def like_or_dislike_ios
    data = JSON.parse(params['json']) 
    post_hash = Hash.new
    post_hash[:flag] = data["flag"].to_s
    post_hash[:user_id] = active_user_id.to_s
    if data["feed_id"]
      post_hash[:feed_id] = data["feed_id"].to_s 
    else
      post_hash[:feed_id] = active_feed_id.to_s
    end
    json_headers = {"Content-Type" => "application/json",
                "Accept" => "application/json"}
    uri = URI.parse('http://172.27.221.133:5001/update')
    http = Net::HTTP.new(uri.host, uri.port)
    response = http.post(uri.path, post_hash.to_json, json_headers)
    res_data = JSON.parse(response.body)
    res_data["prediction"].each_pair do |user_id, feed_id_and_prediction|
      feed_id_and_prediction.each_pair do |feed_id, prediction|
        Record.find_by_user_id_and_feed_id(user_id, feed_id).update_attributes({predict: prediction})
      end
    end
    respond_to do |format|
      format.json { render json: { msg: "success" }}
    end
  end

  def like_or_dislike_pebble
    post_hash = Hash.new
    post_hash[:flag] = params[:flag].to_s
    post_hash[:user_id] = active_user_id.to_s
    post_hash[:feed_id] = active_feed_id.to_s
    json_headers = {"Content-Type" => "application/json",
                "Accept" => "application/json"}
    uri = URI.parse('http://172.27.221.133:5001/update')
    http = Net::HTTP.new(uri.host, uri.port)
    response = http.post(uri.path, post_hash.to_json, json_headers)
    res_data = JSON.parse(response.body)
    res_data["prediction"].each_pair do |user_id, feed_id_and_prediction|
      feed_id_and_prediction.each_pair do |feed_id, prediction|
        Record.find_by_user_id_and_feed_id(user_id, feed_id).update_attributes({predict: prediction})
      end
    end
    pebble_get_feed
  end

  def visualization
    response = Hash.new
    response[:active_user] = active_user_id
    uc_chain = []
    User.find(active_user_id).user_cats.each do |uc|
      uc_chain << [uc.category.label, uc.avg_action]
    end
    sorted_uc_chain = uc_chain.sort_by(&:last)
    respond_to do |format|
      format.json { render json: recursive_output(sorted_uc_chain, 1, 0).to_json}
    end
  end

  def recursive_output(chain, level, index)
    if index == (chain.length - 1)
      return {name: level, children: [{name: chain[index][0], size: subs(chain[index][1])}]}
    else
      return {name: level, children: [recursive_output(chain, level+1, index+1), {name: chain[index][0], size: subs(chain[index][1])}]}
    end
  end

  def subs x
    (x + 1) * 1000
  end

end
