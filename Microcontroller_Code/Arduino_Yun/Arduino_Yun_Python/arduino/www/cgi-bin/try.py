#!/usr/bin/python

import urllib2,json

print

###### Make sure to update the api with yours #############################
###### Replace INSERTAPI with the number ################################
req = urllib2.Request('http://api.wunderground.com/api/INSERTAPI/hourly10day/q/OH/Cincinnati.json')
data = ''

try: 
    response = urllib2.urlopen(req)
    data = json.load(response)

except URLError as e:
    print e.reason   
except urllib2.HTTPError as e:
    print e.code
    print e.read() 

#print 'DATA:', (data)
#print data["hourly_forecast"]["FCTTIME"]["epoch"]
#print data["hourly_forecast"]["temp"]["english"]
#print data["hourly_forecast"]["pop"]
#print data["hourly_forecast"]["qpf"]["english"]

loadmain = data['hourly_forecast']

epoch = [ hourly_forecast['FCTTIME']['epoch'] for hourly_forecast in loadmain ]
epoch = list(map(int, epoch))
print epoch

pop = [ hourly_forecast['pop'] for hourly_forecast in loadmain ]
pop = list(map(int, pop))
print pop

qpf = [ hourly_forecast['qpf']['english'] for hourly_forecast in loadmain ]
#qpf[qpf=='']='0'

def FloatOrZero(value):
    try:
        return float(value)
    except:
        return 0.0

#qpf = [float(i) for i in qpf]
qpf = list(map(FloatOrZero, qpf))
print qpf

temperature = [ hourly_forecast['temp']['english'] for hourly_forecast in loadmain ]
temperature = list(map(int, temperature))
print temperature

ProbPrecip = map(lambda x, y: x*y/100, qpf, pop)
#[a*b for a,b in zip(lista,listb)]
print ProbPrecip

print sum(ProbPrecip)

#would be cool to include a way to drain it at the beginning of the storm if the capacity will not be enough to capture the peak of the storm