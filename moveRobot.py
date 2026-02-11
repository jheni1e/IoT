import pyads

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

        while True:

            robotStatus = plc.read_by_name("GVL.robotStatus", pyads.PLCTYPE_INT)

            if robotStatus == 1:
                print("Destino alcançado!")
                break

            while True:
                try:
                    position = int(input("Escolha a posição: "))
                    if position not in positions:
                        print("Posição inválida. Tente novamente.")
                    else:
                        break
                except ValueError:
                    print("Digite apenas números inteiros.")

            plc.write_by_name("GVL.position", position, pyads.PLCTYPE_INT)
            currentPosition = plc.read_by_name("GVL.position", pyads.PLCTYPE_INT)
            print(f"Posição atual: {positions.get(currentPosition)}")

            while True:
                try:
                    grip = int(input("Escolha a garra (0=Fechada, 1=Aberta): "))
                    if grip not in (0, 1):
                        print("Valor inválido. Digite 0 ou 1.")
                    else:
                        break
                except ValueError:
                    print("Digite apenas 0 ou 1.")

            plc.write_by_name("GVL.gripperStatus", grip, pyads.PLCTYPE_INT)
            currentGrip = plc.read_by_name("GVL.gripperStatus", pyads.PLCTYPE_INT)

            if currentGrip == 0:
                print("Garra fechada.")
            elif currentGrip == 1:
                print("Garra aberta.")

            while True:
                try:
                    status = int(input("Chegou ao destino? (0=Não, 1=Sim): "))
                    if status not in (0, 1):
                        print("Resposta inválida. Digite 0 ou 1.")
                    else:
                        break
                except ValueError:
                    print("Digite apenas 0 ou 1.")

            plc.write_by_name("GVL.robotStatus", status, pyads.PLCTYPE_INT)

            if status == 0:
                print("Indo...")

except Exception as e:
    print(f"Erro: {e}")
