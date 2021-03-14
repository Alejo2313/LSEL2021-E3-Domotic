from lsel_mqttclient import mqtt_publisher
import sys
pub1=mqtt_publisher(broker_addr=sys.argv[1], gw_name=sys.argv[2],client_id=sys.argv[3])

pub1.publish_msg(topic_base_id="/casa/salon/0001", endpoint_id="1", msg="lore ipsun! \n eeey dora klk")