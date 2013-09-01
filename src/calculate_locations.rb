DISPLAY_WIDTH = 144
EDGE_PADDING = 2

CLOCK_CENTER = DISPLAY_WIDTH/2

MINUTE_DOT_RADIUS = 8
HOUR_DOT_RADIUS = 12
HOUR_INSET = 25

MINUTE_CIRCLE_RADIUS = (DISPLAY_WIDTH/2) - EDGE_PADDING - MINUTE_DOT_RADIUS
HOUR_CIRCLE_RADIUS = (DISPLAY_WIDTH/2) - EDGE_PADDING - HOUR_INSET - HOUR_DOT_RADIUS

def degrees_to_radians(deg)
  deg * Math::PI / 180.0
end

def angle_of_tick(clock_tick)
  90.0 - (30.0 * clock_tick)
end

def output_circle_centers(label, dot_radius)
  result = []

  (1..12).each do |clock_tick|
    angle = angle_of_tick(clock_tick)
    radians = degrees_to_radians(angle)

    x = (CLOCK_CENTER + dot_radius * Math::cos(radians)).round
    y = (CLOCK_CENTER - dot_radius * Math::sin(radians)).round

    result << "{#{x}, #{y}}"
  end

  puts result.join(', ')
end

def output_coordinates
  output_circle_centers("minute", MINUTE_CIRCLE_RADIUS)

  output_circle_centers("hour", HOUR_CIRCLE_RADIUS)
end