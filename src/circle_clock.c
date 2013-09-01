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

int minute_centers[][2] = {
  {72, 10}, {103, 18}, {126, 41}, {134, 72}, {126, 103}, {103, 126}, {72, 134},
  {41, 126}, {18, 103}, {10, 72}, {18, 41}, {41, 18} };

int hour_centers[][2] = {
  {89, 43}, {101, 56}, {105, 72}, {101, 89}, {89, 101}, {72, 105},
  {56, 101}, {43, 89}, {39, 72}, {43, 56}, {55, 43}, {72, 39} };
#define NUMBER_OF_FIDUCIALS 12

Window window;

Layer display_layer;

int tick;

#define CIRCLE_RADIUS 12
#define CIRCLE_LINE_THICKNESS 2

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
  draw_cell(ctx, center, filled, 6);
}

void draw_hour_cell(GContext* ctx, GPoint center, bool filled) {
  draw_cell(ctx, center, filled, 10);
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

  // draw_cell_row_for_digit(ctx, display_hour / 10, HOURS_FIRST_DIGIT_MAX_COLS, HOURS_FIRST_DIGIT_ROW);
  // draw_cell_row_for_digit(ctx, display_hour % 10, DEFAULT_MAX_COLS, HOURS_SECOND_DIGIT_ROW);

  // draw_cell_row_for_digit(ctx, t.tm_min / 10, MINUTES_FIRST_DIGIT_MAX_COLS, MINUTES_FIRST_DIGIT_ROW);
  // draw_cell_row_for_digit(ctx, t.tm_min % 10, DEFAULT_MAX_COLS, MINUTES_SECOND_DIGIT_ROW);

  // draw_cell_row_for_digit(ctx, t.tm_sec / 10, SECONDS_FIRST_DIGIT_MAX_COLS, SECONDS_FIRST_DIGIT_ROW);
  // draw_cell_row_for_digit(ctx, t.tm_sec % 10, DEFAULT_MAX_COLS, SECONDS_SECOND_DIGIT_ROW);

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
