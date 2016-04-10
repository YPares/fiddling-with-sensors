#!/usr/bin/python
#encoding=utf8

from bottle import route, run, template
import grovepi as g

sensor_pin = 7


@route('/')
def index():
    try:
        [temp, humid] = g.dht(sensor_pin, 0)
        return template("<h1>Le capteur à Gégé</h1><br/><b>Le capteur dit :\
                     <br/>Température : {{temp}}°C\
                     <br/>Humidité : {{humid}}%", temp=temp, humid=humid)
    except Exception, e:
        return template("<h1>Oups, rechargez la page</h1>")
    
@route('/hello/<name>')
def index(name):
    return template('<b>Hello {{name}}</b>!', name=name)

run(host='', port=8080)
