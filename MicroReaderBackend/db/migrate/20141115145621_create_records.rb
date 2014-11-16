class CreateRecords < ActiveRecord::Migration
  def change
    create_table :records do |t|
      t.integer :action
      t.decimal :predict
      t.integer :user_id
      t.integer :feed_id

      t.timestamps
    end
  end
end
