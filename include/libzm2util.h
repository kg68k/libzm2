// libzm2util.h - libzm2: Z-MUSIC v2 function call library
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

#ifndef LIBZM2UTIL_H_INCLUDED
#define LIBZM2UTIL_H_INCLUDED

#include <stddef.h>
#include <stdint.h>

#include "libzm2.h"
#include "libzm2const.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline int  //
zm2_tracks_set(struct Zm2Tracks* tracks, uint32_t track) {
  uint32_t mask;
  uint32_t tr = track - 1;  // 1..80 -> 0..79
  if (tr >= 80) return -1;

  zm2__asm(
      "moveq.l #0,%0\n\t"
      "bchg %1,%0"  //
      : "=&d"(mask) : "d"(tr));

  tracks->d[tr / 32] |= mask;
  return 0;
}

static inline int  //
zm2_tracks_clear(struct Zm2Tracks* tracks, uint32_t track) {
  uint32_t clear_mask;
  uint32_t tr = track - 1;  // 1..80 -> 0..79
  if (tr >= 80) return -1;

  zm2__asm(
      "moveq.l #-1,%0\n\t"
      "bchg %1,%0"  //
      : "=&d"(clear_mask) : "d"(tr));

  tracks->d[tr / 32] &= clear_mask;
  return 0;
}

static inline int  //
zm2_tracks_isset(struct Zm2Tracks* tracks, uint32_t track, int* isset) {
  uint32_t mask;
  uint32_t tr = track - 1;  // 1..80 -> 0..79

  *isset = 0;
  if (tr >= 80) return -1;

  zm2__asm(
      "moveq.l #0,%0\n\t"
      "bchg %1,%0"  //
      : "=&d"(mask) : "d"(tr));

  if (tracks->d[tr / 32] &= mask) *isset = 1;
  return 0;
}

static inline int  //
zm2_is_zmd_data(const uint8_t* buf, size_t filesize) {
  const uint32_t id1 = 0x105a6d75;  // "\x10Zmu"
  const uint32_t id2 = 0x536943;    // "SiC"
  const uint32_t* p = (const uint32_t*)buf;

  if (filesize < ZM2_ZMD_MIN_SIZE) return 0;
  if (p[0] != id1 || (p[1] >> 8) != id2) return 0;
  return 1;
}

static inline int  //
zm2_is_zpd_data(const uint8_t* buf, size_t filesize) {
  const uint32_t id1 = 0x105a6d41;  // "\x10ZmA"
  const uint32_t id2 = 0x6470436d;  // "dpCm"
  const uint32_t* p = (const uint32_t*)buf;

  if (filesize < ZM2_ZPD_MIN_SIZE) return 0;
  if (p[0] != id1 || p[1] != id2) return 0;
  return 1;
}

static inline int32_t  //
zm2__get_zmd_strlen1(const uint8_t* buf, const uint8_t* tail) {
  const uint8_t* p = buf;
  while (p < tail) {
    if (*p++ == '\0') return (int32_t)(p - buf);
  }
  return -1;
}

static inline int32_t  //
zm2__get_zmd_word(const uint8_t* p, const uint8_t* tail) {
  if (tail < (p + 2)) return -1;
  return (p[0] << 8) + p[1];
}

static inline int32_t  //
zm2__get_zmd_cmncmdsize(const uint8_t* p, const uint8_t* tail) {
  const int cmd = 1;
  const int byte_data = 1;
  const int word_data = 2;
  const int long_data = 4;

  int n;
  switch (*p++) {
    default:
      break;
    case 0x04:  // (Vn,...)
    case 0x1b:  //
      return cmd + byte_data + 55;
    case 0x05:  // (On)
      return cmd + word_data;
    case 0x15:  // (Bn) (v1.09D and earlier)
      return cmd + byte_data;
    case 0x18:  // (X...) / .MIDI_DATA {...}
      n = zm2__get_zmd_word(p, tail);
      if (n < 0) break;
      return cmd + word_data + n;
    case 0x40:  // n=... / Onk=...
      p += 19;  // skip to filename or note number
      n = zm2__get_zmd_word(p, tail);
      if (n < 0) break;

      if ((n >> 8) == 0) {  // note number
        if (n != 0) break;
        return cmd + 19 + word_data + word_data;
      }
      // filename
      n = zm2__get_zmd_strlen1(p, tail);
      if (n < 0) break;
      return cmd + 19 + n;
    case 0x42:  // (Zn)
      return cmd + byte_data + long_data;
    case 0x4a:  // .WAVE_FORM ... {...}
      n = zm2__get_zmd_word(p, tail);
      if (n < 0) break;
      return cmd + word_data + byte_data + byte_data + word_data + n * 2;
    case 0x60:  // .ADPCM_LIST ...
    case 0x61:  // .PRINT ...
    case 0x62:  // .MIDI_DUMP=...
    case 0x63:  // .ADPCM_BLOCK_DATA ...
    case 0x7f:  // .COMMENT ...
      n = zm2__get_zmd_strlen1(p, tail);
      if (n < 0) break;
      return cmd + n;
    case 0x7e:  // dummy code
      return cmd;
  }
  return -1;
}

static inline int32_t  //
zm2_get_zmd_common_size(const uint8_t* buf, size_t filesize) {
  const uint8_t* p = buf;
  const uint8_t* tail = p + filesize;

  if (!zm2_is_zmd_data(p, filesize)) return -1;
  p += ZM2_ZMD_ID_LEN + ZM2_ZMD_VER_LEN;

  while (p && p < tail) {
    int32_t size;
    if (*p == 0xff) {  // end code
      p += 1;
      if (((uintptr_t)p & 1) != 0) {
        if (tail <= p || *p++ != 0xff) break;
      }
      return (int32_t)(p - buf);
    }
    size = zm2__get_zmd_cmncmdsize(p, tail);
    p += size;
    if (size <= 0) break;
  }
  return -1;
}

#ifdef __cplusplus
}
#endif

#endif
