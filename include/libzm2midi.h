// libzm2midi.h - libzm2: Z-MUSIC v2 function call library
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

#ifndef LIBZM2MIDI_H_INCLUDED
#define LIBZM2MIDI_H_INCLUDED

#include <stdint.h>

#include "libzm2internal.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Zm2MidiRecResult {
  uint32_t size;
  uint8_t* data;
};

typedef uint16_t Zm2Sc55Display[16];
typedef uint8_t Zm2M1MidiCh[8];
typedef uint8_t Zm2M1TrackParam[40];
typedef uint8_t Zm2M1EffectParam[25];

enum { ZM2_USE_PREVIOUS_DEVID = -1 };

static inline void  //
zm2_midi_rec(void) {
  zm2__func(0x16);
}

static inline struct Zm2MidiRecResult  //
zm2_midi_rec_end(uint32_t mode) {
  struct Zm2_TupleResult t = zm2__func_d2_tuple_wa(0x17, mode);
  return (struct Zm2MidiRecResult){t.d0, (uint8_t*)t.a0};
}

static inline zm2__nonnull int32_t  //
zm2_midi_trns(uint32_t size, const uint8_t* data) {
  return zm2__func_d2a1_wa(0x18, size, data);
}

static inline zm2__nonnull int32_t  //
zm2_send_rd_exc(uint32_t size, int8_t devid, uint8_t modelid,
                const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x1c, size, ((uint8_t)devid << 8) | modelid, data);
}

static inline zm2__nonnull int32_t  //
zm2_send_exc(uint32_t size, const uint8_t* data) {
  return zm2__func_d2a1_wa(0x1d, size, data);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_p_rsv(uint32_t size, int8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x1e, size, (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_reverb(uint32_t size, int8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x1f, size, (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_chorus(uint32_t size, int8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x20, size, (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_part_setup(uint32_t size, uint8_t part, int8_t devid,
                    const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x21, size, (part << 16) | (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_drum_setup(uint32_t size, uint8_t map, uint8_t note, int8_t devid,
                    const uint8_t* data) {
  uint32_t d3 = (((map << 8) | note) << 16) | (uint8_t)devid;
  return zm2__func_d2d3a1_wa(0x22, size, d3, data);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_print(uint32_t size, int8_t devid, const char* message) {
  return zm2__func_d2d3a1_wa(0x23, size, (uint8_t)devid, message);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_display(int8_t devid, const Zm2Sc55Display* pattern) {
  return zm2__func_d3a1(0x24, (uint8_t)devid, pattern);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_p_rsv(uint32_t size, int8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x25, size, (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_reverb(uint32_t size, int8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x26, size, (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_setup(uint32_t size, int8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x27, size, (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_drum(uint32_t size, uint8_t note, int8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x28, size, (note << 16) | (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_common(uint32_t size, uint8_t prog, int8_t devid,
                const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x29, size, (prog << 16) | (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_partial(uint32_t size, uint8_t prog, uint8_t part, int8_t devid,
                 const uint8_t* data) {
  uint32_t d3 = (((prog << 8) | part) << 16) | (uint8_t)devid;
  return zm2__func_d2d3a1_wa(0x2a, size, d3, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_patch(uint32_t size, uint8_t patch, int8_t devid,
               const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x2b, size, (patch << 16) | (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_print(uint32_t size, int8_t devid, const char* message) {
  return zm2__func_d2d3a1_wa(0x2c, size, (uint8_t)devid, message);
}

static inline zm2__nonnull int32_t  //
zm2_u220_setup(uint32_t size, int8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x2d, size, (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_u220_common(uint32_t size, int8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x2e, size, (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_u220_d_setup(uint32_t size, int8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x2f, size, (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_u220_p_setup(uint32_t size, uint8_t part, int8_t devid,
                 const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x30, size, (part << 16) | (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_u220_print(uint32_t size, int8_t devid, const char* message) {
  return zm2__func_d2d3a1_wa(0x31, size, (uint8_t)devid, message);
}

static inline zm2__nonnull int32_t  //
zm2_u220_timbre(uint32_t size, uint8_t prog, int8_t devid,
                const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x32, size, (prog << 16) | (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_u220_drum(uint32_t size, uint8_t note, int8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x33, size, (note << 16) | (uint8_t)devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_m1_midi_ch(const Zm2M1MidiCh* data) {
  return zm2__func_a1(0x34, data);
}

static inline int32_t  //
zm2_send_to_m1(int8_t devid) {
  return zm2__func_d3(0x35, (uint8_t)devid);
}

static inline zm2__nonnull int32_t  //
zm2_m1_p_setup(const Zm2M1TrackParam* data) {
  return zm2__func_a1(0x36, data);
}

static inline zm2__nonnull int32_t  //
zm2_m1_e_setup(const Zm2M1EffectParam* data) {
  return zm2__func_a1(0x37, data);
}

static inline zm2__nonnull int32_t  //
zm2_m1_print(uint32_t size, const char* message) {
  return zm2__func_d2a1_wa(0x38, size, message);
}

static inline int32_t  //
zm2_eox_wait(uint16_t wait) {
  return zm2__func_d2_wa(0x48, wait);
}

static inline int32_t  //
zm2_sc55_init(int8_t devid) {
  return zm2__func_d3(0x51, (uint8_t)devid);
}

static inline int32_t  //
zm2_mt32_init(int8_t devid) {
  return zm2__func_d3(0x52, (uint8_t)devid);
}

static inline int32_t  //
zm2_m_inp1(uint32_t wait) {
  return zm2__func_d2_wa(0x55, wait);
}

static inline int32_t  //
zm2_m_out1(uint8_t data) {
  return zm2__func_d2_wa(0x56, data);
}

#ifdef __cplusplus
}
#endif

#endif
