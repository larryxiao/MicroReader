@json_dir = "/Users/Beaver/output_json" 

Dir.glob(@json_dir + "/*.json").each do |filename|
  data = JSON.parse(open(filename, "r:iso-8859-1:utf-8").read)
  all_labels = Category.all.map { |c| c.label }
  if data["category"].class.to_s == "Array"
    cate = data["category"][0]
  else
    cate = data["category"]["label"]
  end
  unless all_labels.include? cate 
    Category.create({label: cate}) 
  end
end

all_labels = Category.all.map { |c| c.label }

Dir.glob(@json_dir + "/*.json").each do |filename|
  data = JSON.parse(open(filename, "r:iso-8859-1:utf-8").read)
  if data["category"].class.to_s == "Array"
    cate = data["category"][0]
  else
    cate = data["category"]["label"]
  end
  Feed.create({html_url: data["html_url"], summary: data["summary"], title: data["title"], logo_url: data["logo_url"], thumbnail_url: data["thumbnail_url"], unix_timestamp: data["timestamp"], category_id: all_labels.index(cate) + 1})
end

ActiveUser.create({user_id: 1})
