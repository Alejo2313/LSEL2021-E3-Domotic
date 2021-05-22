import pygame
class audio(object):
    def __init__(self,song_name):
        self.song_name = song_name
    
    def get_song(self):
        return self.song_name

    def play_audio(self):
        pygame.mixer.init()
        pygame.mixer.music.load(self.get_song)
        pygame.mixer.music.play()
        while pygame.mixer.music.get_bussy() == True:
            continue