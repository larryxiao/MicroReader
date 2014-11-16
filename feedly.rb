#!/usr/bin/env ruby

require 'net/http'
require 'feedlr'
require 'debugger'
require 'json'
require 'fileutils'
require 'ap'

#uri = URI('http://cloud.feedly.com/v3/feeds/1')
#params = { :id => 1 }
#uri.query = URI.encode_www_form(params)

#res = Net::HTTP.get_response(uri)

#puts res.body
#puts res

client = Feedlr::Client.new(oauth_access_token: 'AgHNLm17ImEiOiJGZWVkbHkgRGV2ZWxvcGVyIiwiZSI6MTQyMzgxMTg4MTk0MiwiaSI6Ijg0NzI5NDQ3LTRiY2EtNGIwNC04NTRhLWQzNTNiNDRhMWNiOSIsInAiOjYsInQiOjEsInYiOiJwcm9kdWN0aW9uIiwidyI6IjIwMTQuNDYiLCJ4Ijoic3RhbmRhcmQifQ:feedlydev')

FileUtils.mkdir_p("output_json")

cats = client.user_categories
id = 0
cats.each do |category|
  data = client.stream_entries_contents("user/84729447-4bca-4b04-854a-d353b44a1cb9/category/#{category["label"]}", {count: 300})
  data["items"].each do |feed|
    puts "processing #{id}"
    id += 1
#uri = URI("http://www.readability.com/api/content/v1/parser?url=#{feed["alternate"][0]["href"]}&token=60082179dbe5bc0e7a7d38b8478ea9e90298cd1a")
#res = Net::HTTP.get_response(uri)
#json_res = JSON.parse(res.body)
    begin
    output_hash = {category: feed["categories"][0], timestamp: feed["published"], html_url: feed["alternate"][0]["href"], title: feed["title"]}
#output_hash = {category: feed["categories"][0], readability: json_res["content"], timestamp: feed["published"], html_url: feed["alternate"][0]["href"], title: feed["title"]}
    rescue
      debugger
      next
    end
    if feed["summary"] && feed["thumbnail"] && feed["visual"]
      output_hash["summary"] = feed["summary"]["content"] 
      output_hash["thumbnail_url"] = feed["thumbnail"][0]["url"]
      output_hash["logo_url"] = feed["visual"]["url"]
    else
      next
    end
    f = open("output_json/#{id}.json", "w+")
    f.puts(JSON.generate(output_hash))
    f.close
  end
end


