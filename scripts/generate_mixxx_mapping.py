#!/usr/bin/python


def header():
    return('''
<?xml version='1.0' encoding='utf-8'?>
<MixxxControllerPreset mixxxVersion="" schemaVersion="1">
    <info/>
    <controller id="RtMidi">
        <scriptfiles/>
        <controls>
    ''')

def playButton(ch):
    return('''
            <control>
                <group>[Channel''' + str(ch) +  ''']</group>
                <key>play</key>
                <description>MIDI Learned from 9 messages.</description>
                <status>0x9''' + str(ch - 1) +  '''</status>
                <midino>0x32</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def beatJumpForward(ch):
    return('''
            <control>
                <group>[Channel''' + str(ch) +  ''']</group>
                <key>beatjump_forward</key>
                <description>MIDI Learned from 2 messages.</description>
                <status>0xB''' + str(ch - 1) +  '''</status>
                <midino>0x20</midino>
                <options>
                    <selectknob/>
                </options>
            </control>
    ''')

def beatJumpBackward(ch):
    return('''
            <control>
                <group>[Channel''' + str(ch) +  ''']</group>
                <key>beatjump_backward</key>
                <description>MIDI Learned from 6 messages.</description>
                <status>0xB''' + str(ch - 1) +  '''</status>
                <midino>0x1F</midino>
                <options>
                    <selectknob/>
                </options>
            </control>
    ''')

def loopActivate(ch):
    return('''
            <control>
                <group>[Channel''' + str(ch) +  ''']</group>
                <key>beatloop_activate</key>
                <description>MIDI Learned from 9 messages.</description>
                <status>0x9''' + str(ch - 1) +  '''</status>
                <midino>0x26</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def footer():
    return('''
        </controls>
        <outputs/>
    </controller>
</MixxxControllerPreset>''')


def executeBlock(fnc, *args):
    s = fnc(*args)
    print(s)

def executeBlockForAllChannel(fnc):
    [executeBlock(playButton, ch) for ch in range(1, 5)]

def xmlComment(s, lvl=3):
    xmlBr()
    print(("    " * lvl) + "<-- " + s + " -->")

def xmlBr():
    print("\n")

if __name__ == "__main__":
    executeBlock(header)
    xmlComment("Play Button")
    executeBlockForAllChannel(playButton)
    xmlComment("Position Rotary Encoder - beat jump backward")
    executeBlockForAllChannel(beatJumpBackward)
    xmlComment("Position Rotary Encoder - beat jump forward")
    executeBlockForAllChannel(beatJumpForward)
    xmlComment("Position Rotary Encoder - loop activate")
    executeBlockForAllChannel(loopActivate)
    executeBlock(footer)
