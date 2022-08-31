#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {
0x0d,			// Number of tracks
0x00, 0x00,		// Address of track 0
0x05, 0x00,		// Address of track 1
0x30, 0x00,		// Address of track 2
0x39, 0x00,		// Address of track 3
0x3a, 0x00,		// Address of track 4
0x4b, 0x00,		// Address of track 5
0x64, 0x00,		// Address of track 6
0x67, 0x00,		// Address of track 7
0x7a, 0x00,		// Address of track 8
0x85, 0x00,		// Address of track 9
0x96, 0x00,		// Address of track 10
0xaf, 0x00,		// Address of track 11
0xc2, 0x00,		// Address of track 12
0x00,			// Channel 0 entry track
0x01,			// Channel 1 entry track
0x02,			// Channel 2 entry track
0x03,			// Channel 3 entry track
//"Track channel 0"
0x40, 48,		// set volume
0x9D, 16,		// set tempo
0x9F,			// FX: STOP CURRENT CHANNEL
//"Track channel 1"
0x40, 32,		// set volume
0x9D, 16,		// set tempo
0xFD, 3, 6,		// REPEAT: count = 3 + 1 / track = 6
0xFC, 5,		// GOTO track 5
0xFC, 7,		// GOTO track 7
0xFC, 5,		// GOTO track 5
0xFC, 8,		// GOTO track 8
0xFC, 5,		// GOTO track 5
0xFC, 7,		// GOTO track 7
0xFC, 5,		// GOTO track 5
0xFC, 8,		// GOTO track 8
0xFD, 3, 6,		// REPEAT: count = 3 + 1 / track = 6
0xFC, 10,		// GOTO track 10
0xFC, 11,		// GOTO track 11
0xFC, 10,		// GOTO track 10
0xFC, 12,		// GOTO track 12
0xFC, 10,		// GOTO track 10
0xFC, 11,		// GOTO track 11
0xFC, 10,		// GOTO track 10
0xFC, 12,		// GOTO track 12
0x9F,			// FX: STOP CURRENT CHANNEL
//"Track channel 2"
0x40, 32,		// set volume
0xFD, 11, 4,		// REPEAT: count = 11 + 1 / track = 4
0xFD, 11, 9,		// REPEAT: count = 11 + 1 / track = 9
0x9F,			// FX: STOP CURRENT CHANNEL
//"Track channel 3"
0x9F,			// FX: STOP CURRENT CHANNEL
//"Track bass 1"
0x00 + 25,		// NOTE ON: note = 25
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0x00 + 32,		// NOTE ON: note = 32
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0x00 + 20,		// NOTE ON: note = 20
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0x00 + 32,		// NOTE ON: note = 32
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0xFE,			// RETURN
//"Track Track 2"
0x00 + 30,		// NOTE ON: note = 30
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 32,		// NOTE ON: note = 32
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 34,		// NOTE ON: note = 34
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 32,		// NOTE ON: note = 32
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 37,		// NOTE ON: note = 37
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0x00 + 25,		// NOTE ON: note = 25
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0xFE,			// RETURN
//"Track blank"
0x00 + 0,		// NOTE ON: note = 0
0x9F + 16,		// DELAY: ticks = 16
0xFE,			// RETURN
//"Track Track 4"
0x00 + 0,		// NOTE ON: note = 0
0x9F + 4,		// DELAY: ticks = 4
0x00 + 34,		// NOTE ON: note = 34
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 32,		// NOTE ON: note = 32
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 37,		// NOTE ON: note = 37
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0x00 + 25,		// NOTE ON: note = 25
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0xFE,			// RETURN
//"Track Track 5"
0x00 + 0,		// NOTE ON: note = 0
0x9F + 4,		// DELAY: ticks = 4
0x00 + 37,		// NOTE ON: note = 37
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0x00 + 25,		// NOTE ON: note = 25
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 7,		// DELAY: ticks = 7
0xFE,			// RETURN
//"Track Track 6"
0x00 + 24,		// NOTE ON: note = 24
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0x00 + 31,		// NOTE ON: note = 31
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0x00 + 20,		// NOTE ON: note = 20
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0x00 + 31,		// NOTE ON: note = 31
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0xFE,			// RETURN
//"Track Track 7"
0x00 + 29,		// NOTE ON: note = 29
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 31,		// NOTE ON: note = 31
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 33,		// NOTE ON: note = 33
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 31,		// NOTE ON: note = 31
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 36,		// NOTE ON: note = 36
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0x00 + 24,		// NOTE ON: note = 24
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0xFE,			// RETURN
//"Track Track 8"
0x00 + 0,		// NOTE ON: note = 0
0x9F + 4,		// DELAY: ticks = 4
0x00 + 33,		// NOTE ON: note = 33
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 31,		// NOTE ON: note = 31
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 36,		// NOTE ON: note = 36
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0x00 + 24,		// NOTE ON: note = 24
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0xFE,			// RETURN
//"Track Track 9"
0x00 + 0,		// NOTE ON: note = 0
0x9F + 4,		// DELAY: ticks = 4
0x00 + 36,		// NOTE ON: note = 36
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 3,		// DELAY: ticks = 3
0x00 + 24,		// NOTE ON: note = 24
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 7,		// DELAY: ticks = 7
0xFE,			// RETURN

};



#endif
