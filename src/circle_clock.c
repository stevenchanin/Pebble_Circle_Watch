#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

/* Based on the Pebble SDK example 'Just A Bit' */

#define MY_UUID { 0x90, 0x8A, 0x6D, 0x42, 0x5F, 0xB6, 0x4C, 0x72, 0xAD, 0xD9, 0xDB, 0xDD, 0x76, 0xB7, 0x83, 0x5A }
PBL_APP_INFO(MY_UUID,
             "Circle Clock", "Development Leverage",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

#define HOUR_CIRCLE_RADIUS 9
#define MINUTE_CIRCLE_RADIUS 6
#define CIRCLE_LINE_THICKNESS 2

#define DATE_WIDTH 30
#define DATE_HEIGHT 28

int minute_centers[][2] = {
  {72, 10}, {103, 18}, {126, 41}, {134, 72}, {126, 103}, {103, 126}, {72, 134},
  {41, 126}, {18, 103}, {10, 72}, {18, 41}, {41, 18} };

int hour_centers[][2] = {
  {89, 43}, {101, 56}, {105, 72}, {101, 89}, {89, 101}, {72, 105},
  {56, 101}, {43, 89}, {39, 72}, {43, 56}, {55, 43}, {72, 39} };
#define NUMBER_OF_FIDUCIALS 12

Window window;

Layer display_layer;
TextLayer date_layer;

int tick;

void draw_cell(GContext* ctx, GPoint center, bool filled, int circle_radius) {
  // Each "cell" represents a binary digit or 0 or 1.

  graphics_context_set_fill_color(ctx, GColorWhite);

  graphics_fill_circle(ctx, center, circle_radius);

  if (!filled) {
    // This is our ghetto way of drawing circles with a line thickness
    // of more than a single pixel.
    graphics_context_set_fill_color(ctx, GColorBlack);

    graphics_fill_circle(ctx, center, circle_radius - CIRCLE_LINE_THICKNESS);
  }

}

void draw_minute_cell(GContext* ctx, GPoint center, bool filled) {
  draw_cell(ctx, center, filled, MINUTE_CIRCLE_RADIUS);
}

void draw_hour_cell(GContext* ctx, GPoint center, bool filled) {
  draw_cell(ctx, center, filled, HOUR_CIRCLE_RADIUS);
}

unsigned short get_display_hour(unsigned short hour) {
  unsigned short display_hour = hour % 12;

  // Converts "0" to "12"
  return display_hour ? display_hour : 12;

}

unsigned short get_display_minute(unsigned short minute) {
  return minute / 5;

}

void display_layer_update_callback(Layer *me, GContext* ctx) {
  static char date_text[] = "MMM-DD"; // Needs to be static because it's used by the system later.

  PblTm t;

  get_time(&t);

  unsigned short display_hour = get_display_hour(t.tm_hour);
  unsigned short display_minute = get_display_minute(t.tm_min);

  /* pulse the seconds */
  draw_hour_cell(ctx, GPoint(72, 72), tick);
  tick = 1 - tick;

  for(int i=0; i < NUMBER_OF_FIDUCIALS; i++) {
    int x = minute_centers[i][0];
    int y = minute_centers[i][1];

    if (i <= display_minute) {
      draw_minute_cell(ctx, GPoint(x, y), true);
    } else {
      draw_minute_cell(ctx, GPoint(x, y), false);
    }
  }

  for(int i=0; i < NUMBER_OF_FIDUCIALS; i++) {
    int x = hour_centers[i][0];
    int y = hour_centers[i][1];

    if (i < display_hour) {
      draw_hour_cell(ctx, GPoint(x, y), true);
    } else {
      draw_hour_cell(ctx, GPoint(x, y), false);
    }
  }

  graphics_draw_line(ctx, GPoint(0,168 - DATE_HEIGHT + 2), GPoint(144, 168 - DATE_HEIGHT + 2));
  graphics_draw_line(ctx, GPoint(0,168 - DATE_HEIGHT + 3), GPoint(144, 168 - DATE_HEIGHT + 3));
  string_format_time(date_text, sizeof(date_text), "%b-%d", &t);
  text_layer_set_text(&date_layer, date_text);

}

void handle_second_tick(AppContextRef ctx, PebbleTickEvent *t) {

  layer_mark_dirty(&display_layer);
}

void handle_init(AppContextRef ctx) {

  window_init(&window, "Circle Clock");
  window_stack_push(&window, true /* Animated */);

  window_set_background_color(&window, GColorBlack);

  // Init the layer for the display
  layer_init(&display_layer, window.layer.frame);
  display_layer.update_proc = &display_layer_update_callback;
  layer_add_child(&window.layer, &display_layer);

  text_layer_init(&date_layer, 
    GRect(72-DATE_WIDTH, 168-DATE_HEIGHT-2, 72+DATE_WIDTH, 168-2));
  text_layer_set_text_color(&date_layer, GColorWhite);
  text_layer_set_background_color(&date_layer, GColorClear);
  text_layer_set_font(&date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(&window.layer, &date_layer.layer);

  tick = 0;
 }


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    
    .tick_info = {
      .tick_handler = &handle_second_tick,
      .tick_units = SECOND_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
