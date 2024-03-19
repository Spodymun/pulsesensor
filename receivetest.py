import socket
from datetime import datetime

server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(('', 12000))
memory = []
nutzdauer = float(input("Wie viele Sekunden sollen wir messen?\n"))
now = datetime.now().timestamp()

while (datetime.now().timestamp()-now)<nutzdauer:
    message, address = server_socket.recvfrom(1024)
    print(message)
    memory.append(int(message.decode()))

print(memory)
print("Hab fertig :)")

dateiname = input("Wie möchtest du die Datei nenen?\n")
file = open("Messungen/" + dateiname + ".txt", "w")
for i in memory:
    file.write(str(i) + "\n")
file.close()



