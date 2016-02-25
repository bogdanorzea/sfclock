/*
// Notes
 Pebble Time screen resolution: 144x168
 APP_LOG(APP_LOG_LEVEL_DEBUG, "Returned week day: %s", weekday); // How to log
 time_font = fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS); // How to get system fonts


// To install for config website
  sudo apt-get install node.js
  sudo ln -s /usr/bin/nodejs /usr/bin/node
  sudo apt-get install npm
  sudo npm install -g bower
  sudo npm install -g http-server
  cd ./%WATCHFACE_FOLDER%/
  get ngrok from https://ngrok.com/ and extract to %WATCHFACE_FOLDER%
  ./ngrok authtoken <ID>
  [bower install pebble-slate]
  http-server
  ./ngrok http 8080

// To install the SDK go to : https://developer.getpebble.com/sdk/install/linux/

// Shell config
  echo 'export PATH=~/pebble-dev/pebble-sdk-4.1.1-linux64/bin:$PATH' >> ~/.bash_profile
  . ~/.bash_profile

// To run build watchface
  pebble build
  pebble install --emulator basalt /or/ pebble install --phone 192.168.1.102
  pebble logs --emulator basalt --color
  pebble emu-app-config --emulator basalt
  pebble emu-battery --emulator basalt --percent 100 --charging
  pebble emu-battery --emulator basalt --percent 100
  pebble emu-bt-connection --emulator basalt --connected yes / no
  pebble emu-bt-connection --emulator basalt --connected no

*/

// Included headers
#include <pebble.h>
#include <pebble_fonts.h>
#include <time.h>

// DEFINE KEYS
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
#define KEY_WEATHER_VISIBLE 2
#define KEY_BACKGROUND_COLOR 3
#define KEY_FONT_COLOR 4
#define KEY_BACKGROUND_DISCONNECT_COLOR 5
#define KEY_FONT_DISCONNECT_COLOR 6
#define KEY_MILITARY_TIME 7
#define KEY_BATTERY_VISIBLE 8
#define KEY_BATTERY_TYPE 9
#define KEY_VIBRATE_HOURLY 10
#define KEY_VIBRATE_START_TIME 11
#define KEY_VIBRATE_END_TIME 12
#define KEY_VIBRATE_START_DAY 13
#define KEY_VIBRATE_END_DAY 14
#define KEY_VIBRATE_HOURLY_STYLE 15
#define KEY_VIBRATE_DISCONNECT 16
#define KEY_VIBRATE_DISCONNECT_STYLE 17
#define KEY_CUSTOM_TEXT_VISIBLE 18
#define KEY_CUSTOM_TEXT_VALUE 19
#define KEY_VISUAL_DISCONNECT 20
#define KEY_WEATHER_TEXT 21   

// Global variables declaration
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_day_layer;
static TextLayer *s_weather_layer;
static TextLayer *s_custom_layer;
static TextLayer *s_battery_percent_layer;
//static Layer *s_battery_layer;
static GFont time_font;
static GFont text_font;
static GFont text_small_font;
static int background_color = 0xFFFFFF;
static int font_color = 0x000000;
static bool visual_disconnect = true;
static int background_disconnect_color = 0xFF0000;
static int font_disconnect_color = 0x000000;
static bool military_time = 1;
static bool weather_visible = true;
static char weather_text[32];
static bool custom_text_visible = true;
static char custom_text_value[32];
static bool battery_visible = true;
static bool battery_type = 1;
static bool vibrate_hourly = true;
static bool vibrate_disconnect = true;
static int vibrate_start_time = 9;
static int vibrate_end_time = 18;
static int vibrate_start_day = 1;
static int vibrate_end_day = 5;
static int vibrate_hourly_style = 0;
static int vibrate_disconnect_style = 0;

// Vibrates the watch
static void emit_vibration(int vibrate){
  switch(vibrate) {
    case 0: vibes_short_pulse(); break;
    case 1: vibes_long_pulse(); break;
    case 2: vibes_double_pulse(); break;
  }
}
// Set colors for background
static void set_background_color(int color) {
  GColor b_color = GColorFromHEX(color);
  window_set_background_color(s_main_window, b_color);
}
// Set colors for font
static void set_text_color(int color) {
  GColor f_color = GColorFromHEX(color);
  text_layer_set_text_color(s_time_layer, f_color);
  text_layer_set_text_color(s_day_layer, f_color);
  text_layer_set_text_color(s_weather_layer, f_color);
  text_layer_set_text_color(s_custom_layer, f_color);
  text_layer_set_text_color(s_battery_percent_layer, f_color);
}
// Update time
static void update_time() {
  // Gets a time structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  // Sets the time
  static char time[6];
  if(clock_is_24h_style() == military_time) {
    // Use 24 hour format
    strftime(time, sizeof(time), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(time, sizeof(time), "%I:%M", tick_time);
  }
  text_layer_set_text(s_time_layer, time);

  
  /*
   * Conditions below try to solve the cases when start is greater than end for hourly vibration intervals (both hour and days)
   */

  // Checks if time is on the hour and vibrate_hourly is active
  if (vibrate_hourly && tick_time->tm_sec == 0 && tick_time->tm_min == 0) {
    // Triggers if vibrate_start_day < vibrate_end_day
    if (vibrate_start_day <= vibrate_end_day && tick_time->tm_wday >= vibrate_start_day && tick_time->tm_wday <= vibrate_end_day) {
      // Triggers if vibrate_start_time < vibrate_end_time
      if (vibrate_start_time <= vibrate_end_time && tick_time->tm_hour >= vibrate_start_time && tick_time->tm_hour <= vibrate_end_time) {
        emit_vibration(vibrate_hourly_style);
      }
      // Triggers if vibrate_start_time > vibrate_end_time
      else if (vibrate_start_time > vibrate_end_time && (tick_time->tm_hour >= vibrate_start_time || tick_time->tm_hour <= vibrate_end_time)) {
        emit_vibration(vibrate_hourly_style);
      }
    }
    // Triggers if vibrate_start_day > vibrate_end_day
    else if (vibrate_start_day > vibrate_end_day && (tick_time->tm_wday >= vibrate_start_day || tick_time->tm_wday <= vibrate_end_day)) {
      // Triggers if vibrate_start_time < vibrate_end_time
      if (vibrate_start_time <= vibrate_end_time && tick_time->tm_hour >= vibrate_start_time && tick_time->tm_hour <= vibrate_end_time) {
        emit_vibration(vibrate_hourly_style);
      }
      // Triggers if vibrate_start_time > vibrate_end_time
      else if (vibrate_start_time > vibrate_end_time && (tick_time->tm_hour >= vibrate_start_time || tick_time->tm_hour <= vibrate_end_time)) {
        emit_vibration(vibrate_hourly_style);
      }
    }
  }
  
  //Sets the weekday
  static char weekday[11];
  strftime(weekday, sizeof(weekday), "%a.%d.%b", tick_time);
  text_layer_set_text(s_day_layer, weekday);
  
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

  //free(tick_time);
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
// Update Bluetooth
static void bt_handler(bool connected) {
  if (connected) {
      set_background_color(background_color);
      set_text_color(font_color);  
  } else {
    // Vibrates on disconnect
    if (vibrate_disconnect) {
      emit_vibration(vibrate_disconnect_style);
    }
    // Visual alert on disconnect
    if (visual_disconnect) {
      set_background_color(background_disconnect_color);
      set_text_color(font_disconnect_color);
    }
  }
}
// Update battery
static void battery_handler(BatteryChargeState state) {
  // Update s_battery_percent_layer with battery info
  text_layer_set_text(s_battery_percent_layer, " ");
  if (battery_visible) {
      static char batt[12];
      if (state.is_charging) {
        snprintf(batt, sizeof(batt), "Charging...");
      } else {
        snprintf(batt, sizeof(batt), "Batt: %i%%", (int)state.charge_percent);
      }
    text_layer_set_text(s_battery_percent_layer, batt);
  }

// DISABLED PROGRESSBAR
// 
// static void battery_update_proc(Layer *layer, GContext *ctx) {
//   GRect bounds = layer_get_bounds(layer);
//   GColor8 battery_color;

//   // Find the width of the bar
//   int width = (int)(float)(((float)s_battery_level / 100.0F) * 114.0F);

//   // Set the gradient for battery meter
//   if (s_battery_level <= 20) {battery_color = GColorRed;} 
//   else if (s_battery_level <= 40) {battery_color = GColorChromeYellow;}
//   else if (s_battery_level <= 70) {battery_color = GColorKellyGreen;}
//   else {battery_color = GColorDarkGreen;}

//   // Draw the battery value
//   graphics_context_set_fill_color(ctx, battery_color);
//   graphics_fill_rect(ctx, GRect(0, 0, width, bounds.size.h), 1, GCornersAll);

//   // Draw the battery background
//   graphics_context_set_stroke_color(ctx, GColorFromHEX(font_color));
//   graphics_draw_round_rect(ctx, bounds, 1);

}
// Load main window
static void main_window_load(Window *window) {
  GColor font_background_color = GColorClear;  
  
  //// COMPLETE HERE WITH VALUE THAT ARE READ FROM THE PERSISTENT MEMORY
  if (persist_exists(KEY_MILITARY_TIME)) {
    military_time = persist_read_bool(KEY_MILITARY_TIME);
  }
  if (persist_exists(KEY_FONT_COLOR)) {
    font_color = persist_read_int(KEY_FONT_COLOR);
  }
  if (persist_exists(KEY_BACKGROUND_DISCONNECT_COLOR)) {
    background_disconnect_color = persist_read_int(KEY_BACKGROUND_DISCONNECT_COLOR);
  }
  if (persist_exists(KEY_FONT_DISCONNECT_COLOR)) {
    font_disconnect_color = persist_read_int(KEY_FONT_DISCONNECT_COLOR);
  }
  if (persist_exists(KEY_BATTERY_VISIBLE)) {
    battery_visible = persist_read_bool(KEY_BATTERY_VISIBLE);
  }
  if (persist_exists(KEY_BATTERY_TYPE)) {
    battery_type = persist_read_bool(KEY_BATTERY_TYPE);
  }
  if (persist_exists(KEY_WEATHER_VISIBLE)) {
    weather_visible = persist_read_bool(KEY_WEATHER_VISIBLE);
  }
  if (persist_exists(KEY_VIBRATE_HOURLY)) {
    vibrate_hourly = persist_read_bool(KEY_VIBRATE_HOURLY);
  }
  if (persist_exists(KEY_VIBRATE_DISCONNECT)) {
    vibrate_disconnect = persist_read_bool(KEY_VIBRATE_DISCONNECT);
  }
  if (persist_exists(KEY_VISUAL_DISCONNECT)) {
    visual_disconnect = persist_read_bool(KEY_VISUAL_DISCONNECT);
  }
  if (persist_exists(KEY_CUSTOM_TEXT_VISIBLE)) {
    custom_text_visible = persist_read_bool(KEY_CUSTOM_TEXT_VISIBLE);
  }
  if (persist_exists(KEY_CUSTOM_TEXT_VALUE)) {
    persist_read_string(KEY_CUSTOM_TEXT_VALUE, custom_text_value, sizeof(custom_text_value));
  } else {
    snprintf(custom_text_value, sizeof(custom_text_value), "%s", "This is CS50!");
  }
  if (persist_exists(KEY_VIBRATE_START_TIME)) {
    vibrate_start_time = persist_read_int(KEY_VIBRATE_START_TIME);
  }
  if (persist_exists(KEY_VIBRATE_END_TIME)) {
    vibrate_end_time = persist_read_int(KEY_VIBRATE_END_TIME);
  }
  if (persist_exists(KEY_VIBRATE_START_DAY)) {
    vibrate_start_day = persist_read_int(KEY_VIBRATE_START_DAY);
  }
  if (persist_exists(KEY_VIBRATE_END_DAY)) {
    vibrate_end_day = persist_read_int(KEY_VIBRATE_END_DAY);
  }
  if (persist_exists(KEY_VIBRATE_HOURLY_STYLE)) {
    vibrate_hourly_style = persist_read_int(KEY_VIBRATE_HOURLY_STYLE);
  }
  if (persist_exists(KEY_VIBRATE_DISCONNECT_STYLE)) {
    vibrate_disconnect_style = persist_read_int(KEY_VIBRATE_DISCONNECT_STYLE);
  }
  if (persist_exists(KEY_WEATHER_TEXT)) {
    persist_read_string(KEY_WEATHER_TEXT, weather_text, sizeof(weather_text));
  } else {
    snprintf(weather_text, sizeof(weather_text), "%s", "Loading...");
  }
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Military time is %d", military_time);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Font color is 0x%06x", font_color);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Background disconnect color is 0x%06x", background_disconnect_color);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Font disconnect color is 0x%06x", font_color);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Battery visibility is %i", battery_visible);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Battery type is %i", battery_type);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Weather visibility is %i", weather_visible);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Vibrate hourly is %i", vibrate_hourly);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Vibrate disconnect is %i", vibrate_disconnect);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Visual disconnect is %i", visual_disconnect);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Custom text visibility is %i", custom_text_visible);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Custom text value is '%s'", custom_text_value);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Vibrate start time is %i", vibrate_start_time);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Vibrate end time is %i", vibrate_end_time);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Vibrate start day is %i", vibrate_start_day);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Vibrate end day is %i", vibrate_end_day);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Vibrate hourly style %i", vibrate_hourly_style);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Vibrate disconnect style %i", vibrate_disconnect_style);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Custom text value is '%s'", custom_text_value);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Weather text value is '%s'", weather_text);

  //Setting the UX
  time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_OTHER_FONT_45));
  text_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_OTHER_FONT_25));
  text_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_OTHER_FONT_20));
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(3, -4, 142, 50));
  text_layer_set_background_color(s_time_layer, font_background_color);
  text_layer_set_text_color(s_time_layer, GColorFromHEX(font_color));
  text_layer_set_font(s_time_layer, time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // Create weekday TextLayer
  s_day_layer = text_layer_create(GRect(0, 49, 142, 30));
  text_layer_set_background_color(s_day_layer, font_background_color);
  text_layer_set_text_color(s_day_layer, GColorFromHEX(font_color));
  text_layer_set_font(s_day_layer, text_font);
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));
  
  // Create weather TextLayer
  s_weather_layer = text_layer_create(GRect(0, 80, 142, 30));
  text_layer_set_background_color(s_weather_layer, font_background_color);
  text_layer_set_text_color(s_weather_layer, GColorFromHEX(font_color));
  text_layer_set_font(s_weather_layer, text_small_font);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentRight);
  text_layer_set_text(s_weather_layer, " ");
  if (weather_visible) {
    text_layer_set_text(s_weather_layer, weather_text);
  }
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  
  // Create customText TextLayer
  s_custom_layer = text_layer_create(GRect(0, 106, 142, 29));
  text_layer_set_background_color(s_custom_layer, font_background_color);
  text_layer_set_text_color(s_custom_layer, GColorFromHEX(font_color));
  text_layer_set_font(s_custom_layer, text_small_font);
  text_layer_set_text_alignment(s_custom_layer, GTextAlignmentRight);
  text_layer_set_overflow_mode(s_custom_layer, GTextOverflowModeWordWrap);
  text_layer_set_text(s_custom_layer, " ");
  if (custom_text_visible) {
    text_layer_set_text(s_custom_layer, custom_text_value);
  }
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_custom_layer));
  
  // Create battery procent Layer
  s_battery_percent_layer = text_layer_create(GRect(0, 132, 142, 29));
  text_layer_set_background_color(s_battery_percent_layer, font_background_color);
  text_layer_set_text_color(s_battery_percent_layer, GColorFromHEX(font_color));
  text_layer_set_text_color(s_battery_percent_layer, GColorFromHEX(font_color));
  text_layer_set_font(s_battery_percent_layer, text_small_font);
  text_layer_set_text_alignment(s_battery_percent_layer, GTextAlignmentRight);
  text_layer_set_text(s_battery_percent_layer, " ");
  if (battery_visible) {
    text_layer_set_text(s_battery_percent_layer, "Batt: ?");
  }
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_percent_layer));
  text_layer_set_text_color(s_battery_percent_layer, GColorFromHEX(font_color));

  // DISABLED PROGRESSBAR
  // s_battery_layer = layer_create(GRect(6, 155, 132, 7));
  // layer_set_update_proc(s_battery_layer, battery_update_proc);
  // layer_add_child(window_get_root_layer(window), s_battery_layer);

  // if (battery_visible) {
  //   if (battery_percent) {
  //     text_layer_set_text(s_battery_percent_layer, "Batt: ?");
  //     layer_set_hidden(s_battery_layer, true);
  //   } else {
  //     text_layer_set_text(s_battery_percent_layer, " ");
  //     layer_set_hidden(s_battery_layer, false);
  //     layer_mark_dirty(s_battery_layer);
  //   }
  // } else {
  //   text_layer_set_text(s_battery_percent_layer, " ");
  //   layer_set_hidden(s_battery_layer, true);
  // }

  //Call updates
  update_time();
  bt_handler(bluetooth_connection_service_peek());
  battery_handler(battery_state_service_peek());
}
// Unload main window
static void main_window_unload(Window *window) {
  // Destroy Layers
  fonts_unload_custom_font(time_font);
  fonts_unload_custom_font(text_font);
  fonts_unload_custom_font(text_small_font);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_weather_layer);
  text_layer_destroy(s_custom_layer);
  text_layer_destroy(s_battery_percent_layer);
  //layer_destroy(s_battery_layer);
}
// Communicate with the phone using AppMessage
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read weather data
  static char temperature_buffer[8];
  static char conditions_buffer[32];

  Tuple *temp_tuple = dict_find(iterator, KEY_TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, KEY_CONDITIONS);

  if(temp_tuple && conditions_tuple) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°C", (int)temp_tuple->value->int32);
    snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);
    snprintf(weather_text, sizeof(weather_text), "%s, %s", temperature_buffer, conditions_buffer);
    persist_write_string(KEY_WEATHER_TEXT, weather_text);
    text_layer_set_text(s_weather_layer, " ");
    if (weather_visible) {
      text_layer_set_text(s_weather_layer, weather_text);
    }
  }
  
  // Read configuration data
  Tuple *temp_military_time = dict_find(iterator, KEY_MILITARY_TIME);
  Tuple *temp_background_color = dict_find(iterator, KEY_BACKGROUND_COLOR);
  Tuple *temp_font_color = dict_find(iterator, KEY_FONT_COLOR);
  Tuple *temp_background_disconnect_color = dict_find(iterator, KEY_BACKGROUND_DISCONNECT_COLOR);
  Tuple *temp_font_disconnect_color = dict_find(iterator, KEY_FONT_DISCONNECT_COLOR);
  Tuple *temp_weather_visible = dict_find(iterator, KEY_WEATHER_VISIBLE);
  Tuple *temp_battery_visible = dict_find(iterator, KEY_BATTERY_VISIBLE);
  Tuple *temp_battery_type = dict_find(iterator, KEY_BATTERY_TYPE);
  Tuple *temp_vibrate_hourly = dict_find(iterator, KEY_VIBRATE_HOURLY);
  Tuple *temp_vibrate_hourly_style = dict_find(iterator, KEY_VIBRATE_HOURLY_STYLE);
  Tuple *temp_vibrate_start_time= dict_find(iterator, KEY_VIBRATE_START_TIME);
  Tuple *temp_vibrate_end_time= dict_find(iterator, KEY_VIBRATE_END_TIME);
  Tuple *temp_vibrate_start_day= dict_find(iterator, KEY_VIBRATE_START_DAY);
  Tuple *temp_vibrate_end_day= dict_find(iterator, KEY_VIBRATE_END_DAY);
  Tuple *temp_vibrate_disconnect = dict_find(iterator, KEY_VIBRATE_DISCONNECT);
  Tuple *temp_vibrate_disconnect_style = dict_find(iterator, KEY_VIBRATE_DISCONNECT_STYLE);
  Tuple *temp_visual_disconnect = dict_find(iterator, KEY_VISUAL_DISCONNECT);
  Tuple *temp_custom_text_visible = dict_find(iterator, KEY_CUSTOM_TEXT_VISIBLE);
  Tuple *temp_custom_text_value = dict_find(iterator, KEY_CUSTOM_TEXT_VALUE);

  if (temp_military_time) {
    military_time = temp_military_time->value->int8;
    persist_write_bool(KEY_MILITARY_TIME, military_time);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Time is %i (0 for 24-H format, 1 for 12-H format)", military_time);
    update_time();
  }
  if (temp_background_color) {
    int b_color = temp_background_color->value->int32;
    persist_write_int(KEY_BACKGROUND_COLOR, b_color);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Background color is 0x%06x", b_color);
    set_background_color(b_color);
  }
  if (temp_font_color) {
    int f_color = temp_font_color->value->int32;
    persist_write_int(KEY_FONT_COLOR, f_color);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Font color is 0x%06x", f_color);
    set_text_color(f_color);
  }
  if (temp_background_disconnect_color) {
    int bd_color = temp_background_disconnect_color->value->int32;
    persist_write_int(KEY_BACKGROUND_DISCONNECT_COLOR, bd_color);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Background disconnect color is 0x%06x", bd_color);
    background_disconnect_color = bd_color;
  }
  if (temp_font_disconnect_color) {
    int fd_color = temp_font_disconnect_color->value->int32;
    persist_write_int(KEY_FONT_DISCONNECT_COLOR, fd_color);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Font disconnect color is 0x%06x", fd_color);
    font_disconnect_color = fd_color;
  }
  if (temp_weather_visible) {
    weather_visible = temp_weather_visible->value->int8;
    persist_write_bool(KEY_WEATHER_VISIBLE, weather_visible);
    text_layer_set_text(s_weather_layer, " ");
    if (weather_visible) {
      text_layer_set_text(s_weather_layer, weather_text);
    }
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather visibility is %i", weather_visible);
  }
  if (temp_battery_visible) {
    battery_visible = temp_battery_visible->value->int8;
    persist_write_bool(KEY_BATTERY_VISIBLE, battery_visible);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Battery visibility is %i", battery_visible);
  }
  if (temp_battery_type) {
    battery_type = temp_battery_type->value->int8;
    persist_write_bool(KEY_BATTERY_TYPE, battery_type);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Battery type is %i (0 for progressbar, 1 for percentage)", battery_type);
  }
  battery_handler(battery_state_service_peek());
  if (temp_vibrate_hourly) {
    vibrate_hourly = temp_vibrate_hourly->value->int8;
    persist_write_bool(KEY_VIBRATE_HOURLY, vibrate_hourly);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibrate hourly is %i", vibrate_hourly);    
  }
  if (temp_vibrate_hourly_style) {
    vibrate_hourly_style = temp_vibrate_hourly_style->value->int32;
    persist_write_int(KEY_VIBRATE_HOURLY_STYLE, vibrate_hourly_style);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibrate hourly style is %d", vibrate_hourly_style);
  }
  if (temp_vibrate_disconnect_style) {
    vibrate_disconnect_style = temp_vibrate_disconnect_style->value->int32;
    persist_write_int(KEY_VIBRATE_DISCONNECT_STYLE, vibrate_disconnect_style);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibrate disconnect style is %d", vibrate_disconnect_style);
  }
  if (temp_vibrate_start_time) {
    vibrate_start_time = temp_vibrate_start_time->value->int32;
    persist_write_int(KEY_VIBRATE_START_TIME, vibrate_start_time);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibrate start time is %02d:00", vibrate_start_time);
  }
  if (temp_vibrate_end_time) {
    vibrate_end_time = temp_vibrate_end_time->value->int32;
    persist_write_int(KEY_VIBRATE_END_TIME, vibrate_end_time);
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibrate end time is %02d:00", vibrate_end_time);
  }
  if (temp_vibrate_start_day) {
    vibrate_start_day = temp_vibrate_start_day->value->int32;
    persist_write_int(KEY_VIBRATE_START_DAY, vibrate_start_day);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibrate start day is %d", vibrate_start_day);
  }
  if (temp_vibrate_end_day) {
    vibrate_end_day = temp_vibrate_end_day->value->int32;
    persist_write_int(KEY_VIBRATE_END_DAY, vibrate_end_day);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibrate end day is %d", vibrate_end_day);
  }
  if (temp_vibrate_disconnect) {
    vibrate_disconnect = temp_vibrate_disconnect->value->int8;
    persist_write_bool(KEY_VIBRATE_DISCONNECT, vibrate_disconnect);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibrate disconnect is %i", vibrate_disconnect);    
  }
  if (temp_visual_disconnect) {
    visual_disconnect = temp_visual_disconnect->value->int8;
    persist_write_bool(KEY_VISUAL_DISCONNECT, visual_disconnect);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Visual disconnect is %i", visual_disconnect);    
  }
  if (temp_custom_text_visible) {
    custom_text_visible = temp_custom_text_visible->value->int8;
    persist_write_bool(KEY_CUSTOM_TEXT_VISIBLE, custom_text_visible);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Custom text visibility is %i", custom_text_visible);
    if (!custom_text_visible) {
      text_layer_set_text(s_custom_layer, " ");
    }
  }
  if(temp_custom_text_value) {
    snprintf(custom_text_value, sizeof(custom_text_value), "%s", temp_custom_text_value->value->cstring);
    persist_write_string(KEY_CUSTOM_TEXT_VALUE, custom_text_value);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Custom text value is \"%s\"", custom_text_value);
    if (custom_text_visible) {
      text_layer_set_text(s_custom_layer, custom_text_value);
    }
  }
}
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  // APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  // APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}
static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  // APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}
// Initialize the main window
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  if (persist_exists(KEY_BACKGROUND_COLOR)) {
    background_color = persist_read_int(KEY_BACKGROUND_COLOR);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistentStorage: Background color is 0x%06x", background_color);
  }
  set_background_color(background_color);
  
  // Show the Window on the watch, with animation
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  window_stack_push(s_main_window, true);

  // Register with Services
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  bluetooth_connection_service_subscribe(bt_handler);
  battery_state_service_subscribe(battery_handler);
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}
static void deinit() {
    tick_timer_service_unsubscribe();
    bluetooth_connection_service_unsubscribe();
    battery_state_service_unsubscribe();
    app_message_deregister_callbacks(); 

    window_destroy(s_main_window);
}
int main(void) {
  init();
  app_event_loop();
  deinit();
}
