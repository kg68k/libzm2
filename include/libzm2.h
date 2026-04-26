// libzm2.h - libzm2: Z-MUSIC v2 function call library
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

#ifndef LIBZM2_H_INCLUDED
#define LIBZM2_H_INCLUDED

#include <stdint.h>

#include "libzm2internal.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Zm2Tracks {
  uint32_t d[3];  // [0] = tr32..1, [1] = tr64..33, [2] = tr80..65
};

struct Zm2DriverSpec {
  int32_t version;
  int16_t type;
  int16_t is_tiny;
};

typedef uint8_t Zm2FmTone[55];

struct Zm2AdpcmConf {
  uint32_t note;         // d2.l
  uint16_t pitch;        // d3.hw
  uint16_t volume;       // d3.lw
  uint16_t delay;        // d4.hw
  int16_t mix_note;      // d4.lw
  uint16_t cut_offset;   // d5.hw
  uint16_t cut_size;     // d5.lw
  uint32_t reverse;      // d6.l
  uint16_t fade_offset;  // d7.hw
  int8_t fade_mode;      // d7.hb
  uint8_t fade_level;    // d7.lb
} zm2__packed;

struct Zm2TempoResult {
  uint32_t tempo;
  uint16_t timer;
};

typedef uint8_t Zm2RealChTbl[32];

struct Zm2TrkTblResult {
  const Zm2RealChTbl* real_ch_tbl;
  const uint8_t* play_trk_tbl;
};

struct Zm2PlayWorkResult {
  const struct Zm2CnvWk* cnv_wk;
  const struct Zm2SeqWk* seq_wk;
};

struct Zm2OccupiedSize {
  uint32_t track_usage;
  uint32_t adpcm_usage;
  uint32_t work_usage;
} zm2__packed;

static inline int32_t  //
zm2_get_version_super(void) {
  const char* vec = zm2__get_trap3_entry();

  if (vec) {
    uint16_t version = *(const uint16_t*)(vec - 2);
    return (int32_t)version;
  }
  return -1;
}

static inline int32_t  //
zm2_get_version(void) {
  register void* reg_a0 zm2__reg("a0") = zm2_get_version_super;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(
      "pea (%1)\n\t"
      ".dc 0xfff6\n\t"            // _SUPER_JSR
      "addq.l #4," zm2__rp("sp")  //
      : "=d"(reg_d0),
      "=a"(reg_a0) : "a"(reg_a0) : "d1", "d2", "a1", "a2");

  return reg_d0;
}

static inline struct Zm2DriverSpec  //
zm2_get_spec_super(void) {
  const uint32_t tiny_id = 0x54494e59;  // "TINY"
  const char* vec = zm2__get_trap3_entry();

  if (vec) {
    uint16_t version = *(const uint16_t*)(vec - 2);
    int32_t ver = version & 0xff0f;
    int16_t type = (version >> 4) & 0xf;
    int16_t is_tiny = *(const uint32_t*)(vec - 12) == tiny_id;
    return (struct Zm2DriverSpec){ver, type, is_tiny};
  }
  return (struct Zm2DriverSpec){-1, -1, -1};
}

static inline struct Zm2DriverSpec  //
zm2_get_spec(void) {
  register void* reg_a0 zm2__reg("a0") = zm2_get_spec_super;
  register struct Zm2DriverSpec reg_d0d1 zm2__reg("d0");

  zm2__asm(
      "pea (%2)\n\t"
      ".dc 0xfff6\n\t"            // _SUPER_JSR
      "addq.l #4," zm2__rp("sp")  //
      : "=d"(reg_d0d1),
      "=a"(reg_a0) : "a"(reg_a0) : "d2", "a1", "a2");

  return reg_d0d1;
}

static inline void  //
zm2_m_init(void) {
  zm2__func(0x00);
}

static inline int32_t  //
zm2_m_alloc(uint16_t track, uint16_t size) {
  return zm2__func_d2_wa(0x01, (track << 16) | size);
}

static inline int32_t  //
zm2_m_assign(uint16_t channel, uint16_t track) {
  return zm2__func_d2_wa(0x02, (channel << 16) | track);
}

static inline zm2__nonnull int32_t  //
zm2_m_vget(uint32_t no, Zm2FmTone* buf) {
#ifdef ZM2__WORKAROUND
  register int32_t reg_d5 zm2__reg("d5") = -1;
#endif
  register Zm2FmTone* reg_a1 zm2__reg("a1") = buf;
  register uint32_t reg_d2 zm2__reg("d2") = no;
  register uint32_t reg_d1 zm2__reg("d1") = 0x03;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "trap #3" : "=d"(reg_d0), "=m"(*buf) : "d"(reg_d1), "d"(reg_d2),
#ifdef ZM2__WORKAROUND
      "d"(reg_d5),
#endif
      "a"(reg_a1) : "a0");

  return reg_d0;
}

static inline zm2__nonnull int32_t  //
zm2_m_vset(uint32_t tone, const Zm2FmTone* data) {
  return zm2__func_d2a1_wa(0x04, tone, data);
}

static inline struct Zm2TempoResult  //
zm2_m_tempo_get(void) {
  struct Zm2_TupleResult t = zm2__func_d2_tuple(0x05, (uint32_t)-1);
  return (struct Zm2TempoResult){t.d0, (uint16_t)t.a0};
}

static inline int32_t  //
zm2_m_tempo_set(uint32_t tempo) {
  return zm2__func_d2(0x05, tempo);
}

static inline zm2__nonnull int32_t  //
zm2_m_trk(uint32_t track, const char* mml) {
  return zm2__func_d2a1_wa(0x06, track, mml);
}

static inline int32_t  //
zm2_m_free(uint32_t track) {
  return zm2__func_d2_wa(0x07, track);
}

static inline zm2__nonnull int32_t  //
zm2_m_play(struct Zm2Tracks* tracks) {
  return zm2__func_tracks(0x08, tracks->d);
}

static inline int32_t  //
zm2_m_play_all(void) {
  return zm2__func_tracks_all(0x08);
}

static inline int32_t  //
zm2_m_stat(uint32_t channels) {
  return zm2__func_d2_wa(0x09, channels);
}

static inline zm2__nonnull int32_t  //
zm2_m_stop(struct Zm2Tracks* tracks) {
  return zm2__func_tracks(0x0a, tracks->d);
}

static inline int32_t  //
zm2_m_stop_all(void) {
  return zm2__func_tracks_all(0x0a);
}

static inline zm2__nonnull int32_t  //
zm2_m_cont(struct Zm2Tracks* tracks) {
  return zm2__func_tracks(0x0b, tracks->d);
}

static inline int32_t  //
zm2_m_cont_all(void) {
  return zm2__func_tracks_all(0x0b);
}

static inline void*  //
zm2_m_atoi(uint32_t track) {
  return (void*)zm2__func_d2_wa(0x0c, track);
}

static inline void  //
zm2_init_all(void) {
  zm2__func(0x0d);
}

static inline void  //
zm2_int_stop(void) {
  zm2__func(0x0e);
}

static inline int32_t  //
zm2_m_play2(void) {
  return zm2__func(0x0f);
}

static inline zm2__nonnull int32_t  //
zm2_adpcm_read(const struct Zm2AdpcmConf* conf, const char* filename) {
  register const char* reg_a1 zm2__reg("a1") = filename;
  register uint32_t reg_d1 zm2__reg("d1") = 0x10;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "movem.l (%3)," zm2__rp("d2-") zm2__rp("d7") "\n\t"
      "trap #3"
      : "=d"(reg_d0)
      : "d"(reg_d1), "a"(reg_a1), "a"(conf)
      : "d2", "d3", "d4", "d5", "d6", "d7", "a0");

  return reg_d0;
}

static inline zm2__nonnull int32_t  //
zm2_play_cnv_data(uint32_t size, const uint8_t* zmd) {
  return zm2__func_d2a1_wa(0x11, size, zmd);
}

static inline zm2__nonnull int32_t  //
zm2_play_cnv_data_fast(const uint8_t* zmd) {
  return zm2__func_d2a1_wa(0x11, 0, zmd);
}

static inline zm2__nonnull void  //
zm2_se_play(uint32_t track, const uint8_t* zmd) {
  zm2__func_d2a1(0x12, track, zmd);
}

static inline zm2__nonnull void  //
zm2_se_adpcm1(uint32_t size, uint8_t priority, uint8_t freq, uint8_t pan,
              const uint8_t* pcm) {
  zm2__func_d2d3a1(0x13, size, (priority << 16) | (freq << 8) | pan, pcm);
}

static inline void  //
zm2_se_adpcm2(uint32_t note, uint8_t priority, uint8_t freq, uint8_t pan) {
  zm2__func_d2d3(0x14, note, (priority << 16) | (freq << 8) | pan);
}

static inline void  //
zm2_set_ch_mode(uint32_t mode) {
  zm2__func_d2(0x15, mode);
}

static inline int32_t  //
zm2_calc_total(uint32_t mode) {
  return zm2__func_d2(0x19, mode);
}

static inline int32_t  //
zm2_fade_out(int32_t speed) {
  return zm2__func_d2(0x1a, speed);
}

static inline zm2__nonnull int32_t  //
zm2_m_vset2(uint32_t tone, const Zm2FmTone* data) {
  return zm2__func_d2a1_wa(0x1b, tone, data);
}

static inline zm2__nonnull int32_t  //
zm2_adpcm_block_data(const char* filename) {
  return zm2__func_a1(0x39, filename);
}

static inline struct Zm2TrkTblResult  //
zm2_get_trk_tbl(void) {
  struct Zm2_TupleResult t = zm2__func_tuple(0x3a);
  return (struct Zm2TrkTblResult){(const Zm2RealChTbl*)t.d0, (uint8_t*)t.a0};
}

static inline zm2__nonnull void  //
zm2_loop_time_set(uint8_t count, void* address) {
  zm2__func_d2a1(0x3b, count, address);
}

static inline void  //
zm2_loop_time_cancel(void) {
  zm2__func_d2a1(0x3b, 0, 0);
}

static inline struct Zm2PlayWorkResult  //
zm2_get_play_work(uint8_t track) {
  struct Zm2_TupleResult t = zm2__func_d2_tuple(0x3c, track);
  return (struct Zm2PlayWorkResult)  //
      {(const struct Zm2CnvWk*)t.d0, (const struct Zm2SeqWk*)t.a0};
}

static inline int32_t  //
zm2_get_timer_mode(void) {
  return zm2__func(0x3d);
}

static inline int32_t  //
zm2_set_fm_master_vol(uint8_t volume) {
  return zm2__func_d2(0x3e, volume);
}

static inline int32_t  //
zm2_set_timer_value(uint32_t timer) {
  return zm2__func_d2(0x3f, timer);
}

static inline zm2__nonnull int32_t  //
zm2_release_support_set(const char* filename) {
  return zm2__func_a1(0x40, filename);
}

static inline int32_t  //
zm2_release_support_cancel(int32_t code) {
  return zm2__func_d2a1_wa(0x40, (uint32_t)code, 0);
}

static inline void  //
zm2_jump_active(uint8_t mode) {
  zm2__func_d2(0x41, mode);
}

static inline int32_t  //
zm2_set_mclk(uint32_t count) {
  return zm2__func_d2_wa(0x42, count);
}

static inline int32_t  //
zm2_picture_sync(uint32_t mode) {
  return zm2__func_d2(0x43, mode);
}

static inline int32_t  //
zm2_mask_channels(uint32_t channels) {
  return zm2__func_d2(0x44, channels);
}

static inline struct Zm2BufferInfo*  //
zm2_buffer_info(void) {
  return (struct Zm2BufferInfo*)zm2__func_tuple(0x45).a0;
}

static inline zm2__nonnull int32_t  //
zm2_set_zpd_tbl(const void* zpd) {
  return zm2__func_a1_wa(0x46, zpd);
}

static inline int32_t  //
zm2_set_output_level(uint32_t channels, int8_t level) {
  return zm2__func_d2d3(0x47, channels, level);
}

static inline zm2__nonnull int32_t  //
zm2_set_wave_form1(uint32_t size, uint8_t no, uint8_t type, uint16_t point,
                   const uint8_t* wave) {
  return zm2__func_d2d3a1_wa(0x49, size, (no << 24) | (type << 16) | point,
                             wave);
}

static inline zm2__nonnull int32_t  //
zm2_set_wave_form2(uint32_t size, uint8_t no, uint8_t type, uint16_t point,
                   const uint8_t* wave) {
  return zm2__func_d2d3a1_wa(0x4a, size, (no << 24) | (type << 16) | point,
                             wave);
}

static inline void  //
zm2_mask_tracks(uint32_t track) {
  zm2__func_d2_wa(0x4b, track);
}

static inline void  //
zm2_set_output_level2(uint32_t track, int8_t level) {
  zm2__func_d2d3(0x4c, track, level);
}

static inline int32_t  //
zm2_get_loop_time(void) {
  return zm2__func(0x4d);
}

static inline const char*  //
zm2_get_1st_comment(void) {
  return (const char*)zm2__func_tuple(0x4e).a0;
}

static inline void  //
zm2_int_start(void) {
  zm2__func(0x4f);
}

static inline void*  //
zm2_zm_status(void) {
  return (void*)zm2__func_tuple(0x50).a0;
}

static inline struct Zm2Status*  //
zm2_get_status(void) {
  return (struct Zm2Status*)(zm2__func_tuple(0x50).a0 - 0x86);
}

static inline void  //
zm2_relative_uv(uint32_t mode) {
  zm2__func_d2(0x53, mode);
}

static inline void  //
zm2_intercept_play(int32_t mode) {
  zm2__func_d2(0x54, (uint32_t)mode);
}

static inline zm2__nonnull void  //
zm2_occupied_size(struct Zm2OccupiedSize* buf) {
  zm2__func_a1_wa(0x57, buf);
}

static inline zm2__nonnull void  //
zm2_call_int_play_ope(struct Zm2SeqWk* seq_wk) {
  register struct Zm2SeqWk* reg_a5 zm2__reg("a5") = seq_wk;
  register uint32_t reg_d1 zm2__reg("d1") = 0x58;

  zm2__asm("trap #3" : : "d"(reg_d1), "a"(reg_a5) : "d0", "a0");
}

#ifdef __cplusplus
}
#endif

#endif
