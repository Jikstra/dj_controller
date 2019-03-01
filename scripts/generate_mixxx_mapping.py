#!/usr/bin/python


def header():
    return('''<?xml version='1.0' encoding='utf-8'?>
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

def beatSyncButton(ch):
    return('''
            <control>
                <group>[Channel''' + str(ch) +  ''']</group>
                <key>beatsync_tempo</key>
                <description>MIDI Learned from 1 messages.</description>
                <status>0x9''' + str(ch - 1) +  '''</status>
                <midino>0x33</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def headphoneListenButton(ch):
    return('''
            <control>
                <group>[Channel2]</group>
                <key>pfl</key>
                <description>MIDI Learned from 1 messages.</description>
                <status>0x90</status>
                <midino>0x34</midino>
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
                <midino>0x1F</midino>
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
                <midino>0x20</midino>
                <options>
                    <selectknob/>
                </options>
            </control>
    ''')

def reloopToggle(ch):
    return('''
            <control>
                <group>[Channel''' + str(ch) +  ''']</group>
                <key>reloop_toggle</key>
                <description>MIDI Learned from 10 messages.</description>
                <status>0x9''' + str(ch - 1) +  '''</status>
                <midino>0x21</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def loopDouble(ch):
    return('''
            <control>
                <group>[Channel''' + str(ch) +  ''']</group>
                <key>loop_double</key>
                <description>MIDI Learned from 7 messages.</description>
                <status>0xB''' + str(ch - 1) +  '''</status>
                <midino>0x24</midino>
                <options>
                    <selectknob/>
                </options>
            </control>
    ''')

def loopHalve(ch):
    return('''
            <control>
                <group>[Channel''' + str(ch) +  ''']</group>
                <key>loop_halve</key>
                <description>MIDI Learned from 8 messages.</description>
                <status>0xB''' + str(ch - 1) +  '''</status>
                <midino>0x25</midino>
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
                <description>MIDI Learned from 1 messages.</description>
                <status>0x9''' + str(ch - 1) +  '''</status>
                <midino>0x26</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def volumeRate(ch):
    return('''
            <control>
                <group>[Channel''' + str(ch) +  ''']</group>
                <key>volume</key>
                <description>MIDI Learned from 7 messages.</description>
                <status>0xB''' + str(ch - 1) +  '''</status>
                <midino>0x05</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def volumeKill(ch):
    return('''
            <control>
                <group>[Channel''' + str(ch) +  ''']</group>
                <key>volume_set_zero</key>
                <description>MIDI Learned from 1 messages.</description>
                <status>0x9''' + str(ch - 1) +  '''</status>
                <midino>0x06</midino>
                <options>
                    <switch/>
                </options>
            </control>
    ''')


def tempoRate(ch):
    return('''
            <control>
                <group>[Channel''' + str(ch) +  ''']</group>
                <key>rate</key>
                <description>MIDI Learned from 4 messages.</description>
                <status>0xB''' + str(ch - 1) +  '''</status>
                <midino>0x07</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def tempoReset(ch):
    return('''
            <control>
                <group>[Channel''' + str(ch) +  ''']</group>
                <key>rate_set_default</key>
                <description>MIDI Learned from 2 messages.</description>
                <status>0x9''' + str(ch - 1) +  '''</status>
                <midino>0x08</midino>
                <options>
                    <switch/>
                </options>
            </control>
    ''')

def generateEQRate(parameter, midino):
    def eqRate(ch):
        return('''
                <control>
                    <group>[EqualizerRack1_[Channel''' + str(ch) +  ''']_Effect1]</group>
                    <key>parameter''' + str(parameter) + '''</key>
                    <description>MIDI Learned from 2 messages.</description>
                    <status>0xB''' + str(ch - 1) +  '''</status>
                    <midino>''' + midino + '''</midino>
                    <options>
                        <normal/>
                    </options>
                </control>
        ''')
    return eqRate

def generateEQKill(parameter, midino):
    def eqKill(ch):
        return('''
                <control>
                    <group>[EqualizerRack1_[Channel''' + str(ch) +  ''']_Effect1]</group>
                    <key>button_parameter''' + str(parameter) + '''</key>
                    <description>MIDI Learned from 2 messages.</description>
                    <status>0x9''' + str(ch - 1) +  '''</status>
                    <midino>''' + midino + '''</midino>
                    <options>
                        <normal/>
                    </options>
                </control>
        ''')
    return eqKill

def generateFXRate(parameter, midino):
    def fxRate(ch):
        return('''
            <control>
                <group>[EffectRack1_EffectUnit''' + str(ch) +  '''_Effect''' + parameter + ''']</group>
                <key>meta</key>
                <description>MIDI Learned from 8 messages.</description>
                <status>0xB''' + str(ch - 1) +  '''</status>
                <midino>''' + midino + '''</midino>
                <options>
                    <normal/>
                </options>
            </control>
        ''')
    return fxRate
   

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
    [executeBlock(fnc, ch) for ch in range(1, 5)]

def xmlComment(s, lvl=3):
    xmlBr()
    print(("    " * lvl) + "<!-- " + s + " -->")

def xmlBr():
    print("\n")

if __name__ == "__main__":
    executeBlock(header)
    xmlComment("Play Button")
    executeBlockForAllChannel(playButton)
    xmlComment("BeatSync Button")
    executeBlockForAllChannel(beatSyncButton)
    xmlComment("HeadphoneListen Button")
    executeBlockForAllChannel(headphoneListenButton)
        
    xmlComment("Position Rotary Encoder - beat jump backward")
    executeBlockForAllChannel(beatJumpBackward)
    xmlComment("Position Rotary Encoder - beat jump forward")
    executeBlockForAllChannel(beatJumpForward)
    xmlComment("Position Rotary Encoder - toggle loop")
    executeBlockForAllChannel(reloopToggle)

    xmlComment("Loop Rotary Encoder - halve loop")
    executeBlockForAllChannel(loopHalve)
    xmlComment("Loop Rotary Encoder - double loop")
    executeBlockForAllChannel(loopDouble)
    xmlComment("Loop Rotary Encoder - loop activate")
    executeBlockForAllChannel(loopActivate)
    
    xmlComment("Volume Rotary Encoder - Rate")
    executeBlockForAllChannel(volumeRate)
    xmlComment("Loop Rotary Encoder - Kill")
    executeBlockForAllChannel(volumeKill)
    
    xmlComment("Tempo Rotary Encoder - Rate")
    executeBlockForAllChannel(tempoRate)
    xmlComment("Tempo Rotary Encoder - Reset")
    executeBlockForAllChannel(tempoReset)

    xmlComment("Low EQ Rotary Encoder - Rate")
    executeBlockForAllChannel(generateEQRate(1, '0x09'))
    xmlComment("Low EQ Rotary Encoder - Mute")
    executeBlockForAllChannel(generateEQKill(1, '0x0A'))

    xmlComment("Mid EQ Rotary Encoder - Rate")
    executeBlockForAllChannel(generateEQRate(2, '0x0B'))
    xmlComment("Mid EQ Rotary Encoder - Mute")
    executeBlockForAllChannel(generateEQKill(2, '0x0C'))

    xmlComment("High EQ Rotary Encoder - Rate")
    executeBlockForAllChannel(generateEQRate(3, '0x0D'))
    xmlComment("High EQ Rotary Encoder - Mute")
    executeBlockForAllChannel(generateEQKill(3, '0x0E'))

    xmlComment("Effect 1 Rotary Encoder - Rate")
    executeBlockForAllChannel(generateEQRate(3, '0x0D'))
    xmlComment("Effect 1 Rotary Encoder - Toggle")
    executeBlockForAllChannel(generateEQKill(3, '0x0E'))

    executeBlock(footer)
