# Exploring Procedural Methods for Use in SpeechTherapy for Aphasia Patients

This project, as part of my master's dissertation, investigates procedural methods for helping with the rehabilitation of patients with Aphasia. Aphasia is a language disorder resulting from the damage to the region of the brain that handles language. A procedural approach means that the application could be easily adapted to individual patients.

The role of the application is to provide a platform where those in need of speech therapy can practice in between therapy sessions. 

### Visual Cue: Real time Procedural Generation of Animation Based on Audio

|  <img  src="https://github.com/SharonGomez/speech-therapy/blob/main/Demos/repetition.gif" width="772" height="475"  alt="Repetition Screen Grab"/>|
|:--:| 
| <p align="justify">Learning how to speak involves both listening and reading lip movements. Procedurally animating metahumans based on audios would enable speech therapists to provide visual cues along with audio for the patient to practice with by simply adding in audios of materials to practice before next consultation. For the real time procedural generation of the facial animation on the Metahuman, improvements were made on top of the Oculus Lipsync module to make the animation better and more profound, control the speed and make the process realtime. </p>|

### Feedback: Real Time Conversion of Speech to Text to Assess the Response of the Patient

One of the main issues with aphasia is the disconnect it causes between what a person thinks and says. Without proper feedback those affected might feel like what they are saying is correct while speaking out completely unrelated things. To avoid this when practicing, audio is converted to text real time and compared with the expected outcome. There is an option to slow down the audio and animation to enable easier practice. Doing practice with the metahuman avatar would create a learning environment without fear of judgment.
For the real time conversion of speech to text of the patients, the Runtime Speech Recognizer based on the Whisper OpenAI Technology was used. 

### Other Methods Explored
| <img src="https://github.com/SharonGomez/speech-therapy/blob/main/Demos/dall-e.png" width="375" height="250"/> <img src="https://github.com/SharonGomez/speech-therapy/blob/main/Demos/conversation.png" width="375" height="250"/>|
|:--:|
| <p align="justify"> Visuographic supports in the form of images and conversational coaching using a partner are also important parts of treatment of aphasia. This is where the integration of generative AI for intelligent conversations and images along with the core modules of practicing with visual and audio cues could be explored. For this project I have explored adding them in the project at a minimal level.</p>|



[PDF Documentation to the Dissertation](https://github.com/SharonGomez/speech-therapy/releases/tag/v1.0/Procedural_Approach_to_Speech_Therapy_for_Aphasia_Patients.pdf)
