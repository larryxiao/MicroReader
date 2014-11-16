PebbleReader::Application.routes.draw do
  match '/get_all_categories', to: 'categories#get_all_categories', via: 'get'
  match '/initialize_with_categories', to: 'categories#initialize_with_categories', via: 'get'
  match '/pebble_get_feed', to: 'categories#pebble_get_feed', via: 'get'
  match '/get_best_feeds', to: 'categories#get_best_feeds', via: 'get'
  match '/like_or_dislike', to: 'categories#like_or_dislike_ios', via: 'get'

  match '/like_pebble', to: 'categories#like_or_dislike_pebble', via: 'get', defaults: { flag: 1}
  match '/dislike_pebble', to: 'categories#like_or_dislike_pebble', via: 'get', defaults: { flag: -1}
  match '/visualization', to: 'categories#visualization', via: 'get'

  # The priority is based upon order of creation: first created -> highest priority.
  # See how all your routes lay out with "rake routes".

  # You can have the root of your site routed with "root"
  # root 'welcome#index'

  # Example of regular route:
  #   get 'products/:id' => 'catalog#view'

  # Example of named route that can be invoked with purchase_url(id: product.id)
  #   get 'products/:id/purchase' => 'catalog#purchase', as: :purchase

  # Example resource route (maps HTTP verbs to controller actions automatically):
  #   resources :products

  # Example resource route with options:
  #   resources :products do
  #     member do
  #       get 'short'
  #       post 'toggle'
  #     end
  #
  #     collection do
  #       get 'sold'
  #     end
  #   end

  # Example resource route with sub-resources:
  #   resources :products do
  #     resources :comments, :sales
  #     resource :seller
  #   end

  # Example resource route with more complex sub-resources:
  #   resources :products do
  #     resources :comments
  #     resources :sales do
  #       get 'recent', on: :collection
  #     end
  #   end

  # Example resource route with concerns:
  #   concern :toggleable do
  #     post 'toggle'
  #   end
  #   resources :posts, concerns: :toggleable
  #   resources :photos, concerns: :toggleable

  # Example resource route within a namespace:
  #   namespace :admin do
  #     # Directs /admin/products/* to Admin::ProductsController
  #     # (app/controllers/admin/products_controller.rb)
  #     resources :products
  #   end
end
