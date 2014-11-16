class User < ActiveRecord::Base
  has_many :records
  has_many :feeds, through: :records
  has_many :user_cats
end
