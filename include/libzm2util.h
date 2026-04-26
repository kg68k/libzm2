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

#include "libzm2.h"

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

#ifdef __cplusplus
}
#endif

#endif
