require "sinatra"
require "./grove"

set :bind, ""

gd = Grove::I2CDev.new
light_sensor = Grove::AnalogInput.new(gd, 2)

get "/" do
  "<h1>Hello world!</h1>
  <p>Light is at #{light_sensor.read} right now</p>"
end
