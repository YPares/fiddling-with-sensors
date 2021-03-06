#!/usr/bin/python
#encoding=utf8

import grovepi as g
import grove_rgb_lcd as l
import math
import time
import httplib
import json
import traceback

def clamp(m1, x, m2):
    return min(max(m1, x), m2)

def e(v):
    return clamp(0,int(v),255)

def setColor(time, w, temp, humid):
    ## Hour factor (brightness)
    hr = 0
    ## Temp factor (inc. red)
    tm = 0
    # Humidity factor (inc. blue)
    hm = (clamp(0,humid,100) - 50) * 1.5
    l.setRGB(e(128 + hr + tm), e(128 + hr), e(128 + hr + hm))

def logAll(t, w, temp, humid):
    s = time.localtime(t)
    with open("/home/pi/tempHumid.csv", "a") as f:
        f.write(",".join(str(x) for x in (t, s.tm_year, s.tm_mon, s.tm_mday, s.tm_hour, s.tm_min, s.tm_sec, temp, humid, w.m["temp"], w.m["humidity"])) + "\n")
    with open("/home/pi/tempHumid.json", "a") as f:
        json.dump(w.bulletin, f)
        f.write("\n")

class Weather:
    def __init__(self, city, countryCode):
        self.bulletin = None
        self.__last_refresh = 0
        self.__city = city
        self.__cc = countryCode

    def update(self, t):
        if not self.bulletin or t - last_resfresh >= 10*60:
            self.bulletin = self.__get()
            self.__last_refresh = t

    def __get(self):
        h=httplib.HTTPConnection("api.openweathermap.org")
	h.request("GET","/data/2.5/weather?q={:s},{:s}&units=metric&APPID=e76a982d1b921bc16c1a5f8d2d973085".format(self.__city, self.__cc))
        return json.load(h.getresponse())
            
    @property
    def m(self):
        return self.bulletin[u"main"]

def props(obj):
    return dict((name, getattr(obj, name)) for name in dir(obj) if name.startswith("tm"))

# O's are more readable then zeros on the LCD
def repl0(args):
    return [("{:.1f}".format(x).replace("0","O") if x else "0") \
            for x in args]

def readTH():
    temp = float("nan")
    humid = float("nan")
    while (math.isnan(temp) or math.isnan(humid)):
        try:
            [temp,humid]=g.dht(7,0)
        except:
            print(time.ctime(t))
            print(traceback.format_exc())
    return [temp, humid]

while True:
    try:
	[temp, humid] = readTH()
	t = time.time()
        s = time.localtime(t)
        w = Weather("Neuilly-Plaisance", "fr")
        w.update(t)
        w.bulletin[u"home"] = {u"temp": temp, u"humidity": humid}
        w.bulletin[u"time"] = props(s)
	setColor(s, w, temp, humid)
	l.setText("Ext: {:s}C {:s}%\nInt: {:s}C {:s}%".format( \
                  *repl0((w.m["temp"], w.m["humidity"], temp, humid))))
        logAll(t, w, temp, humid)
    except:
        print(time.ctime(t))
        print(traceback.format_exc())
    time.sleep(60)

