import smtplib
from email.message import EmailMessage
import speech_recognition as sr 
import pyttsx3 

# Initialize the voice assistant
engine = pyttsx3.init()

# Get available voices
voices = engine.getProperty('voices')

# Set the voice to female (the first available female voice)
for voice in voices:
    if "female" in voice.name.lower():
        engine.setProperty('voice', voice.id)
        break

def speak(text):
    engine.say(text)
    engine.runAndWait()

# Function to listen for voice input
def listen():
    recognizer = sr.Recognizer()
    with sr.Microphone() as source:
        recognizer.adjust_for_ambient_noise(source)
        print("Listening...")
        audio = recognizer.listen(source)
        try:
            text = recognizer.recognize_google(audio)
            print(f"You said: {text}")
            return text
        except sr.UnknownValueError:
            print("Sorry, I couldn't understand. Please try again.")
            return None
        except sr.RequestError:
            print("Sorry, there was an error with the speech recognition service.")
            return None

def get_valid_input(prompt):
    speak(prompt)
    while True:
        input_text = listen()
        if input_text:
            return input_text
        speak("I didn't catch that. Please try again.")

# Sender email (fixed)
sender_email = "shaikmahammadhussain916@gmail.com"  # This will remain the same
app_password = "qzcu ffzi urqt exdj"  # Replace with your actual app password

# Voice assistant prompts and capture user input with validation
receiver_email = get_valid_input("Please tell me the receiver's email address.")
subject = get_valid_input("Please say the subject of the email.")
content = get_valid_input("Please say the content of the email.")

# Sanitize the receiver email: remove spaces and convert to lowercase
if receiver_email:
    receiver_email = receiver_email.replace(" ", "").lower()

# Ensure receiver email ends with @gmail.com if needed
if receiver_email and not receiver_email.endswith('@gmail.com'):
    receiver_email += "@gmail.com"	
print("receiver_email : ",receiver_email)

# Create the email message
msg = EmailMessage()
msg['From'] = sender_email
msg['To'] = receiver_email
msg['Subject'] = subject
msg.set_content(content)

# Send the email
try:
    with smtplib.SMTP_SSL('smtp.gmail.com', 465) as server:
        server.login(sender_email, app_password)
        server.send_message(msg)
        print("✅ Email sent successfully!")
        speak("Email sent successfully.")
except Exception as e:
    print(f"❌ Error: {e}")
    speak(f"Sorry, there was an error: {e}")
