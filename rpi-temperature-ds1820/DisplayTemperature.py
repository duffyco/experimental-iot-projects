#!/usr/bin/python
 
# Uses Dallas DS18B20 One-wire Temperature Sensor
 
# Displays Temperature in Degrees F to 16X2 LCD Display and to stdio.
 
# Supports two sensors, one on the breadboard and the other a sensor on a cable connected to the breadboard
 
# User chooses interval between measurements.
 
# Program stopped by Ctrl-C or by switch on breadboard
 
# Thanks to Simon Monk and his Adafruit tutorial: " ...DS18B20 Temperature Sensing"
 
# MJL -- www.thepiandi.blogspot.com --  7/1/2013
 
import os
from datetime import datetime
# from datetime import timedelta  #for testing the totaltime function
from time import sleep
import subprocess
import sys
 
def loadmodules():
    """ Checks to see if the 1-wire modules are loaded.  If not, loades them."""
    err = 'error'
    while err != '':
        modloaded = subprocess.Popen(['lsmod'], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
        out, err = modloaded.communicate()
    if 'w1_therm' not in out or 'w1_gpio' not in out:
        print '1-Wire modules had to be loaded.  Waiting 10 seconds'
        print
        os.system('sudo modprobe w1-gpio')        
        os.system('sudo modprobe w1-therm')
        sleep(10)
        
def read_temp_raw():
    
    """ Uses Popen to open then read contents of w-slave. Returns the contents of the file as a two line list.
    If the file cannot be read the 1-wire modules are unloaded and loaded again. This is repeated
    three times until the file is read successfully.  After three times, the program
    is stopped."""
    
    global glitches
    trials = 3
    catdata = subprocess.Popen(['cat', device_file], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
    out, err = catdata.communicate()
    while err != "" and trials:
        print "Got a glitch - trying to recover"        
        trials -= 1
        sleep(1)
        os.system('sudo modprobe -r w1-gpio')
        os.system('sudo modprobe -r w1-therm')
        sleep(1)
        os.system('sudo modprobe w1-gpio')
        os.system('sudo modprobe w1-therm')
        sleep(10)
        glitches += 1
        catdata = subprocess.Popen(['cat', device_file], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
        out, err = catdata.communicate()
    if trials:
        out_decode = out.decode('utf-8')
        lines = out_decode.split('\n')
        return lines        
    else:
        raise(IOError)
        
def totaltime():
    """ Determines the total run time and prints a formated string of day, if over a day, hour, minute
    and second.  The string elapsedtime will be quite different depending if the run goes over a day."""
 
    global starttime
    print starttime.strftime("Started run %A, %B %d at %I:%M:%S %p")
    elapsedtime = str(datetime.now() - starttime)
#    elapsedtime = str(datetime.now() - starttime + timedelta(hours = 23.9)) # for testing the function
    firstcolon = elapsedtime.find(':')
    minutes = int(elapsedtime[firstcolon+1:firstcolon+3])
    seconds = float(elapsedtime[firstcolon+4:])
    
    dayposition = elapsedtime.find('day')
    if dayposition != -1:
           days = int(elapsedtime[:dayposition - 1])
           hours = int(elapsedtime[firstcolon -2:firstcolon])
           print "total run time = %2d day %2d hours, %2d minutes, %4.1f seconds" %(days, hours, minutes, seconds)    
    else:     
        hours = int(elapsedtime[:firstcolon])
        print "total run time = %2d hours, %2d minutes, %4.1f seconds" %(hours, minutes, seconds)
 
def check_switch():
    """Checks to see if the breadboard switch was pressed.  If so it generates a keyboard interrupt
    as if CTRL-C was pressed.  Switch must be held down for at least .2 seconds to be reconized"""
    input_switch = GPIO.input(SwitchInput) # Look for breadboard switch
    if not input_switch:
        sleep(.2)
        input_switch = GPIO.input(SwitchInput) # Look for breadboard switch again
        if not input_switch: # Must still be pressed
            print
            print "Switch Pressed"
            raise(KeyboardInterrupt)
    
 
# --------------------------------------------------------------------------------------------------
 
# Main Program
 
glitches = 0
starttime = datetime.now()
device_id = ('28-00000400d39d' ,'28-000004986cbb')
prompt = 'Enter the minimum time between measurements in seconds: '
prompt1 = 'Enter 0 for sensor on breadboard, 1 for cable sensor: '
short_wait = .1
 
print
measurement_interval = 0
while not measurement_interval:
    try:
        measurement_interval = float(raw_input(prompt))
        break
    except:
        pass
 
print
choice = 'none'
while choice != '0' and choice != '1': 
    choice = raw_input(prompt1) 
print
device_file = '/sys/bus/w1/devices/' + device_id[int(choice)] + '/w1_slave'
if int(choice):
    sensor = 'cable'
else:
    sensor = 'breadboard'
 
loadmodules()   # Check to see if 1-Wire modules are loaded.
   
try:
    while True:
        lines = read_temp_raw()
        if 'YES' in lines[0]:
            equals_pos = lines[1].find('t=')
            if equals_pos != -1:
                temp_string = lines[1][equals_pos +2:]
                Deg_C = float(temp_string)/1000.0
                Deg_F = round(Deg_C * 1.8 + 32.0, 1)
                timenow = datetime.now()
                print timenow.strftime("%A, %B %d, %I:%M:%S %p") + (", the %s sensor temperature is %3.1f") %(sensor, Deg_F) + u"\xB0" +"F"
                downtime = measurement_interval
            else:
                downtime = short_wait
        else:
            downtime = short_wait
        if downtime != short_wait:
            check_switch()
            while downtime:
                check_switch()
                sleep(1)
                downtime -= 1
        else:              
            sleep(downtime)
                          
except(KeyboardInterrupt):
    print
 
except(IOError):
    print
    print 'Three tries and we are out of here'    
 
except:
    print
    print "Unexpected Error: ", sys.exc_info()[1]
        
print
print "See you later, Glitches = %d" % glitches
totaltime()
 
GPIO.cleanup()

