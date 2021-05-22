#Libraries
import RPi.GPIO as GPIO
import time
class Distance(object):
    def __init__(self,trigger_pin, echo_pin):
        self.trigger_pin = trigger_pin
        self.echo_pin = echo_pin

    def get_trigger_pin(self):
        return self.trigger_pin

    def get_echo_pin(self):
        return self.echo_pin

    def configuration(self):
        """change GPIO mode to BCM and sets trigger pin as output and echo as input
        """
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.get_trigger_pin,GPIO.OUT)
        GPIO.setup(self.get_echo_pin,GPIO.IN)
 
    def distance(self):
        """Calculates the distance of the eco

        Returns:
            int: distance measured in cm
        """
        GPIO.output(self.get_trigger_pin, True)
    
        time.sleep(0.00001)
        GPIO.output(self.get_trigger_pin, False)
    
        StartTime = time.time()
        StopTime = time.time()
    
        while GPIO.input(self.get_echo_pin) == 0:
            StartTime = time.time()
    
        while GPIO.input(self.get_echo_pin) == 1:
            StopTime = time.time()
    
        TimeElapsed = StopTime - StartTime
        # multiply with the sonic speed (34300 cm/s)
        # and divide by 2, because there and back
        distance = (TimeElapsed * 34300) / 2
    
        return distance
    
    def clean_dev(self):
        """clean gpio
        """
        GPIO.cleanup()