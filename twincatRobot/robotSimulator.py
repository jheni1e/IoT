import pyads
import time

AMS_NET_ID = '192.168.56.1.1.1'
ADS_PORT = 851

plc = pyads.Connection(AMS_NET_ID, ADS_PORT)

with plc:
    current_position = plc.read_by_name("GVL.position", pyads.PLCTYPE_INT)
    robot_status = plc.read_by_name("GVL.robotStatus", pyads.PLCTYPE_INT)
    while True:
        target_position = plc.read_by_name("GVL.position", pyads.PLCTYPE_INT)
        if target_position != current_position:
            print(f"Movendo do destino {current_position} para {target_position}...")
            plc.write_by_name("GVL.robotStatus", 0, pyads.PLCTYPE_INT)
            time.sleep(2)
            current_position = target_position
            plc.write_by_name("GVL.robotStatus", 1, pyads.PLCTYPE_INT)
            print(f"Chegou no destino {current_position}.")
        else:
            plc.write_by_name("GVL.robotStatus", 1, pyads.PLCTYPE_INT)
            time.sleep(0.1)