#include <pebble.h>
#include <string.h>

#define KEY_CONTENT 0
/*#define KEY_TITLE 0*/
/*#define KEY_SUMMARY 1*/

#define STEP 3
#define THRESH_HOLD 9
#define PLOT_OFFSET 10

static Window *s_main_window;
static TextLayer *s_word_layer;
static TextLayer *s_status_layer;

static GFont s_word_font;
static GFont s_status_font;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static char * content;
static char * title;
static int start_entry;
static int next_word;

static bool flag = true;
static AppTimer *timer;
static int wpm = 300;
static int factor = 1;
static int factorEnabled = 0;

// plotting
static Layer *path_layer;
static GPath *speed_path;
// This is the layer update callback which is called on render updates
static int ptr = 0;
// range 10 - 70
static int speed[] = {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};
/*static int speed[] = {10, 20, 30, 20, 30, 40, 50, 20, 30, 40, 10, 20, 40, 30, 50, 20, 10, 30, 20, 40, 10, 20, 25, 30, 45, 60};*/
static int plot_enabled = 1;

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
                //              || (content[start_char] == 'º')
                || (content[start_char] == '\''))
        {
            next_word = 1;
            break;
        }
        start_char++;
    }

    if(next_word == 0) {
        timer = app_timer_register(60*1000/wpm, (AppTimerCallback) timer_callback, NULL);
        return;
    }

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
             //          || (content[start_char] == 'º')
             || (content[end_char] == '\''))) {
        end_char++;
    }

    /*1 6*/
    /*2 6*/
    /*3 4*/
    /*4 4*/
    /*5 4*/
    if (end_char - start_char <= 2) {
        buffer = (char*)malloc(sizeof(char)*(1 + end_char - start_char + 7));
        memset(buffer, ' ', 7);
        for (char_index = start_char; char_index < end_char; char_index++) {
            buffer[char_index - start_char + 7] = content[char_index];
        }
        // magic formula
        factor = 0;
        buffer[end_char - start_char + 7] = 0;
    } 
    if (end_char - start_char >= 3 && end_char - start_char <= 4) {
        buffer = (char*)malloc(sizeof(char)*(1 + end_char - start_char + 6));
        memset(buffer, ' ', 6);
        for (char_index = start_char; char_index < end_char; char_index++) {
            buffer[char_index - start_char + 6] = content[char_index];
        }
        // magic formula
        factor = 0;
        buffer[end_char - start_char + 6] = 0;
    } 
    if (end_char - start_char >= 5 && end_char - start_char <= 6) {
        buffer = (char*)malloc(sizeof(char)*(1 + end_char - start_char + 6));
        memset(buffer, ' ', 6);
        for (char_index = start_char; char_index < end_char; char_index++) {
            buffer[char_index - start_char + 6] = content[char_index];
        }
        // magic formula
        factor = 0;
        buffer[end_char - start_char + 6] = 0;
    } 
    if (end_char - start_char >= 7 && end_char - start_char <= 11) {
        buffer = (char*)malloc(sizeof(char)*(1 + end_char - start_char + 3));
        memset(buffer, ' ', 3);
        for (char_index = start_char; char_index < end_char; char_index++) {
            buffer[char_index - start_char + 3] = content[char_index];
        }
        // magic formula
        factor = (THRESH_HOLD - end_char + start_char)*STEP + 0*1 + 1;
        buffer[end_char - start_char + 3] = 0;
    } 

    if (end_char - start_char >= 12) {
        buffer = (char*)malloc(sizeof(char)*(1 + end_char - start_char));
        for (char_index = start_char; char_index < end_char; char_index++) {
            buffer[char_index - start_char] = content[char_index];
        }
        // magic formula
        factor = (THRESH_HOLD - end_char + start_char)*STEP + 0*1 + 1;
        buffer[end_char - start_char] = 0;
    }

    // split content to get clean word everytime it's called

    // strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);

    // Display this time on the TextLayer
    text_layer_set_text(s_word_layer, buffer);
    free(buffer);
    start_entry = end_char + 1;
    // split content to get clean word everytime it's called

    // strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);

    // report read speed
    static char speed_buffer[8];
    wpm = wpm + factor * factorEnabled;
    if (wpm < 100) wpm = 100;
    if (wpm > 2000) wpm = 2000;
    snprintf(speed_buffer, sizeof(speed_buffer), "%d wpm", wpm);
    text_layer_set_text(s_status_layer, speed_buffer);

    timer = app_timer_register(60*1000/wpm, (AppTimerCallback) timer_callback, NULL);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    speed[ptr++ % 25] = wpm / 10;
    /*if(tick_time->tm_sec % 1 == 0) {*/
    /*}*/
}

static void main_window_load(Window *window) {
    //Create GBitmap, then set to created BitmapLayer
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
    s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

    // Create Summary TextLayer
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
    text_layer_set_text_alignment(s_word_layer, GTextAlignmentLeft);
    // Add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_word_layer));

    // Create summary Layer
    s_status_layer = text_layer_create(GRect(0, 120, 144, 25));
    text_layer_set_background_color(s_status_layer, GColorClear);
    text_layer_set_text_color(s_status_layer, GColorWhite);
    text_layer_set_text_alignment(s_status_layer, GTextAlignmentCenter);
    text_layer_set_text(s_status_layer, "Hello World!");
    // Create second custom font, apply it and add to Window
    s_status_font = 
        // fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20));
        fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SANSATION_18));
    text_layer_set_font(s_status_layer, s_status_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_status_layer));
}

static void main_window_unload(Window *window) {
    //Destroy GBitmap
    gbitmap_destroy(s_background_bitmap);

    //Destroy BitmapLayer
    bitmap_layer_destroy(s_background_layer);

    // Destroy TextLayer
    text_layer_destroy(s_word_layer);
    text_layer_destroy(s_status_layer);
    //Unload GFont
    fonts_unload_custom_font(s_word_font);
    fonts_unload_custom_font(s_status_font);
}

static void send_request() {
    // Begin dictionary
    DictionaryIterator *iter = NULL;
    app_message_outbox_begin(&iter);
    if (iter == NULL) {
        APP_LOG(APP_LOG_LEVEL_INFO, "send_request NULL!");
        return;
    } else APP_LOG(APP_LOG_LEVEL_INFO, "send_request OK!");

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
}

/* communication handler */ 

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Enter inbox_received_callback!");
    // Store incoming information
    /*free(content);*/
    content = malloc(1024);
    title = malloc(50);
    APP_LOG(APP_LOG_LEVEL_INFO, "inbox_received_callback! 1");

    // Read first item
    Tuple *t = dict_read_first(iterator);

    // For all items
    while(t != NULL) {
        // Which key was received?
        APP_LOG(APP_LOG_LEVEL_INFO, "inbox_received_callback! 2");
        switch(t->key) {
            case KEY_CONTENT:
                /*snprintf(content, sizeof(content), "%s", t->value->cstring);*/
                /*snprintf(content, sizeof(t->value->cstring), "%s", t->value->cstring);*/
                /*content = (char *) malloc(sizeof(t->value->cstring) + 10);*/
                strcpy(content, t->value->cstring);
                break;
                /*case KEY_TEMPERATURE:*/
                /*snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);*/
                /*break;*/
                /*case KEY_TITLE:*/
                /*snprintf(title, sizeof(title), "%s", t->value->cstring);*/
                /*break;*/
                /*case KEY_SUMMARY:*/
                /*snprintf(content, sizeof(content), "%s", t->value->cstring);*/
                /*break;*/
            default:
                APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
                break;
        }

        // Look for next item
        t = dict_read_next(iterator);
    }

    APP_LOG(APP_LOG_LEVEL_INFO, "inbox_received_callback! 3");
    // ready
    APP_LOG(APP_LOG_LEVEL_INFO, content);
    start_entry = 0;
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

/* clicks handler */

static void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_INFO, "up_single_click_handler success!");
}

static void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_INFO, "down_single_click_handler success!");
}
// pass
static void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    /*send_request();*/
    // Begin dictionary
    DictionaryIterator *iter = NULL;
    app_message_outbox_begin(&iter);
    if (iter == NULL) {
        APP_LOG(APP_LOG_LEVEL_INFO, "send_request NULL!");
        return;
    } else APP_LOG(APP_LOG_LEVEL_INFO, "send_request OK!");

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
    APP_LOG(APP_LOG_LEVEL_INFO, "select_single_handler success!");
}

static void up_multi_click_handler(ClickRecognizerRef recognizer, void *context) {
    wpm += 50;
    if (wpm > 2000) wpm = 2000;
    static char speed_buffer[8];
    snprintf(speed_buffer, sizeof(speed_buffer), "%d wpm", wpm);
    text_layer_set_text(s_status_layer, speed_buffer);
    APP_LOG(APP_LOG_LEVEL_INFO, "up_multi_click_handler success!");
}

static void down_multi_click_handler(ClickRecognizerRef recognizer, void *context) {
    wpm -= 50;
    if (wpm < 100) wpm = 100;
    static char speed_buffer[8];
    snprintf(speed_buffer, sizeof(speed_buffer), "%d wpm", wpm);
    text_layer_set_text(s_status_layer, speed_buffer);
    APP_LOG(APP_LOG_LEVEL_INFO, "down_multi_click_handler success!");
}

static void select_multi_click_handler(ClickRecognizerRef recognizer, void *context) {
    factorEnabled = 1 - factorEnabled;
    APP_LOG(APP_LOG_LEVEL_INFO, "select_multi_click_handler success!");
}

// auto adjust speed
static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_INFO, "select_long_click_handler success!");
}

static void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
    start_entry = 0;
    APP_LOG(APP_LOG_LEVEL_INFO, "select_long_click_release_handler success!");
}

static void config_provider(Window *window) {
    // single click / repeat-on-hold config:
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);

    // multi click config:
    /*window_multi_click_subscribe(BUTTON_ID_UP, 2, 10, 400, false, up_multi_click_handler);*/
    /*window_multi_click_subscribe(BUTTON_ID_DOWN, 2, 10, 400, false, down_multi_click_handler);*/
    window_multi_click_subscribe(BUTTON_ID_UP, 0, 0, 0, false, up_multi_click_handler);
    window_multi_click_subscribe(BUTTON_ID_DOWN, 0, 0, 0, false, down_multi_click_handler);
    window_multi_click_subscribe(BUTTON_ID_SELECT, 0, 0, 0, false, select_multi_click_handler);

    // long click config:
    window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);

}

// plot path part
static void path_layer_update_callback(Layer *me, GContext *ctx) {
    (void)me;
    graphics_context_set_stroke_color(ctx, GColorWhite);
    /*gpath_draw_outline(ctx, speed_path);*/
    if (plot_enabled){
        int j;
        for (j = 0; j<=24; ++j){
            int i = (j+ptr+12) % 25;
            if(i != 24)
                graphics_draw_line(ctx, GPoint(i*5+PLOT_OFFSET, 60 - speed[i]), GPoint((i+1)*5+PLOT_OFFSET, 60 - speed[i+1]));
        }
    }
}

// This is an example of a path that looks like a compound path
// If you rotate it however, you will see it is a single shape
/*static const GPathInfo SPEED_PATH_POINTS = {*/
/*25,*/
/*(GPoint []) {*/
/*{5, 10},    {10, 10},*/
/*{15, 10},    {20, 10},*/
/*{25, 10},    {30, 20},*/
/*{35, 20},    {40, 20},*/
/*{40, 20},    {45, 20},*/
/*{50, 20},    {55, 30},*/
/*{60, 30},    {65, 30},*/
/*{70, 30},    {75, 30},*/
/*{80, 40},    {85, 40},*/
/*{90, 40},    {95, 40},*/
/*{100, 40},   {105, 50},*/
/*{110, 50},   {115, 50},*/
/*{120, 50},   {125, 50}*/
/*}*/
/*};*/

// accelerometer, shake
static void tap_handler(AccelAxisType axis, int32_t direction) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Shake!");
    plot_enabled = 1 - plot_enabled;
}


static void init() {
    // Spribble init
    title = "Hello World!";
    content = "Hi, this is team PebbleReader at HackShanghai! By Guy Faulconbridge and Alistair Smout PERTH Scotland (Reuters) - If Scottish nationalists win a 'kingmaker' position in Britain's May 2015 election, they would consider supporting a minority Labour government but would never get into bed with the Conservatives, their leader in the London parliament said. Since Scots voted by 55-45 percent to preserve the United Kingdom in a Sept. 18 referendum, support for the Scottish National Party has surged on a perception that Britain's rulers are backsliding on pledges to grant more powers. ...";
    // APP_LOG(APP_LOG_LEVEL_INFO, content);

    start_entry = 0;

    // Create main Window element and assign to pointer
    s_main_window = window_create();

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers) {
            .load = main_window_load,
            .unload = main_window_unload
            });

    // Set click config provider (as in example)
    window_set_click_config_provider(s_main_window, (ClickConfigProvider) config_provider);

    // Show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);

    // Register with TickTimerService
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    timer = app_timer_register(60*1000/wpm, (AppTimerCallback) timer_callback, NULL);

    // Plot speed graph
    Layer *window_layer = window_get_root_layer(s_main_window);
    GRect bounds = layer_get_frame(window_layer);
    path_layer = layer_create(bounds);
    layer_add_child(window_layer, path_layer);
    /*speed_path = gpath_create(&SPEED_PATH_POINTS);*/
    /*gpath_move_to(speed_path, GPoint(bounds.size.w/2, bounds.size.h/2));*/
    /*gpath_move_to(speed_path, GPoint(0, 0));*/
    layer_set_update_proc(path_layer, path_layer_update_callback);

    // Register with Accelerometers
    accel_tap_service_subscribe(tap_handler);

    /*// Register callbacks*/
    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);
    app_message_register_outbox_sent(outbox_sent_callback);

    /*// Open AppMessage*/
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
    gpath_destroy(speed_path);
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
