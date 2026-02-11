import pyads
import time

AMS_NET_ID = ''
ADS_PORT = 851

positions = {
    0: "PHOME",
    1: "P1",
    2: "P2",
    3: "P3",
    4: "P4",
    5: "P5"
}

plc = pyads.Connection(AMS_NET_ID, ADS_PORT)

try:
    with plc:
        print(f"Conexão com: {plc.ip_address}")

        plc.write_by_name("GVL.robotStatus", 0, pyads.PLCTYPE_INT)
        plc.write_by_name("GVL.position", 0, pyads.PLCTYPE_INT)
        plc.write_by_name("GVL.gripperStatus", 0, pyads.PLCTYPE_INT)

        last_position = plc.read_by_name("GVL.position", pyads.PLCTYPE_INT)

        while True:
            while True:
                try:
                    position = int(input("Escolha a posição: "))
                    if position not in positions:
                        print("Posição inválida. Tente novamente.")
                    else:
                        break
                except ValueError:
                    print("Digite apenas números inteiros.")

            if position != last_position:
                print(f"Posição alterada: {positions[position]} indo...")
                plc.write_by_name("GVL.position", position, pyads.PLCTYPE_INT)
                plc.write_by_name("GVL.robotStatus", 0, pyads.PLCTYPE_INT)
                last_position = position

            while True:
                try:
                    grip = int(input("Escolha a garra (0=Fechada, 1=Aberta): "))
                    if grip in (0, 1):
                        break
                except ValueError:
                    pass

            plc.write_by_name("GVL.gripperStatus", grip, pyads.PLCTYPE_INT)
            print("Garra aberta." if grip else "Garra fechada.")

            print("Indo...")
            time.sleep(2)
            plc.write_by_name("GVL.robotStatus", 1, pyads.PLCTYPE_INT)
            print(f"Destino {positions[position]} alcançado!\n")
except Exception as e:
    print(f"Erro: {e}")
