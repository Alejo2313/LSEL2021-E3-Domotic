from lsel_mqttclient import mqtt_publisher
import sys
# pub1=mqtt_publisher(broker_addr=sys.argv[1], gw_name=sys.argv[2],client_id=sys.argv[3])
brokerAddress = "localhost"
pub1=mqtt_publisher(broker_addr=brokerAddress, gw_name="gw",client_id="pub")

pub1.publish_msg("/home/salon/0001",  "S","1","I","alejocagaduro")