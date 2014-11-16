class CreateActiveFeeds < ActiveRecord::Migration
  def change
    create_table :active_feeds do |t|
      t.integer :feed_id

      t.timestamps
    end
  end
end
