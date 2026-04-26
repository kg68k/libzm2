// libzm2const.h - libzm2: Z-MUSIC v2 function call library
// Copyright (C) 2026 TcbnErik

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.
//
// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

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
