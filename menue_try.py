import socket
from datetime import datetime
from openai import OpenAI

server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(('', 12000))
memory = []

client = OpenAI()
messages = [{"role": "system", "content": "Du bist ein Kardiologe und bekommst Messwerte, die die Herzschläge pro Minute eines Patienten angeben. Dies sind Messwerte eines Sensors, die in etwa jede Sekunde neu generiert wurden."}]

def chatbot(input):
  if input:
    messages.append({"role": "user", "content": input})
    chat = client.chat.completions.create(
      model="gpt-3.5-turbo-0125", messages=messages
    )
    reply = chat.choices[0].message.content
    messages.append({"role": "assistant", "content": reply})
    return reply

def dateioeffnen():
    dateiname = input("Wie heißt die gewünschte Datei?\n")
    file = open("Messungen/" + dateiname + ".txt", "r")
    datensatz = ""
    for line in file:
        datensatz = datensatz + line + " "
    file.close()
    return datensatz

while True:
    print("Wie kann ich dir helfen?/n Fragen, die ich dir beantworten kann sind:")
    print("1. Neue Werte messen und speichern")
    print("2. Datensatz laden")
    print("3. Mittelwert bestimmen")
    print("4. Datensatz bereinigen")
    print("5. Vergleich mit anderen Daten")
    print("6. Max/Min BPM ausgeben")
    print("7. Ratschlag von einem Arzt einholen")
    print("8. Beenden")
    anfrage = int(input("Gib die gewünschte Zahl ein"))
    if anfrage > 0 and anfrage < 9:
        if anfrage == 1:
            nutzdauer = float(input("Wie viele Sekunden sollen wir messen?\n"))
            now = datetime.now().timestamp()
            while (datetime.now().timestamp() - now) < nutzdauer:
                message, address = server_socket.recvfrom(1024)
                print(message)
                memory.append(int(message.decode()))
            print("Hier sind deine Messwerte:\n" + memory)
            dateiname = input("Wie möchtest du die Datei nenen?\n")
            file = open("Messungen/" + dateiname + ".txt", "w")
            for i in memory:
                file.write(str(i) + "\n")
            file.close()
        elif anfrage == 2:
            print(chatbot("Hier sind die Messwerte, bitte merke sie dir für später.\n" + dateioeffnen()))
        elif anfrage == 3:
            print(chatbot("Bitte bestimme den Mittelwert meines Datensatz."))
        elif anfrage == 4:
            print(chatbot("Bitte bereinige meinen Datensatz. Die Daten, die du erhalten hast wurden von einem Sensor gemessen, der Fehler aufweisen kann, weshalb es zu unrealistischen Daten kommen kann, die entfernt werden müssen. Bitte arbeite in Zukunft mit den berinigten Daten weiter."))
        elif anfrage == 5:
            print(chatbot("Bitte vergleiche den aktuellen Datensatz mit den Daten, die ich dir jetzt mitgebe\n" + dateioeffnen()))
        elif anfrage == 6:
            print(chatbot("Suche bitte den maximalen und minimalen Wert aus dem Datensatz raus."))
        elif anfrage == 7:
            print(chatbot("Bitte analysiere den dir gegebenen Datensatz und gebe einen ärtzlchen Ratschlag"))
        elif anfrage == 8:
            exit()
    else:
        print("Es muss eine Zahl zwischen 1 und 8 sein")