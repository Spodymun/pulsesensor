from openai import OpenAI
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

nutzer_eingabe = input("Wie kann ich helfen?")
print(chatbot(nutzer_eingabe))