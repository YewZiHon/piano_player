from MIDI import MIDIFile
import shutil

def parse(file):
    newfile = "temp.midi"
    shutil.copy(file, newfile)
    notearray = []
    c=MIDIFile(newfile)
    c.parse()
    for _, track in enumerate(c):
        track.parse()
        for line in track:
            try:
                if line.command ==0x80 or line.command == 0x90:
                    if str(line.message.note) not in notearray:
                        #print(line.message.note)
                        notearray.append(str(line.message.note))
            except:pass
        break
    print(len(notearray))


parse(r"C:\Users\Zi Hon\Documents\GitHub\piano_player\Slime Rancher - Indigo Quarry Piano Tutorial.mp4_output.mid")

