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
zm2_send_rd_exc(uint32_t size, uint16_t devid, uint16_t modelid,
                const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x1c, size, (devid << 16) + modelid, data);
}

static inline zm2__nonnull int32_t  //
zm2_send_exc(uint32_t size, const uint8_t* data) {
  return zm2__func_d2a1_wa(0x1d, size, data);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_p_rsv(uint32_t size, uint8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x1e, size, devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_reverb(uint32_t size, uint8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x1f, size, devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_chorus(uint32_t size, uint8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x20, size, devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_part_setup(uint32_t size, uint16_t part, uint16_t devid,
                    const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x21, size, (part << 16) + devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_drum_setup(uint32_t size, uint8_t map, uint8_t note, uint16_t devid,
                    const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x22, size, (map << 24) + (note << 16) + devid,
                             data);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_print(uint32_t size, uint8_t devid, const char* message) {
  return zm2__func_d2d3a1_wa(0x23, size, devid, message);
}

static inline zm2__nonnull int32_t  //
zm2_sc55_display(uint8_t devid, const Zm2Sc55Display* pattern) {
  return zm2__func_d3a1(0x24, devid, pattern);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_p_rsv(uint32_t size, uint8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x25, size, devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_reverb(uint32_t size, uint8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x26, size, devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_setup(uint32_t size, uint8_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x27, size, devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_drum(uint32_t size, uint16_t note, uint16_t devid,
              const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x28, size, (note << 16) + devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_common(uint32_t size, uint16_t prog, uint16_t devid,
                const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x29, size, (prog << 16) + devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_partial(uint32_t size, uint8_t prog, uint8_t part, uint16_t devid,
                 const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x2a, size, (prog << 24) + (part << 16) + devid,
                             data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_patch(uint32_t size, uint16_t patch, uint16_t devid,
               const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x2b, size, (patch << 16) + devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_mt32_print(uint32_t size, uint8_t devid, const char* message) {
  return zm2__func_d2d3a1_wa(0x2c, size, devid, message);
}

static inline zm2__nonnull int32_t  //
zm2_u220_setup(uint32_t size, uint16_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x2d, size, devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_u220_common(uint32_t size, uint16_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x2e, size, devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_u220_d_setup(uint32_t size, uint16_t devid, const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x2f, size, devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_u220_p_setup(uint32_t size, uint16_t part, uint16_t devid,
                 const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x30, size, (part << 16) + devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_u220_print(uint32_t size, uint8_t devid, const char* message) {
  return zm2__func_d2d3a1_wa(0x31, size, devid, message);
}

static inline zm2__nonnull int32_t  //
zm2_u220_timbre(uint32_t size, uint16_t prog, uint16_t devid,
                const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x32, size, (prog << 16) + devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_u220_drum(uint32_t size, uint16_t note, uint16_t devid,
              const uint8_t* data) {
  return zm2__func_d2d3a1_wa(0x33, size, (note << 16) + devid, data);
}

static inline zm2__nonnull int32_t  //
zm2_m1_midi_ch(const Zm2M1MidiCh* data) {
  return zm2__func_a1(0x34, data);
}

static inline int32_t  //
zm2_send_to_m1(uint8_t devid) {
  return zm2__func_d3(0x35, devid);
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
  return zm2__func_d3(0x51, devid);
}

static inline int32_t  //
zm2_mt32_init(int8_t devid) {
  return zm2__func_d3(0x52, devid);
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
