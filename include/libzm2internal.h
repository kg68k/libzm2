// libzm2internal.h - libzm2: Z-MUSIC v2 function call library
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

#ifndef LIBZM2INTERNAL_H_INCLUDED
#define LIBZM2INTERNAL_H_INCLUDED

#define LIBZM2_VERSION_MAJOR 1
#define LIBZM2_VERSION_MINOR 0
#define LIBZM2_VERSION_PATCH 0

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__GNUC__) && defined(__INTELLISENSE__)
// suppress syntax violation reports from IDE with non-GCC compilers.
#define zm2__reg(r)
#define zm2__asm(t, ...)
#else
#define zm2__reg(r) __asm(r)
#define zm2__asm __asm volatile
#endif

#if __GNUC__ < 3
#define zm2__rp(s) s
#define zm2__packed
#define zm2__nonnull
#else
#define zm2__rp(s) "%%" s
#define zm2__packed __attribute__((__packed__))
#define zm2__nonnull __attribute__((__nonnull__))
#endif

#ifndef ZM2_NO_WORKAROUND
#define ZM2__WORKAROUND
#endif

struct Zm2_TupleResult {
  int32_t d0;
  int32_t a0;
};

static const char* zm2__get_trap3_entry(void) {
  const uint32_t id1 = 0x5a6d7553;  // "ZmuS"
  const uint16_t id2 = 0x6943;      // "iC"

  const char* vec;
  zm2__asm("move.l (0x8c),%0" : "=r"(vec));  // trap #3 vector

  if (*(const uint32_t*)(vec - 8) == id1 &&
      *(const uint16_t*)(vec - 4) == id2) {
    uint16_t version = *(const uint16_t*)(vec - 2);
    if (version < 0x3000) return vec;
  }
  return nullptr;
}

static inline int32_t  //
zm2__func(uint32_t func) {
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm("trap #3" : "=d"(reg_d0) : "d"(reg_d1) : "a0");

  return reg_d0;
}

static inline struct Zm2_TupleResult  //
zm2__func_tuple(uint32_t func) {
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");
  register int32_t reg_a0 zm2__reg("a0");

  zm2__asm("trap #3" : "=d"(reg_d0), "=a"(reg_a0) : "d"(reg_d1));

  return (struct Zm2_TupleResult){reg_d0, reg_a0};
}

static inline int32_t  //
zm2__func_d2(uint32_t func, uint32_t d2) {
  register uint32_t reg_d2 zm2__reg("d2") = d2;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm("trap #3" : "=d"(reg_d0) : "d"(reg_d1), "d"(reg_d2) : "a0");

  return reg_d0;
}

#ifdef ZM2__WORKAROUND
static inline int32_t  //
zm2__func_d2_wa(uint32_t func, uint32_t d2) {
  register int32_t reg_d5 zm2__reg("d5") = -1;  // workaround
  register uint32_t reg_d2 zm2__reg("d2") = d2;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "trap #3" : "=d"(reg_d0) : "d"(reg_d1), "d"(reg_d2), "d"(reg_d5) : "a0");

  return reg_d0;
}
#else
#define zm2__func_d2_wa zm2__func_d2
#endif

static inline struct Zm2_TupleResult  //
zm2__func_d2_tuple(uint32_t func, uint32_t d2) {
  register int32_t reg_d2 zm2__reg("d2") = d2;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");
  register int32_t reg_a0 zm2__reg("a0");

  zm2__asm("trap #3" : "=d"(reg_d0), "=a"(reg_a0) : "d"(reg_d1), "d"(reg_d2));

  return (struct Zm2_TupleResult){reg_d0, reg_a0};
}

#ifdef ZM2__WORKAROUND
static inline struct Zm2_TupleResult  //
zm2__func_d2_tuple_wa(uint32_t func, uint32_t d2) {
  register int32_t reg_d5 zm2__reg("d5") = -1;  // workaround
  register int32_t reg_d2 zm2__reg("d2") = d2;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");
  register int32_t reg_a0 zm2__reg("a0");

  zm2__asm("trap #3" : "=d"(reg_d0), "=a"(reg_a0) : "d"(reg_d1), "d"(reg_d2),
           "d"(reg_d5));

  return (struct Zm2_TupleResult){reg_d0, reg_a0};
}
#else
#define zm2__func_d2_tuple_wa zm2__func_d2_tuple
#endif

static inline int32_t  //
zm2__func_d2d3(uint32_t func, uint32_t d2, uint32_t d3) {
  register uint32_t reg_d3 zm2__reg("d3") = d3;
  register uint32_t reg_d2 zm2__reg("d2") = d2;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "trap #3" : "=d"(reg_d0) : "d"(reg_d1), "d"(reg_d2), "d"(reg_d3) : "a0");

  return reg_d0;
}

static inline int32_t  //
zm2__func_d2d3a1(uint32_t func, uint32_t d2, uint32_t d3, const void* a1) {
  register const void* reg_a1 zm2__reg("a1") = a1;
  register uint32_t reg_d3 zm2__reg("d3") = d3;
  register uint32_t reg_d2 zm2__reg("d2") = d2;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "trap #3" : "=d"(reg_d0) : "d"(reg_d1), "d"(reg_d2), "d"(reg_d3),
      "a"(reg_a1) : "a0");

  return reg_d0;
}

#ifdef ZM2__WORKAROUND
static inline int32_t  //
zm2__func_d2d3a1_wa(uint32_t func, uint32_t d2, uint32_t d3, const void* a1) {
  register int32_t reg_d5 zm2__reg("d5") = -1;  // workaround
  register const void* reg_a1 zm2__reg("a1") = a1;
  register uint32_t reg_d3 zm2__reg("d3") = d3;
  register uint32_t reg_d2 zm2__reg("d2") = d2;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "trap #3" : "=d"(reg_d0) : "d"(reg_d1), "d"(reg_d2), "d"(reg_d3),
      "d"(reg_d5), "a"(reg_a1) : "a0");

  return reg_d0;
}
#else
#define zm2__func_d2d3a1_wa zm2__func_d2d3a1
#endif

static inline int32_t  //
zm2__func_d2a1(uint32_t func, uint32_t d2, const void* a1) {
  register const void* reg_a1 zm2__reg("a1") = a1;
  register uint32_t reg_d2 zm2__reg("d2") = d2;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "trap #3" : "=d"(reg_d0) : "d"(reg_d1), "d"(reg_d2), "a"(reg_a1) : "a0");

  return reg_d0;
}

#ifdef ZM2__WORKAROUND
static inline int32_t  //
zm2__func_d2a1_wa(uint32_t func, uint32_t d2, const void* a1) {
  register int32_t reg_d5 zm2__reg("d5") = -1;  // workaround
  register const void* reg_a1 zm2__reg("a1") = a1;
  register uint32_t reg_d2 zm2__reg("d2") = d2;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "trap #3" : "=d"(reg_d0) : "d"(reg_d1), "d"(reg_d2), "d"(reg_d5),
      "a"(reg_a1) : "a0");

  return reg_d0;
}
#else
#define zm2__func_d2a1_wa zm2__func_d2a1
#endif

static inline int32_t  //
zm2__func_d3(uint32_t func, uint32_t d3) {
#ifdef ZM2__WORKAROUND
  register int32_t reg_d5 zm2__reg("d5") = -1;  // workaround
#endif
  register uint32_t reg_d3 zm2__reg("d3") = d3;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "trap #3" : "=d"(reg_d0) :
#ifdef ZM2__WORKAROUND
      "d"(reg_d5),
#endif
      "d"(reg_d1), "d"(reg_d3) : "a0");

  return reg_d0;
}

static inline int32_t  //
zm2__func_d3a1(uint32_t func, uint32_t d3, const void* a1) {
#ifdef ZM2__WORKAROUND
  register int32_t reg_d5 zm2__reg("d5") = -1;  // workaround
#endif
  register const void* reg_a1 zm2__reg("a1") = a1;
  register uint32_t reg_d3 zm2__reg("d3") = d3;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "trap #3" : "=d"(reg_d0) : "d"(reg_d1), "d"(reg_d3),
#ifdef ZM2__WORKAROUND
      "d"(reg_d5),
#endif
      "a"(reg_a1) : "a0");

  return reg_d0;
}

static inline int32_t  //
zm2__func_a1(uint32_t func, const void* a1) {
  register const void* reg_a1 zm2__reg("a1") = a1;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm("trap #3" : "=d"(reg_d0) : "d"(reg_d1), "a"(reg_a1) : "a0");

  return reg_d0;
}

#ifdef ZM2__WORKAROUND
static inline int32_t  //
zm2__func_a1_wa(uint32_t func, const void* a1) {
  register int32_t reg_d5 zm2__reg("d5") = -1;  // workaround
  register const void* reg_a1 zm2__reg("a1") = a1;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "trap #3" : "=d"(reg_d0) : "d"(reg_d1), "d"(reg_d5), "a"(reg_a1) : "a0");

  return reg_d0;
}
#else
#define zm2__func_a1_wa zm2__func_a1
#endif

static inline int32_t  //
zm2__func_tracks(uint32_t func, uint32_t* tracks) {
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "movem.l (%2)," zm2__rp("d2-") zm2__rp("d4") "\n\t"
      "trap #3"
      : "=d"(reg_d0)
      : "d"(reg_d1), "a"(tracks)
      : "d2", "d3", "d4", "a0");

  return reg_d0;
}

static inline int32_t  //
zm2__func_tracks_all(uint32_t func) {
  register uint32_t reg_d2 zm2__reg("d2") = 0;
  register uint32_t reg_d3 zm2__reg("d3") = 0;
  register uint32_t reg_d4 zm2__reg("d4") = 0;
  register uint32_t reg_d1 zm2__reg("d1") = func;
  register int32_t reg_d0 zm2__reg("d0");

  zm2__asm(  //
      "trap #3" : "=d"(reg_d0) : "d"(reg_d1), "d"(reg_d2), "d"(reg_d3),
      "d"(reg_d4) : "a0");

  return reg_d0;
}

#ifdef __cplusplus
}
#endif

#endif
