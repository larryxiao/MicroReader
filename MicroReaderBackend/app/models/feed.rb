class Feed < ActiveRecord::Base
  belongs_to :category
  has_many :records
  has_many :users, through: :records
end
