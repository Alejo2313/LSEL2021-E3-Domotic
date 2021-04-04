from lsel_mqttclient import mqtt_subscriber
import sys
# sub1=mqtt_subscriber(broker_addr=sys.argv[1], gw_name=sys.argv[2],client_id=sys.argv[3],
#     subscribe_topic=sys.argv[4])
brokerAddres = "192.168.1.41"
sub1=mqtt_subscriber(broker_addr=brokerAddres, gw_name="gw",client_id="sub",
    subscribe_topic="/casa")

sub1.sub_all_connect()