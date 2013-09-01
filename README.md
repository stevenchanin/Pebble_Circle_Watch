# Pebble Circle Watch
Circular Geometric Watch Face for Pebble Watch

## Background
I had some time over Labor Day weekend and it seemed like it would be fun to try out the Pebble SDK.

Discussed design ideas with my wife and daughter over breakfast and decided to try an analog design that showed hours and minutes using concentric circles that filled in based on what time it was.

## Image
Here is an example from 10:52am.

![example 10:52](https://raw.github.com/stevenchanin/Pebble_Circle_Watch/master/misc/example_10_52am.jpg)

## Build Instructions
### Get a copy
	git clone git@github.com:stevenchanin/Pebble_Circle_Watch.git

### Build it
	./waf configure
	./waf build

### Start a local webserver
	python -m SimpleHTTPServer 8000

### On your phone (on the same LAN)
open your browser, go to the ip address of your laptop on port 8000
for example
	http://192.168.1.xxx:8000/build

Click on Pebble_Circle_Watch.pbw

Open it in the Pebble application
It should install and then be available on your watch as a watchface

## Credit
Code based on the example 'Just A Bit' from the Pebble SDK.

Design credit to Jessie Dotson (https://www.facebook.com/jessie.dotson.9) & Allison Chanin
