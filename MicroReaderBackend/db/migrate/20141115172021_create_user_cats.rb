class CreateUserCats < ActiveRecord::Migration
  def change
    create_table :user_cats do |t|
      t.decimal :avg_predict
      t.decimal :avg_action
      t.integer :user_id
      t.integer :category_id

      t.timestamps
    end
  end
end
