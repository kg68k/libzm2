// libzm2work.h - libzm2: Z-MUSIC v2 function call library
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

#ifndef LIBZM2WORK_H_INCLUDED
#define LIBZM2WORK_H_INCLUDED

#if defined(__GNUC__) && (__GNUC__ < 3)
#error "GCC version 3 or later is required."
#endif

#include <stdint.h>

#include "libzm2internal.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Zm2CnvRep {
  uint8_t* data;
  int8_t is_written;
  uint8_t count;
} zm2__packed;

struct Zm2CnvWk {
  uint8_t* cv_data_adr;
  uint16_t cv_l_com;
  uint8_t cv_oct;
  int8_t cv_device;
  uint32_t cv_len;
  uint8_t cv_rep_cnt[8];
  int16_t cv_q_com;
  uint8_t cv_cnv_flg;
  uint8_t cv_velo_n;
  uint16_t cv_port_dly;
  uint16_t cv_bend_dly;
  uint8_t cv_ktrans;
  uint8_t cv_rltv_velo;
  uint8_t cv_rltv_vol;
  uint8_t cv_waon_dly;
  uint8_t cv_velo2;
  uint8_t cv_k_sig[7];
  struct Zm2CnvRep* cv_rep_start;
  uint16_t* cv_rep_exit[8];
  struct Zm2CnvRep cv_rep_addr[8];
  int8_t cv_velo[16];
} zm2__packed;

struct Zm2SeqWk {
  uint16_t p_on_count;
  uint16_t p_gate_time;
  uint8_t* p_data_pointer;
  uint8_t p_fo_spd;
  uint8_t p_ch;
  int8_t p_not_empty;
  uint8_t p_amod_step;
  union {
    uint16_t p_mstep_tbl;
    uint16_t* p_wvpm_loop;
  } zm2__packed;
  uint16_t p_wvpm_lpmd;
  uint8_t p_altp_flg;
  char reserved_13[9];
  uint8_t p_fo_mode;
  uint8_t p_pgm;
  uint8_t p_pan;
  uint8_t p_vol;
  union {
    uint8_t p_mrvs_tbl;
    uint16_t* p_wvpm_point;
  } zm2__packed;
  uint16_t* p_wvpm_end;
  union {
    int16_t p_sp_tie;
    struct {
      uint8_t p_om;
      uint8_t p_sync;
    } zm2__packed;
  } zm2__packed;
  uint8_t p_af;
  int8_t p_se_mode;
  uint16_t p_pmod_tbl[8];
  uint32_t p_total;
  uint8_t p_fo_lvl;
  char reserved_41;
  uint8_t p_note[8];
  uint8_t p_extra_ch;
  uint8_t p_aftc_n;
  int16_t p_bend_rng_f;
  int16_t p_bend_rng_m;
  int16_t p_detune_f;
  int16_t p_detune_m;
  uint16_t p_port_dly;
  uint16_t p_bend_dly;
  uint8_t p_port_work;
  uint8_t p_port_rvs;
  uint16_t p_port_work2;
  union {
    uint8_t p_amod_tbl[8];
    uint8_t p_arcc_tbl[8];
  } zm2__packed;
  union {
    uint8_t p_arvs_tbl[8];
    struct {
      uint16_t* p_wvam_point;
      uint16_t* p_wvam_end;
    } zm2__packed;
  } zm2__packed;
  uint16_t p_pmod_work4;
  int16_t p_port_flg;
  int16_t p_bend_flg;
  uint8_t p_aftc_tbl[8];
  uint16_t p_aftc_dly;
  uint16_t p_aftc_work;
  union {
    uint8_t p_astep_tbl[8];
    struct {
      uint16_t* p_wvam_loop;
      uint16_t p_wvam_lpmd;
      uint8_t p_alta_flg;
    } zm2__packed;
  } zm2__packed;
  uint16_t p_pmod_step2;
  uint16_t p_pmod_work;
  uint16_t p_pmod_work2;
  uint8_t p_pmod_work3;
  uint8_t p_pmod_n;
  uint8_t p_sync_wk;
  uint8_t p_rpt_last;
  uint8_t p_at_b_range;
  uint8_t p_arcc;
  int16_t p_pmod_flg;
  int8_t p_pmod_sw;
  union {
    int8_t p_amod_sw;
    uint8_t p_arcc_sw;
  } zm2__packed;
  uint8_t p_bend_sw;
  int8_t p_aftc_flg;
  uint8_t p_md_flg;
  int8_t p_waon_flg;
  uint16_t p_pmod_dly;
  union {
    uint16_t p_amod_dly;
    uint16_t p_arcc_dly;
  } zm2__packed;
  int16_t p_port_step;
  union {
    struct {
      uint8_t p_bank_msb;
      uint8_t p_bank_lsb;
      uint8_t p_effect1;
      uint8_t p_effect3;
      int8_t p_d6_last;
    } zm2__packed;
    struct {
      uint8_t p_ol1;
      uint8_t p_ol2;
      uint8_t p_ol3;
      uint8_t p_ol4;
      uint8_t p_cf;
    } zm2__packed;
  } zm2__packed;
  uint8_t p_amod_step2;
  uint8_t p_pb_add;
  int8_t p_vset_flg;
  int8_t p_arcc_rst;
  uint8_t p_arcc_def;
  uint8_t* p_coda_ptr;
  uint8_t* p_pointer;
  uint8_t* p_do_loop_ptr;
  uint16_t p_pmod_work5;
  uint16_t p_pmod_work6;
  union {
    int8_t p_amod_flg;
    int8_t p_arcc_flg;
  } zm2__packed;
  int8_t p_aftc_sw;
  int8_t p_dumper;
  int8_t p_tie_flg;
  uint16_t p_pmod_dpt;
  uint8_t p_seq_flag;
  uint8_t p_do_loop_flag;
  uint16_t p_pmod_spd;
  uint16_t p_amod_spd;
  uint32_t p_total_olp;
  uint16_t p_pmod_step;
  int8_t p_tie_pmod;
  int8_t p_tie_bend;
  union {
    int8_t p_tie_amod;
    int8_t p_tie_arcc;
  };
  int8_t p_tie_aftc;
  uint8_t p_pan2;
  int8_t p_non_off;
  uint8_t p_frq;
  uint8_t p_velo;
  uint16_t p_amod_work4;
  uint8_t p_pmod_rvs;
  uint8_t p_waon_dly;
  uint8_t p_waon_work;
  union {
    int8_t p_waon_num;
    uint8_t p_note_last;
  } zm2__packed;
  uint8_t p_rpt_cnt[8];
  uint8_t p_maker;
  uint8_t p_device;
  uint8_t p_module;
  uint8_t p_last_aft;
  union {
    struct {
      uint16_t p_amod_work;
      uint8_t p_amod_work2;
      uint8_t p_amod_work3;
      uint8_t p_amod_work7;
      uint8_t p_amod_n;
      uint16_t p_amod_work5;
      uint16_t p_amod_work6;
    } zm2__packed;
    struct {
      uint16_t p_arcc_work;
      uint8_t p_arcc_work2;
      char reserved_e9[2];
      uint8_t p_arcc_n;
      uint16_t p_arcc_work5;
      uint16_t p_arcc_work6;
    } zm2__packed;
  } zm2__packed;
  int8_t p_pmod_wf;
  uint8_t p_amod_dpt;
  int8_t p_amod_wf;
  uint8_t p_dmp_n;
  uint8_t p_pmod_omt;
  union {
    uint8_t p_arcc_omt;
    uint8_t p_amod_omt;
  } zm2__packed;
  int8_t p_pmod_mode;
  int8_t p_arcc_mode;
  int8_t p_pmod_chain;
  int8_t p_amod_chain;
  int8_t p_velo_dmy;
  uint8_t p_waon_mark;
  int16_t p_marker;
  uint8_t p_amod_rvs;
  union {
    int8_t p_ne_buff;
    uint8_t p_user;
  };
} zm2__packed;

struct Zm2BufferInfo {
  void* trk_top;
  uint32_t trk_buf_size;
  union {
    void* trk_buf_end;
    void* dev_end_adr;
  } zm2__packed;
  void* adpcm_buffer_top;
  uint32_t adpcm_buffer_size;
  void* adpcm_buffer_end;
  void* adpcm_work_top;
  uint32_t adpcm_work_size;
  void* adpcm_work_end;
  void* adpcm_buffer_next;
  void* adpcm_work_now;
  uint32_t adpcm_work_true_size;
  struct Zm2SeqWk* seq_wk_tbl;
  struct Zm2SeqWk* seq_wk_tbl2;
  void* adpcm_tbl;
  void* wave_tbl;
  void* neiro;
  void* trk_po_tbl;
  void* trk_len_tbl;
} zm2__packed;

struct Zm2Status {
  uint8_t sc55_id;
  uint8_t mt32_id;
  uint8_t u220_id;
  uint8_t m1_id;
  uint8_t loop_chk;
  int8_t ps_flg;
  uint8_t frq;
  uint8_t y3;
  uint8_t noise_mode;
  char first_cmt[97];
  uint8_t pcm8_ch;
  int8_t ch_tr_msk;
  int8_t ch_tr_opl;
  uint8_t mclk;
  int8_t se_mode;
  int8_t cmd_or_dev;
  int8_t timer_a_mode;
  int8_t synchro_mode;
  int8_t mfp_mode;
  int8_t no_init_mode;
  int8_t trace_mode;
  char reserved_11;
  uint16_t m_tmp_buf;
  uint16_t timer_value;
  uint32_t zmusic_int;
  uint8_t err_code[8];
  int8_t midi_board;
  int8_t rs_midi;
  int8_t emulate_mode;
  int8_t pcm8_flg;
  int16_t poly_ch;
  uint8_t juke_mode;
  int8_t timer_flg;
  char last_fn[91];
  char reserved_99;
  uint32_t date_buf;
} zm2__packed;

#ifdef __cplusplus
}
#endif

#endif
