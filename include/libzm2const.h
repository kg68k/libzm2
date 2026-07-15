// libzm2const.h - libzm2: Z-MUSIC v2 function call library
// Copyright (C) 2026 TcbnErik

// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef LIBZM2CONST_H_INCLUDED
#define LIBZM2CONST_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

// sizeof(struct ...)
enum { ZM2_CNV_WK_SIZE = 140 };   // struct Zm2CnvWk
enum { ZM2_SEQ_WK_SIZE = 256 };   // struct Zm2SeqWk
enum { ZM2_BUF_INFO_SIZE = 76 };  // struct Zm2BufferInfo
enum { ZM2_STATUS_SIZE = 238 };   // struct Zm2Status

// Zm2Status::rs_midi
enum {
  ZM2_RSMIDI_RS232C = -1,
  ZM2_RSMIDI_CZ6BM1 = 0,
  ZM2_RSMIDI_POLYPHON = 1,
};

// Zm2Status::juke_mode
enum {
  ZM2_JUKEMODE_JUKEBOX_BIT = 0,
  ZM2_JUKEMODE_CONTROL_BIT = 1,
};

#define ZM2_ZMD_MIN_SIZE 12
#define ZM2_ZMD_ID "\x10ZmuSiC"
#define ZM2_ZMD_ID_LEN 7
#define ZM2_ZMD_VER_LEN 1

#define ZM2_ZPD_MIN_SIZE 10
#define ZM2_ZPD_ID "\x10ZmAdpCm"
#define ZM2_ZPD_ID_LEN 8

enum {
  ZM2_CHANNEL_MIN = 1,
  ZM2_CHANNEL_MAX = 32,
  ZM2_CHANNEL_COUNT = 32,
};

enum {
  ZM2_TRACK_MIN = 1,
  ZM2_TRACK_MAX = 80,
  ZM2_TRACK_COUNT = 80,
};

enum {
  ZM2_PCM_NOTE_MIN = 0,
  ZM2_PCM_NOTE_MAX = 511,
  ZM2_PCM_NOTE_COUNT = 512,
};

enum {
  ZM2_PANPOT_MUTE = 0,
  ZM2_PANPOT_LEFT = 1,
  ZM2_PANPOT_RIGHT = 2,
  ZM2_PANPOT_CENTER = 3,
};

enum {
  ZM2_PCM_3_9KHZ = 0,
  ZM2_PCM_5_2KHZ = 1,
  ZM2_PCM_7_8KHZ = 2,
  ZM2_PCM_10_4KHZ = 3,
  ZM2_PCM_15_6KHZ = 4,
  ZM2_PCM_15_6KHZ_16BIT = 5,  // PCM8
  ZM2_PCM_15_6KHZ_8BIT = 6,   // PCM8
};

enum {
  ZM2_PRIORITY_MIN = 0,
  ZM2_PRIORITY_MAX = 255,
};

enum {
  ZM2_TIMER_MODE_A = 0,
  ZM2_TIMER_MODE_B = 1,
};

enum {
  ZM2_MSTVOL_MIN = 0,
  ZM2_MSTVOL_MAX = 255,
};

enum {
  ZM2_WAVETYPE_ONESHOT = 0,
  ZM2_WAVETYPE_REPEAT = 1,
  ZM2_WAVETYPE_ALTERNATIVE = 1,
};

enum {
  ZM2_WAVENO_SAWTOOTH = 0,
  ZM2_WAVENO_SQUARE = 1,
  ZM2_WAVENO_TRIANGLE = 2,
  ZM2_WAVENO_SAWTOOTH_SINGLE = 3,

  ZM2_WAVENO_USER_MIN = 8,
  ZM2_WAVENO_USER_MAX = 31,
  ZM2_WAVENO_USER_COUNT = 24,
};

enum {
  ZM2_DRIVER_TYPE_UNIVERSAL = 0,
  ZM2_DRIVER_TYPE_16BIT = 1,
  ZM2_DRIVER_TYPE_RS_MIDI = 2,
  ZM2_DRIVER_TYPE_POLYPHON = 3,
};

#ifdef __cplusplus
}
#endif

#endif
