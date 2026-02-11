import pyads

AMS_NET_ID = ''
ADS_PORT = 851
TARGET_IP = ''

plc = pyads.Connection(AMS_NET_ID, ADS_PORT, TARGET_IP)

with plc:
    print(f"Conexão com: {plc.ip_address}")
    
    num = int(input("Digite um número: "))
    is_written = plc.write_by_name("GVL.variable", num, pyads.PLCTYPE_INT)
    
    result = plc.read_by_name("GVL.variable", pyads.PLCTYPE_INT)
    print(result)
