class CreateFeeds < ActiveRecord::Migration
  def change
    create_table :feeds do |t|
      t.string :html_url
      t.string :summary
      t.string :logo_url
      t.string :thumbnail_url
      t.integer :category_id
      t.integer :unix_timestamp
      t.string :title

      t.timestamps
    end
  end
end
