#include <pebble.h>
#include <string.h>
  
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1

static Window *s_main_window;
static TextLayer *s_word_layer;
static TextLayer *s_status_layer;

static GFont s_word_font;
static GFont s_status_font;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static char * content;
static int start_entry;
static int next_word;

static bool flag = true;
AppTimer *timer;
static int wpm = 300;

static void timer_callback() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char *buffer;

    int start_char = start_entry, end_char;
    int len = strlen(content);
    int char_index;
    next_word = 0;
    while (start_char < len) {
        if ((content[start_char] <= '9' && content[start_char] >= '0')
            || (content[start_char] <= 'z' && content[start_char] >= 'a')
            || (content[start_char] <= 'Z' && content[start_char] >= 'A')
            || (content[start_char] == '@')
            || (content[start_char] == '#')
            || (content[start_char] == '$')
            || (content[start_char] == '%')
            || (content[start_char] == '&')
            || (content[start_char] == '\''))
        {
            next_word = 1;
            break;
        }
        start_char++;
    }
    
    if(next_word == 0)
        return;
    
    end_char = start_char;
    while (end_char < len &&
           ((content[end_char] <= '9' && content[end_char] >= '0')
            || (content[end_char] <= 'z' && content[end_char] >= 'a')
            || (content[end_char] <= 'Z' && content[end_char] >= 'A')
            || (content[end_char] == '@')
            || (content[end_char] == '#')
            || (content[end_char] == '$')
            || (content[end_char] == '%')
            || (content[end_char] == '&')
            || (content[end_char] == '\''))) {
               end_char++;
           }
    
    buffer = (char*)malloc(sizeof(char)*(1 + end_char - start_char));
    for (char_index = start_char; char_index < end_char; char_index++) {
        buffer[char_index - start_char] = content[char_index];
    }
    buffer[end_char - start_char] = 0;

  // split content to get clean word everytime it's called

  // strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_word_layer, buffer);
    free(buffer);
    start_entry = end_char + 1;
    // split content to get clean word everytime it's called

    // strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);

    timer = app_timer_register(60*1000/wpm, (AppTimerCallback) timer_callback, NULL);
}

static void main_window_load(Window *window) {
    //Create GBitmap, then set to created BitmapLayer
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
    s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

    // Create time TextLayer
    s_word_layer = text_layer_create(GRect(3, 72, 139, 50));
    text_layer_set_background_color(s_word_layer, GColorClear);
    text_layer_set_text_color(s_word_layer, GColorWhite);
    text_layer_set_text(s_word_layer, "Spribblealism");

    //Create GFont
    s_word_font = 
       // fonts_get_system_font("RESOURCE_ID_ROBOTO_CONDENSED_21")
       // fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20));
       fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SANSATION_20));

    //Apply to TextLayer
    text_layer_set_font(s_word_layer, s_word_font);
    text_layer_set_text_alignment(s_word_layer, GTextAlignmentCenter);

    // Add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_word_layer));

    // Create temperature Layer
    /*s_status_layer = text_layer_create(GRect(0, 130, 144, 25));*/
    /*text_layer_set_background_color(s_status_layer, GColorClear);*/
    /*text_layer_set_text_color(s_status_layer, GColorWhite);*/
    /*text_layer_set_text_alignment(s_status_layer, GTextAlignmentCenter);*/
    /*text_layer_set_text(s_status_layer, "Loading...");*/

    /*// Create second custom font, apply it and add to Window*/
    /*s_status_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20));*/
    /*text_layer_set_font(s_status_layer, s_status_font);*/
    /*layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_status_layer));*/

    // Make sure the time is displayed from the start
    // update_time();
}

static void main_window_unload(Window *window) {
    //Unload GFont
    fonts_unload_custom_font(s_word_font);

    //Destroy GBitmap
    gbitmap_destroy(s_background_bitmap);

    //Destroy BitmapLayer
    bitmap_layer_destroy(s_background_layer);

    // Destroy TextLayer
    text_layer_destroy(s_word_layer);

    // Destroy weather elements
    text_layer_destroy(s_status_layer);
    fonts_unload_custom_font(s_status_font);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    //update_time();

    // Get weather update every 30 minutes
    if(tick_time->tm_min % 30 == 0) {
        // Begin dictionary
        DictionaryIterator *iter;
        app_message_outbox_begin(&iter);

        // Add a key-value pair
        dict_write_uint8(iter, 0, 0);

        // Send the message!
        app_message_outbox_send();
    }
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    // Store incoming information
    static char temperature_buffer[8];
    static char conditions_buffer[32];
    static char weather_layer_buffer[32];

    // Read first item
    Tuple *t = dict_read_first(iterator);

    // For all items
    while(t != NULL) {
        // Which key was received?
        switch(t->key) {
            case KEY_TEMPERATURE:
                snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
                break;
            case KEY_CONDITIONS:
                snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
                break;
            default:
                APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
                break;
        }

        // Look for next item
        t = dict_read_next(iterator);
    }

    // Assemble full string and display
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
    text_layer_set_text(s_status_layer, weather_layer_buffer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() {
  // Spribble init
  content = "Hi, this is team PebbleReader at HackShanghai! Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris eu consectetur eros. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Suspendisse vulputate iaculis metus ut lacinia. Nunc dapibus elit in turpis euismod volutpat. Aenean magna neque, fringilla id dui eget, imperdiet volutpat justo. Aliquam ut diam malesuada nunc consectetur vulputate eget ut augue. Nullam lacinia vestibulum lacinia. Pellentesque nisl eros, elementum nec nibh a, efficitur elementum orci. Morbi dolor nisi, mattis sit amet velit eu, maximus tempus odio. Curabitur placerat pulvinar nisi. Nulla auctor tempor viverra. Vestibulum nec urna interdum, imperdiet quam vel, viverra enim. Aenean vitae metus nibh.";
    
  start_entry = 0;

  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  /*tick_timer_service_subscribe(SECOND_UNIT, tick_handler);*/
  timer = app_timer_register(60*1000/wpm, (AppTimerCallback) timer_callback, NULL);
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
