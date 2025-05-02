import whisper
from pydub import AudioSegment
import csv
import warnings

# Suppress specific warnings
warnings.filterwarnings("ignore", category=UserWarning)
warnings.filterwarnings("ignore", category=FutureWarning)

def transcribe_with_whisper(file_path):
    # Convert to wav format if the file is in m4a
    if file_path.endswith(".m4a"):
        audio = AudioSegment.from_file(file_path, format="m4a")
        file_path = "temp.wav"
        audio.export(file_path, format="wav")
    
    # Load the Whisper model
    model = whisper.load_model("base")  # You can change "base" to "large" for higher accuracy
    
    # Perform transcription
    result = model.transcribe(file_path)
    transcription = result["text"]
    
    return transcription

def save_transcription_to_csv(transcription, output_file="transcription.csv"):
    # Open the CSV file and write the transcription
    with open(output_file, mode="w", newline="", encoding="utf-8") as file:
        writer = csv.writer(file)
        writer.writerow(["Transcription"])  # Column header
        writer.writerow([transcription])    # Transcription content

# Run transcription on the uploaded file
file_path = "recording.wav"  # Replace with the actual file path
transcript = transcribe_with_whisper(file_path)
print("Transcription:", transcript)

# Save the transcription to CSV
save_transcription_to_csv(transcript)
print("Transcription saved to transcription.csv")