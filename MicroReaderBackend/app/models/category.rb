class Category < ActiveRecord::Base
  has_many :feeds
  has_many :user_cats
end
