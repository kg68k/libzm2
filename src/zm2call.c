// zm2call.c - libzm2 example
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
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#define PROGRAM "zm2call"
#define VERSION "0.1.0"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef __human68k__
#ifdef __LIBC__
#include <sys/dos.h>
#define _dos_malloc0(md, size, mep) _dos_malloc0(md, size, (struct _psp*)mep)
#else
#include <x68k/dos.h>
#define _mep dos_mep
#define _dos_malloc0(md, size, mep) _dos_malloc0(md, size, (int)mep)
#endif
#else
struct _mep {
  void* prev_mp;
  void* parent_mp;
  void* block_end;
  void* next_mp;
};
int _dos_super(int stack);
void* _dos_malloc0(int md, int size, struct _mep* mep);
int _dos_mfree(void* memptr);
#endif

// Cランタイムが持つ変数で、自分自身のプロセスのメモリ管理ポインタ+0x100を示す
extern void* _PSTA;

#include "compat.h"
#include "libzm2.h"
#include "libzm2const.h"
#include "libzm2midi.h"

typedef struct {
  const char* name;
  int (*func)(char** args);
  const char* usage;
} Command;

static const Command* GetCommand(const char* name);

static int IsNoCheckCommand(const Command* cmd) { return cmd->name[0] == '*'; }

static const char* GetCommandName(const Command* cmd) {
  return IsNoCheckCommand(cmd) ? cmd->name + 1 : cmd->name;
}

static int StrToInt32(const char* str, int32_t* out, const char** endptr) {
  long val;
  const char* dummy;
  const char** ep = endptr ? endptr : &dummy;

  *out = 0;
  *ep = str;

  if (!str || *str == '\0') {
    fprintf(stderr, "数値が指定されていません。\n");
    return -1;
  }

  errno = 0;
  val = strtol(str, (char**)ep, 0);

  if (errno != 0 || (**ep != '\0' && **ep != ',')) {
    fprintf(stderr, "数値に変換できません。\n");
    return -1;
  }
  if (**ep == ',') {
    if (!endptr) {
      fprintf(stderr, "複数の数値は指定できません。\n");
      return -1;
    }
    *ep += 1;  // ','は飛ばしておく
  }

  *out = (int32_t)val;
  return 0;
}

static int StrToInt16(const char* str, int16_t* out, const char** endptr) {
  int32_t val;

  if (StrToInt32(str, &val, endptr) < 0) return -1;
  if (val < INT16_MIN || INT16_MAX < val) {
    fprintf(stderr, "数値が範囲外です。\n");
    return -1;
  }

  *out = (int16_t)val;
  return 0;
}

static int StrToInt8(const char* str, int8_t* out, const char** endptr) {
  int32_t val;

  if (StrToInt32(str, &val, endptr) < 0) return -1;
  if (val < INT8_MIN || INT8_MAX < val) {
    fprintf(stderr, "数値が範囲外です。\n");
    return -1;
  }

  *out = (int8_t)val;
  return 0;
}

static int StrToUint32(const char* str, uint32_t* out, const char** endptr) {
  unsigned long val;
  const char* dummy;
  const char** ep = endptr ? endptr : &dummy;

  *out = 0;
  *ep = str;

  if (!str || *str == '\0') {
    fprintf(stderr, "数値が指定されていません。\n");
    return -1;
  }

  errno = 0;
  val = strtoul(str, (char**)ep, 0);

  if (errno != 0 || (**ep != '\0' && **ep != ',')) {
    fprintf(stderr, "数値に変換できません。\n");
    return -1;
  }
  if (**ep == ',') {
    if (!endptr) {
      fprintf(stderr, "複数の数値は指定できません。\n");
      return -1;
    }
    *ep += 1;  // ','は飛ばしておく
  }

  *out = (uint32_t)val;
  return 0;
}

static int StrToUint16(const char* str, uint16_t* out, const char** endptr) {
  uint32_t val;

  if (StrToUint32(str, &val, endptr) < 0) return -1;
  if (val > UINT16_MAX) {
    fprintf(stderr, "数値が大きすぎます。\n");
    return -1;
  }

  *out = (uint16_t)val;
  return 0;
}

static int StrToUint8(const char* str, uint8_t* out, const char** endptr) {
  uint32_t val;

  if (StrToUint32(str, &val, endptr) < 0) return -1;
  if (val > UINT8_MAX) {
    fprintf(stderr, "数値が大きすぎます。\n");
    return -1;
  }

  *out = (uint8_t)val;
  return 0;
}

static int ParseTrackList(const char* str, struct Zm2Tracks* out) {
  *out = (struct Zm2Tracks){{0}};

  if (!str || *str == '\0') {
    fprintf(stderr, "トラック番号が指定されていません。\n");
    return -1;
  }

  do {
    const char* endptr;
    uint32_t track;

    if (StrToUint32(str, &track, &endptr) < 0) return -1;

    if (zm2_tracks_set(out, track) < 0) {
      fprintf(stderr, "トラック番号が正しくありません。\n");
      return -1;
    }

    str = endptr;
  } while (*str);

  return 0;
}

static int ParseChannelList(const char* str, uint32_t* out) {
  *out = 0;

  if (!str || *str == '\0') {
    fprintf(stderr, "チャンネル番号が指定されていません。\n");
    return -1;
  }

  do {
    const char* endptr;
    uint32_t channel;

    if (StrToUint32(str, &channel, &endptr) < 0) return -1;

    if (channel < ZM2_CHANNEL_MIN || ZM2_CHANNEL_MAX < channel) {
      fprintf(stderr, "チャンネル番号が正しくありません。\n");
      return -1;
    }
    *out |= (1U << (channel - 1));

    str = endptr;
  } while (*str);

  return 0;
}

static void PrintResult(int32_t d) { printf("%d\n", (int)d); }

static void PrintResultHex(uint32_t x) { printf("0x%08x\n", (unsigned int)x); }

static void PrintResultAddress(const void* p) { PrintResultHex((uint32_t)p); }

int PrintZmusicVersion(UNUSED char** args) {
  int32_t version = zm2_get_version();

  if (version < 0) {
    printf("Z-MUSIC v2は常駐していません。\n");
  } else {
    printf("version: 0x%04x\n", (unsigned int)version);
  }

  return EXIT_SUCCESS;
}

static const char* GetSpecTypeString(int type) {
  if (type == ZM2_DRIVER_TYPE_UNIVERSAL) return "UNIVERSAL";
  if (type == ZM2_DRIVER_TYPE_16BIT) return "16bit";
  if (type == ZM2_DRIVER_TYPE_RS_MIDI) return "RS-MIDI";
  if (type == ZM2_DRIVER_TYPE_POLYPHON) return "POLYPHON";
  return "???";
}

int PrintZmusicSpec(UNUSED char** args) {
  struct Zm2DriverSpec spec = zm2_get_spec();

  if (spec.version < 0) {
    printf("Z-MUSIC v2は常駐していません。\n");
  } else {
    printf("version: 0x%04x\n", (unsigned int)spec.version);
    printf("type: %d (%s)\n", (int)spec.type, GetSpecTypeString(spec.type));
    printf("is_tiny: %d\n", (int)spec.is_tiny);
  }
  return EXIT_SUCCESS;
}

int MInit(UNUSED char** args) {
  zm2_m_init();
  return EXIT_SUCCESS;
}

int MAlloc(char** args) {
  uint16_t track, size;

  if (StrToUint16(*args++, &track, nullptr) < 0) return EXIT_FAILURE;
  if (StrToUint16(*args++, &size, nullptr) < 0) return EXIT_FAILURE;

  PrintResult(zm2_m_alloc(track, size));
  return EXIT_SUCCESS;
}

int MAssign(char** args) {
  uint16_t channel, track;

  if (StrToUint16(*args++, &channel, nullptr) < 0) return EXIT_FAILURE;
  if (StrToUint16(*args++, &track, nullptr) < 0) return EXIT_FAILURE;

  PrintResult(zm2_m_assign(channel, track));
  return EXIT_SUCCESS;
}

static void PrintFmTone(uint32_t no, Zm2FmTone* buf) {
  size_t i, slot;
  const char* sep;
  const uint8_t* p = &(*buf)[0];

  printf("(v%u,0\n", (unsigned int)no);

  printf("/\t AF  OM  WF  SY  SP PMD AMD PMS AMS PAN\n");
  sep = "\t";
  for (i = 0; i < 11; i++) {
    printf("%s%3u", sep, (unsigned int)*p++);
    sep = ",";
  }
  printf("\n");

  printf("/\t AR  DR  SR  RR  SL  OL  KS  ML DT1 DT2 AME\n");
  for (slot = 0; slot < 4; slot++) {
    sep = "\t";
    for (i = 0; i < 11; i++) {
      printf("%s%3u", sep, (unsigned int)*p++);
      sep = ",";
    }
    printf((slot == 3) ? ")\n" : "\n");
  }
}

static int SaveFmToneToFile(const char* filename, Zm2FmTone* buf) {
  FILE* fp = fopen(filename, "wb");
  if (!fp) {
    perror(filename);
    return -1;
  }
  if (fwrite(buf, 1, sizeof(*buf), fp) != sizeof(*buf)) {
    perror(filename);
    fclose(fp);
    return -1;
  }
  if (fclose(fp) != 0) {
    perror(filename);
    return -1;
  }
  return 0;
}

int MVget(char** args) {
  uint32_t no;
  Zm2FmTone buf = {0};
  int32_t result;

  if (StrToUint32(*args++, &no, nullptr) < 0) return EXIT_FAILURE;

  result = zm2_m_vget(no, &buf);
  PrintResult(result);

  if (result == 0) {
    // 成功時は音色の表示と、(指定されていれば)ファイルへの保存を行う
    PrintFmTone(no, &buf);
    if (*args && SaveFmToneToFile(*args, &buf) < 0) return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static int LoadFmToneFromFile(const char* filename, Zm2FmTone* buf) {
  FILE* fp = fopen(filename, "rb");
  if (!fp) {
    perror(filename);
    return -1;
  }
  if (fread(buf, 1, sizeof(*buf), fp) != sizeof(*buf)) {
    fprintf(stderr, "ファイルサイズが小さすぎます: %s\n", filename);
    fclose(fp);
    return -1;
  }
  fclose(fp);
  return 0;
}

int MVset(char** args) {
  uint32_t no;
  Zm2FmTone buf = {0};
  int32_t result;

  if (StrToUint32(*args++, &no, nullptr) < 0) return EXIT_FAILURE;
  if (LoadFmToneFromFile(*args++, &buf) < 0) return EXIT_FAILURE;

#if __GNUC__ < 2
  // gcc 1.xだと警告がでるので明示的にキャストしている
  result = zm2_m_vset(no, (const Zm2FmTone*)&buf);
#else
  result = zm2_m_vset(no, &buf);
#endif

  PrintResult(result);
  return EXIT_SUCCESS;
}

int MTempoSet(char** args) {
  uint32_t tempo;

  if (StrToUint32(*args++, &tempo, nullptr) < 0) return EXIT_FAILURE;
  if ((int32_t)tempo < 0) {
    // 符号ビットが1だとテンポ値の取得になるので指定できない
    fprintf(stderr, "テンポ値が大きすぎます。\n");
    return EXIT_FAILURE;
  }

  PrintResult(zm2_m_tempo_set(tempo));
  return EXIT_SUCCESS;
}

int MTempoGet(void) {
  struct Zm2TempoResult t = zm2_m_tempo_get();
  printf("テンポ値: %u, タイマー値: %u\n", (unsigned int)t.tempo,
         (unsigned int)t.timer);

  return EXIT_SUCCESS;
}

int MTempo(char** args) {  //
  return *args ? MTempoSet(args) : MTempoGet();
}

int MTrk(char** args) {
  uint32_t track;

  if (StrToUint32(*args++, &track, nullptr) < 0) return EXIT_FAILURE;
  if (!*args) {
    fprintf(stderr, "MMLが指定されていません。\n");
    return EXIT_FAILURE;
  }

  while (*args) {
    PrintResult(zm2_m_trk(track, *args++));
  }
  return EXIT_SUCCESS;
}

int MFree(char** args) {
  uint32_t track;

  if (StrToUint32(*args++, &track, nullptr) < 0) return EXIT_FAILURE;

  PrintResult(zm2_m_free(track));
  return EXIT_SUCCESS;
}

int MPlay(char** args) {
  if (*args) {
    struct Zm2Tracks tracks;

    if (ParseTrackList(*args++, &tracks) < 0) return EXIT_FAILURE;
    PrintResult(zm2_m_play(&tracks));
  } else {
    PrintResult(zm2_m_play_all());
  }
  return EXIT_SUCCESS;
}

int MStat(char** args) {
  if (*args) {
    uint32_t channels = 0;
    if (ParseChannelList(*args++, &channels) < 0) return EXIT_FAILURE;
    PrintResult(zm2_m_stat(channels));
  } else {
    // チャンネル指定=0の場合はビットマップで返されるので16進数で表示する
    PrintResultHex(zm2_m_stat(0));
  }
  return EXIT_SUCCESS;
}

int MStop(char** args) {
  if (*args) {
    struct Zm2Tracks tracks;

    if (ParseTrackList(*args++, &tracks) < 0) return EXIT_FAILURE;
    PrintResult(zm2_m_stop(&tracks));
  } else {
    PrintResult(zm2_m_stop_all());
  }
  return EXIT_SUCCESS;
}

int MCont(char** args) {
  if (*args) {
    struct Zm2Tracks tracks;

    if (ParseTrackList(*args++, &tracks) < 0) return EXIT_FAILURE;
    PrintResult(zm2_m_cont(&tracks));
  } else {
    PrintResult(zm2_m_cont_all());
  }
  return EXIT_SUCCESS;
}

int MAtoi(char** args) {
  uint32_t track;

  if (StrToUint32(*args++, &track, nullptr) < 0) return EXIT_FAILURE;

  PrintResultAddress(zm2_m_atoi(track));
  return EXIT_SUCCESS;
}

int InitAll(UNUSED char** args) {
  zm2_init_all();
  return EXIT_SUCCESS;
}

int IntStop(UNUSED char** args) {
  zm2_int_stop();
  return EXIT_SUCCESS;
}

int MPlay2(UNUSED char** args) {
  PrintResult(zm2_m_play2());
  return EXIT_SUCCESS;
}

static int ParseAdpcmSource(  //
    const char* str, const char** filename, uint32_t* note) {
  char* endptr;
  unsigned long val;

  *filename = str;
  *note = 0;

  if (!str || *str == '\0') {
    fprintf(stderr, "ファイル名またはソースノート番号が指定されていません。\n");
    return -1;
  }

  errno = 0;
  val = strtoul(str, &endptr, 0);

  if (errno == 0 && *endptr == '\0') {
    // 数値でノート番号が指定された
    if (ZM2_PCM_NOTE_MAX < val) {
      fprintf(stderr, "ノート番号が正しくありません。\n");
      return -1;
    }

    *note = (uint32_t)val;
    *filename = (const char*)note;
  }

  // それ以外ならファイル名の指定
  // (オクターブと音階による指定は対応していない)
  return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
static int ParseAdpcmConf(char** args, struct Zm2AdpcmConf* conf) {
  const char* endptr;

  if (!*args || **args == '\0') {
    fprintf(stderr, "設定ノート番号が指定されていません。\n");
    return -1;
  }
  if (StrToUint32(*args++, &conf->note, nullptr) < 0) return EXIT_FAILURE;

  if (!*args) return 0;
  if (StrToUint16(*args++, &conf->pitch, &endptr) < 0) return EXIT_FAILURE;
  if (*endptr) {
    if (StrToUint16(endptr, &conf->volume, nullptr) < 0) return EXIT_FAILURE;
  }

  if (!*args) return 0;
  if (StrToUint16(*args++, &conf->delay, &endptr) < 0) return EXIT_FAILURE;
  if (*endptr) {
    if (StrToInt16(endptr, &conf->mix_note, nullptr) < 0) return EXIT_FAILURE;
  }

  if (!*args) return 0;
  if (StrToUint16(*args++, &conf->cut_offset, &endptr) < 0) return EXIT_FAILURE;
  if (*endptr) {
    if (StrToUint16(endptr, &conf->cut_size, nullptr) < 0) return EXIT_FAILURE;
  }

  if (!*args) return 0;
  if (StrToUint32(*args++, &conf->reverse, nullptr) < 0) return EXIT_FAILURE;

  if (!*args) return 0;
  if (StrToUint16(*args++, &conf->fade_offset, &endptr) < 0)
    return EXIT_FAILURE;
  if (*endptr) {
    if (StrToInt8(endptr, &conf->fade_mode, &endptr) < 0) return EXIT_FAILURE;
    if (*endptr) {
      if (StrToUint8(endptr, &conf->fade_level, nullptr) < 0)
        return EXIT_FAILURE;
    }
  }

  return 0;
}
#pragma GCC diagnostic pop

int AdpcmRead(char** args) {
  const char* filename;
  uint32_t note;

  struct Zm2AdpcmConf conf = {0};
  conf.mix_note = -1;  // ミキシングなし

  if (ParseAdpcmSource(*args++, &filename, &note) < 0) return EXIT_FAILURE;
  if (ParseAdpcmConf(args, &conf) < 0) return EXIT_FAILURE;

  PrintResult(zm2_adpcm_read(&conf, filename));
  return EXIT_SUCCESS;
}

static uint8_t* ReadFile(const char* filename, size_t* filesize,
                         void*(allocator)(size_t)) {
  FILE* fp;
  struct stat st;
  uint8_t* buf;

  *filesize = 0;

  if (!filename) {
    fprintf(stderr, "ファイル名が指定されていません。\n");
    return nullptr;
  }
  fp = fopen(filename, "rb");
  if (!fp) {
    perror(filename);
    return nullptr;
  }
  if (fstat(fileno(fp), &st) < 0) {
    perror(filename);
    fclose(fp);
    return nullptr;
  }

  buf = allocator(st.st_size + 1);
  if (!buf) {
    perror(nullptr);
    fclose(fp);
    return nullptr;
  }

  if (fread(buf, 1, st.st_size, fp) != (size_t)st.st_size) {
    fprintf(stderr, "ファイルの読み込みに失敗しました。\n");
    free(buf);
    fclose(fp);
    return nullptr;
  }
  fclose(fp);

  buf[st.st_size] = '\0';  // テキストファイル終端用
  *filesize = st.st_size;
  return buf;
}

static uint8_t* ReadFileToHeap(const char* filename, size_t* filesize) {
  return ReadFile(filename, filesize, malloc);
}

static struct _mep* GetHumanMep(void) {
  int ssp = _dos_super(0);
  struct _mep* mep = (struct _mep*)((char*)_PSTA - 0x100);

  while (mep->parent_mp) mep = mep->parent_mp;

  _dos_super(ssp);
  return mep;
}

// このプログラムを終了しても解放されないバッファを確保する
//
// 解放は FreeHumanMemory() を使うこと。
//
// Human68kが確保したメモリブロックのように偽装することで、
// プロセス終了時の自動解放が適用されないようにしている。
// 常駐終了(DOS _KEEP_PR)する方法もあるが、占有メモリ量が
// 多くなるのでこのやり方にしている。
static void* MallocHumanMemory(size_t size) {
  const int FromLower = 0;
  const struct _mep* human = GetHumanMep();

  void* buf = _dos_malloc0(FromLower, (int)size, human);
  if ((int)buf < 0) {
    errno = ENOMEM;
    return nullptr;
  }
  return buf;
}

static void FreeHumanMemory(void* memptr) {
  if ((int)memptr <= 0) _dos_mfree(memptr);
}

static uint8_t* ReadFileToHumanMemory(const char* filename, size_t* filesize) {
  return ReadFile(filename, filesize, MallocHumanMemory);
}

static int is_zmd_file(const uint8_t* buf, size_t filesize) {
  if (filesize < ZM2_ZMD_MIN_SIZE) return 0;
  if (memcmp(buf, ZM2_ZMD_ID, ZM2_ZMD_ID_LEN) != 0) return 0;
  return 1;
}

static int PlayConvDataFast(char** args) {
  size_t filesize;
  uint8_t* buf = ReadFileToHumanMemory(*args++, &filesize);
  if (!buf) return EXIT_FAILURE;

  if (!is_zmd_file(buf, filesize)) {
    fprintf(stderr, "ZMDファイルではありません。\n");
    FreeHumanMemory(buf);
    return EXIT_FAILURE;
  }

  {
    // ZMD IDの直後のVersion Numberのアドレス
    const uint8_t* a1pos = buf + ZM2_ZMD_ID_LEN;

    PrintResult(zm2_play_cnv_data_fast(a1pos));
  }
  // バッファ上のデータを再生しているので解放しない

  return EXIT_SUCCESS;
}

static int PlayConvDataNormal(char** args) {
  size_t filesize;
  uint8_t* buf = ReadFileToHeap(*args++, &filesize);
  if (!buf) return EXIT_FAILURE;

  if (!is_zmd_file(buf, filesize)) {
    fprintf(stderr, "ZMDファイルではありません。\n");
    free(buf);
    return EXIT_FAILURE;
  }

  {
    // ZMD IDの直後のVersion Numberのアドレス
    const uint8_t* a1pos = buf + ZM2_ZMD_ID_LEN;
    uint32_t size = filesize - ZM2_ZMD_ID_LEN;

    PrintResult(zm2_play_cnv_data(size, a1pos));
  }
  // Z-MUSIC内にデータを転送したのでバッファは解放してよい
  free(buf);

  return EXIT_SUCCESS;
}

int PlayCnvData(char** args) {
  if (*args && strcmp(*args, "-f") == 0) {
    // -f: 高速応答モード
    args += 1;
    return PlayConvDataFast(args);
  }
  return PlayConvDataNormal(args);
}

static int GetZmdWord(uint8_t* p, const uint8_t* tail) {
  if (tail < (p + 2)) return -1;
  return (p[0] << 8) + p[1];
}

static uint8_t* SkipZmdString(uint8_t* p, const uint8_t* tail) {
  while (p < tail) {
    if (*p++ == '\0') return p;
  }
  return nullptr;
}

static uint8_t* SkipZmdCommonCommadInner(uint8_t* p, const uint8_t* tail) {
  int n;
  switch (*p++) {
    default:
      break;
    case 0x04:  // (Vn,...)
    case 0x1b:  //
      return p + 1 + 55;
    case 0x05:  // (On)
      return p + 2;
    case 0x15:  // (Bn) (v1.09Dまでは出力されていたコマンド)
      return p + 1;
    case 0x18:  // (X...) / .MIDI_DATA {...}
      n = GetZmdWord(p, tail);
      if (n < 0) break;
      return p + 2 + n;
    case 0x40:  // n=... / Onk=...
      p += 19;  // ファイル名またはノート番号まで進める
      n = GetZmdWord(p, tail);
      if (n < 0) break;

      if ((n >> 8) == 0) {  // ノート番号による指定
        if (n != 0) break;
        return p + 2 + 2;
      }
      // ファイル名による指定
      return SkipZmdString(p, tail);
    case 0x42:  // (Zn)
      return p + 1 + 4;
    case 0x4a:  // .WAVE_FORM ... {...}
      n = GetZmdWord(p, tail);
      if (n < 0) break;
      return p + 2 + 1 + 1 + 2 + n * 2;
    case 0x60:  // .ADPCM_LIST ...
    case 0x61:  // .PRINT ...
    case 0x62:  // .MIDI_DUMP=...
    case 0x63:  // .ADPCM_BLOCK_DATA ...
    case 0x7f:  // .COMMENT ...
      return SkipZmdString(p, tail);
    case 0x7e:  // ダミーコード
      return p;
  }
  return nullptr;
}

static uint8_t* SkipZmdCommonCommand(uint8_t* p, size_t filesize) {
  const uint8_t* tail = p + filesize;

  if (!is_zmd_file(p, filesize)) return nullptr;
  p += ZM2_ZMD_ID_LEN + ZM2_ZMD_VER_LEN;

  while (p && p < tail) {
    if (*p == 0xff) {  // 共通コマンド終了コード
      p += 1;
      if (((uintptr_t)p & 1) != 0) {
        if (tail <= p || *p++ != 0xff) break;
      }
      return p;
    }
    p = SkipZmdCommonCommadInner(p, tail);
  }
  return nullptr;
}

int SePlay(char** args) {
  uint32_t track;
  size_t filesize;
  uint8_t* buf;
  const uint8_t* a1pos;

  if (StrToUint32(*args++, &track, nullptr) < 0) return -1;

  buf = ReadFileToHumanMemory(*args++, &filesize);
  if (!buf) return EXIT_FAILURE;

  // ここでは普通の.zmdファイルをそのまま指定して試せるように共通コマンドの
  // スキップを行っているが、実用上は以下のいずれかにした方がよい。
  // * ヘッダと共通コマンドを削除したデータをファイルとして保存しておく
  // * 共通コマンドを含まないデータ(スキップ長が10バイト固定)として作成する
  // * スキップするバイト数を調べて保存しておく
  a1pos = SkipZmdCommonCommand(buf, filesize);
  if (!a1pos) {
    fprintf(stderr, "ZMDファイルではありません。\n");
    FreeHumanMemory(buf);
    return EXIT_FAILURE;
  }

  zm2_se_play(track, a1pos);
  // バッファ上のデータを再生しているので解放しない

  return EXIT_SUCCESS;
}

int SeAdpcm1(char** args) {
  uint8_t priority = ZM2_PRIORITY_MIN;
  uint8_t freq = ZM2_PCM_15_6KHZ;
  uint8_t pan = ZM2_PANPOT_CENTER;

  size_t filesize;
  uint8_t* buf = ReadFileToHumanMemory(*args++, &filesize);
  if (!buf) return EXIT_FAILURE;

  if (*args && **args) {
    const char* endptr;
    if (StrToUint8(*args++, &priority, &endptr) < 0) return -1;
    if (*endptr) {
      if (StrToUint8(*args++, &freq, &endptr) < 0) return -1;
      if (*endptr) {
        if (StrToUint8(*args++, &pan, nullptr) < 0) return -1;
      }
    }
  }

  zm2_se_adpcm1(filesize, priority, freq, pan, buf);
  // バッファ上のデータを再生しているので解放しない

  return EXIT_SUCCESS;
}

int SeAdpcm2(char** args) {
  uint32_t note;
  uint8_t priority = ZM2_PRIORITY_MIN;
  uint8_t freq = ZM2_PCM_15_6KHZ;
  uint8_t pan = ZM2_PANPOT_CENTER;

  if (StrToUint32(*args++, &note, nullptr) < 0) return EXIT_FAILURE;

  if (*args && **args) {
    const char* endptr;
    if (StrToUint8(*args++, &priority, &endptr) < 0) return -1;
    if (*endptr) {
      if (StrToUint8(*args++, &freq, &endptr) < 0) return -1;
      if (*endptr) {
        if (StrToUint8(*args++, &pan, nullptr) < 0) return -1;
      }
    }
  }

  zm2_se_adpcm2(note, priority, freq, pan);
  return EXIT_SUCCESS;
}

int SetChMode(char** args) {
  uint32_t mode;

  if (StrToUint32(*args++, &mode, nullptr) < 0) return EXIT_FAILURE;

  zm2_set_ch_mode(mode);
  return EXIT_SUCCESS;
}

int MidiRec(UNUSED char** args) {
  zm2_midi_rec();
  return EXIT_SUCCESS;
}

int MidiRecEnd(char** args) {
  uint32_t mode = 0;
  struct Zm2MidiRecResult r;

  if (*args && **args) {
    if (StrToUint32(*args++, &mode, nullptr) < 0) return EXIT_FAILURE;
  }

  r = zm2_midi_rec_end(mode);
  // r.data == nullptrならr.sizeがエラーコード
  printf("データサイズ: %u, データアドレス: 0x%08x\n", (unsigned int)r.size,
         (unsigned int)r.data);

  // ファイルへの保存機能があると実用的だが、実装していない

  return EXIT_SUCCESS;
}

int MidiTrns(char** args) {
  int isBinary = 0;
  size_t filesize;
  uint8_t* buf;

  if (*args && strcmp(*args, "-b") == 0) {
    args += 1;
    isBinary = 1;
  }
  buf = ReadFileToHeap(*args++, &filesize);
  if (!buf) return EXIT_FAILURE;

  PrintResult(zm2_midi_trns(isBinary ? filesize : 0, buf));
  free(buf);
  return EXIT_SUCCESS;
}

int CalcTotal(char** args) {
  uint32_t mode = 0;

  if (*args && **args) {
    if (StrToUint32(*args++, &mode, nullptr) < 0) return EXIT_FAILURE;
  }

  PrintResult(zm2_calc_total(mode));
  return EXIT_SUCCESS;
}

int FadeOut(char** args) {
  int32_t speed = 0;

  if (*args && **args) {
    if (StrToInt32(*args++, &speed, nullptr) < 0) return EXIT_FAILURE;
  }

  PrintResult(zm2_fade_out(speed));
  return EXIT_SUCCESS;
}

int MVset2(char** args) {
  uint32_t no;
  Zm2FmTone buf = {0};
  int32_t result;

  if (StrToUint32(*args++, &no, nullptr) < 0) return EXIT_FAILURE;
  if (LoadFmToneFromFile(*args++, &buf) < 0) return EXIT_FAILURE;

#if __GNUC__ < 2
  // gcc 1.xだと警告がでるので明示的にキャストしている
  result = zm2_m_vset2(no, (const Zm2FmTone*)&buf);
#else
  result = zm2_m_vset(no, &buf);
#endif

  PrintResult(result);
  return EXIT_SUCCESS;
}

int SendRdExc(char** args) {
  uint16_t devid;
  uint16_t modelid;
  size_t filesize;
  uint8_t* buf;

  if (StrToUint16(*args++, &devid, nullptr) < 0) return EXIT_FAILURE;
  if (StrToUint16(*args++, &modelid, nullptr) < 0) return EXIT_FAILURE;
  buf = ReadFileToHeap(*args++, &filesize);
  if (!buf) return EXIT_FAILURE;

  zm2_send_rd_exc(filesize, devid, modelid, buf);
  free(buf);
  return EXIT_SUCCESS;
}

int SendExc(char** args) {
  size_t filesize;
  uint8_t* buf;

  buf = ReadFileToHeap(*args++, &filesize);
  if (!buf) return EXIT_FAILURE;

  zm2_send_exc(filesize, buf);
  free(buf);
  return EXIT_SUCCESS;
}

int AdpcmBlockData(char** args) {
  if (!*args || **args == '\0') {
    fprintf(stderr, "ファイル名が指定されていません。\n");
    return -1;
  }

  PrintResult(zm2_adpcm_block_data(*args++));
  return EXIT_SUCCESS;
}

static void PrintRealChTbl(const Zm2RealChTbl* real_ch_tbl) {
  const uint8_t* p = &(*real_ch_tbl)[0];
  size_t i;
  char sep = ' ';

  printf("絶対チャンネルテーブル: 0x%08x\n ", (unsigned int)p);
  for (i = 0; i < sizeof(*real_ch_tbl); i += 1) {
    printf("%c%d", sep, *p++);
    sep = ',';
  }
  printf("\n");
}

static void PrintPlayTrkTbl(const uint8_t* p) {
  char sep = ' ';

  printf("演奏トラックテーブル: 0x%08x\n ", (unsigned int)p);
  while (*p != 0xff) {
    printf("%c%d", sep, *p++);
    sep = ',';
  }
  printf("%c0xff\n", sep);
}

int GetTrkTbl(UNUSED char** args) {
  struct Zm2TrkTblResult t = zm2_get_trk_tbl();
  PrintRealChTbl(t.real_ch_tbl);
  PrintPlayTrkTbl(t.play_trk_tbl);
  return EXIT_SUCCESS;
}

static volatile int8_t IsLoopOccurred = 0;

// Z-MUSICの割り込み処理内からjsrされるサブルーチンなのでcall-usedな
// レジスタ(d0-d2/a0-a2)のうち破壊してはいけないものもあるが(v2.08ではd1)、
// そのように書くのが面倒なので破壊しないことを期待して特に対処していない。
void LoopOccurred(void) { IsLoopOccurred = -1; }

int SetLoopTime(char** args) {
  uint8_t count;

  if (StrToUint8(*args++, &count, nullptr) < 0) return EXIT_FAILURE;

  fprintf(stderr, "指定した回数だけループするまで待機します。\n");
  zm2_loop_time_set(count, LoopOccurred);
  while (!IsLoopOccurred) {
    // 呼び出しが発生するまで待機する
  }
  zm2_loop_time_cancel();
  fprintf(stderr, "ループしました。\n");

  return EXIT_SUCCESS;
}

int GetPlayWork(char** args) {
  uint8_t track;
  struct Zm2PlayWorkResult w;

  if (StrToUint8(*args++, &track, nullptr) < 0) return -1;

  // ファンクションコール側でエラーチェックしていないので自前でチェックする
  if (track < ZM2_TRACK_MIN || ZM2_TRACK_MAX < track) {
    fprintf(stderr, "トラック番号が正しくありません。\n");
    return EXIT_FAILURE;
  }

  w = zm2_get_play_work(track);
  printf("コンパイルワーク: 0x%08x, 演奏トラックワーク: 0x%08x\n",
         (unsigned int)w.cnv_wk, (unsigned int)w.seq_wk);

  return EXIT_SUCCESS;
}

static const char* GetTimerModeString(uint32_t mode) {
  if (mode == ZM2_TIMER_MODE_A) return "timer A";
  if (mode == ZM2_TIMER_MODE_B) return "timer B";
  return "(unknown)";
}

int GetTimerMode(UNUSED char** args) {
  uint32_t result = zm2_get_timer_mode();
  printf("タイマーモード: %u = %s\n", (unsigned int)result,
         GetTimerModeString(result));
  return EXIT_SUCCESS;
}

int SetFmMasterVol(char** args) {
  uint8_t volume;

  if (StrToUint8(*args++, &volume, nullptr) < 0) return EXIT_FAILURE;

  PrintResult(zm2_set_fm_master_vol(volume));
  return EXIT_SUCCESS;
}

int SetTimerValue(char** args) {
  uint32_t timer;

  if (StrToUint32(*args++, &timer, nullptr) < 0) return EXIT_FAILURE;

  PrintResult(zm2_set_timer_value(timer));
  return EXIT_SUCCESS;
}

static int ParseReleaseProgram(  //
    const char* str, const char** filename, int32_t* code) {
  char* endptr;
  unsigned long val;

  *filename = str;
  *code = -1;

  if (!str || *str == '\0') {
    fprintf(stderr, "解除プログラム名または登録コードが指定されていません。\n");
    return -1;
  }

  errno = 0;
  val = strtoul(str, &endptr, 0);

  if (errno == 0 && *endptr == '\0') {
    // 数値で解除用の登録コードが指定された
    *code = (int32_t)val;
    *filename = nullptr;
  }

  // それ以外なら登録用の解除プログラム名の指定
  return 0;
}

int ReleaseSupport(char** args) {
  const char* filename;
  int32_t code;

  if (ParseReleaseProgram(*args++, &filename, &code) < 0) return EXIT_FAILURE;

  if (filename) {
    PrintResult(zm2_release_support_set(filename));
  } else {
    PrintResult(zm2_release_support_cancel(code));
  }
  return EXIT_SUCCESS;
}

int JumpActive(char** args) {
  uint8_t mode;

  if (StrToUint8(*args++, &mode, nullptr) < 0) return EXIT_FAILURE;

  zm2_jump_active(mode);
  return EXIT_SUCCESS;
}

int SetMclk(char** args) {
  uint32_t count;

  if (StrToUint32(*args++, &count, nullptr) < 0) return EXIT_FAILURE;

  // count==0 だとZ-MUSIC内でゼロ除算を行ってしまい白帯が表示される。
  // 試せるようにここでは値の確認をしていない。
  // (ちなみに count>254 の場合はエラーとなり、エラーコード70が返る)

  PrintResult(zm2_set_mclk(count));
  return EXIT_SUCCESS;
}

int PictureSync(char** args) {
  uint32_t mode;

  if (StrToUint32(*args++, &mode, nullptr) < 0) return EXIT_FAILURE;

  PrintResult(zm2_picture_sync(mode));
  return EXIT_SUCCESS;
}

int MaskChannels(char** args) {
  uint32_t channels = 0;

  if (*args) {
    if (ParseChannelList(*args++, &channels) < 0) return EXIT_FAILURE;
  }
  PrintResult(zm2_mask_channels(channels));

  return EXIT_SUCCESS;
}

int BufferInfo(UNUSED char** args) {
  PrintResultAddress(zm2_buffer_info());
  return EXIT_SUCCESS;
}

int SetZpdTbl(char** args) {
  size_t filesize;
  uint8_t* buf = ReadFileToHumanMemory(*args++, &filesize);
  if (!buf) return EXIT_FAILURE;

  {
    const uint8_t* a1pos = buf + ZM2_ZPD_ID_LEN;
    PrintResult(zm2_set_zpd_tbl(a1pos));
  }
  // バッファ上のデータを登録しているので解放しない
  // (エラー(d0.l=38)の場合も、それまでのデータは登録されている)

  return EXIT_SUCCESS;
}

int SetOutputLevel(char** args) {
  uint32_t channels = ~0;
  int8_t level = -1;

  if (*args) {
    if (ParseChannelList(*args++, &channels) < 0) return EXIT_FAILURE;
    if (*args) {
      if (StrToInt8(*args++, &level, nullptr) < 0) return EXIT_FAILURE;
    }
  }

  // ビットマップで返されるので16進数で表示する
  PrintResultHex(zm2_set_output_level(channels, level));

  return EXIT_SUCCESS;
}

struct WaveParam {
  uint8_t no;
  uint8_t type;
  uint16_t point;
};

static int ParseWaveParam(const char* str, struct WaveParam* w) {
  const char* endptr;

  w->no = 0;  // 実際に指定できるのは8～31
  w->type = ZM2_WAVETYPE_ONESHOT;
  w->point = 0;

  if (!str || *str == '\0') {
    fprintf(stderr, "波形メモリ設定が指定されていません。\n");
    return -1;
  }
  if (StrToUint8(str, &w->no, &endptr) < 0) return -1;
  if (*endptr) {
    if (StrToUint8(endptr, &w->type, &endptr) < 0) return -1;
    if (*endptr) {
      if (StrToUint16(endptr, &w->point, nullptr) < 0) return -1;
    }
  }
  return 0;
}

int SetWaveForm1(char** args) {
  struct WaveParam w;
  size_t filesize;
  uint8_t* buf;

  if (ParseWaveParam(*args++, &w) < 0) return EXIT_FAILURE;

  buf = ReadFileToHeap(*args++, &filesize);
  if (!buf) return EXIT_FAILURE;

  PrintResult(zm2_set_wave_form1(filesize, w.no, w.type, w.point, buf));

  // Z-MUSIC内にデータを転送したのでバッファは解放してよい
  free(buf);

  return EXIT_SUCCESS;
}

int SetWaveForm2(char** args) {
  struct WaveParam w;
  size_t filesize;
  uint8_t* buf;

  if (ParseWaveParam(*args++, &w) < 0) return EXIT_FAILURE;

  buf = ReadFileToHumanMemory(*args++, &filesize);
  if (!buf) return EXIT_FAILURE;

  {
    uint32_t count = (uint32_t)filesize / 2;  // バイト数ではなくデータ個数

    PrintResult(zm2_set_wave_form2(count, w.no, w.type, w.point, buf));
  }
  // バッファ上のデータを登録しているので解放しない

  return EXIT_SUCCESS;
}

int MaskTracks(char** args) {
  int32_t track;

  if (StrToInt32(*args++, &track, nullptr) < 0) return EXIT_FAILURE;

  // ファンクションコール側でエラーチェックしていないので自前でチェックする
  // -80～-1:マスク 0:全トラック解除 1～80:解除
  if (track < -ZM2_TRACK_MAX || ZM2_TRACK_MAX < track) {
    fprintf(stderr, "トラック番号が正しくありません。\n");
    return EXIT_FAILURE;
  }
  zm2_mask_tracks(track);

  return EXIT_SUCCESS;
}

int SetOutputLevel2(char** args) {
  uint32_t track = 0;
  int8_t level = -1;

  if (*args) {
    if (StrToUint32(*args++, &track, nullptr) < 0) return EXIT_FAILURE;
    if (*args) {
      if (StrToInt8(*args++, &level, nullptr) < 0) return EXIT_FAILURE;
    }
  }
  // ファンクションコール側でエラーチェックしていないので自前でチェックする
  // 0:全トラック対象 1～80:トラック番号指定
  if (ZM2_TRACK_MAX < track) {
    fprintf(stderr, "トラック番号が正しくありません。\n");
    return EXIT_FAILURE;
  }

  zm2_set_output_level2(track, level);
  return EXIT_SUCCESS;
}

int GetLoopTime(UNUSED char** args) {
  PrintResult(zm2_get_loop_time());
  return EXIT_SUCCESS;
}

int Get1stComment(UNUSED char** args) {
  const char* s = zm2_get_1st_comment();

  printf("コメント: 0x%08x\n ", (unsigned int)s);
  puts(s);

  return EXIT_SUCCESS;
}

int IntStart(UNUSED char** args) {
  zm2_int_start();
  return EXIT_SUCCESS;
}

int ZmStatus(UNUSED char** args) {
  PrintResultAddress(zm2_zm_status());
  return EXIT_SUCCESS;
}

int RelativeUv(char** args) {
  uint32_t mode;

  if (StrToUint32(*args++, &mode, nullptr) < 0) return EXIT_FAILURE;

  zm2_relative_uv(mode);

  return EXIT_SUCCESS;
}

int InterceptPlay(char** args) {
  int32_t mode;

  if (StrToInt32(*args++, &mode, nullptr) < 0) return EXIT_FAILURE;

  zm2_intercept_play(mode);

  return EXIT_SUCCESS;
}

static volatile uint8_t* ierb = (uint8_t*)0x00e88009;
static volatile uint8_t* imrb = (uint8_t*)0x00e88015;

static void StopMfpFmIrq(void) {
  int ssp = _dos_super(0);
  *ierb &= ~0x08;
  *imrb &= ~0x08;
  _dos_super(ssp);

  printf("MFP FMIRQ 割り込みを停止します。\n");
}

static void RestartMfpFmIrq(void) {
  int ssp = _dos_super(0);
  *imrb |= 0x08;
  *ierb |= 0x08;
  _dos_super(ssp);

  printf("MFP FMIRQ 割り込みを再開します。\n");
}

int MInp1(char** args) {
  uint32_t wait = 0;

  if (*args) {
    if (StrToUint32(*args++, &wait, nullptr) < 0) return EXIT_FAILURE;
  }

  PrintResult(zm2_m_inp1(wait));
  return EXIT_SUCCESS;
}

int MOut1(char** args) {
  uint8_t data;

  if (StrToUint8(*args++, &data, nullptr) < 0) return EXIT_FAILURE;

  PrintResult(zm2_m_out1(data));
  return EXIT_SUCCESS;
}

int OccupiedSize(UNUSED char** args) {
  struct Zm2OccupiedSize s;

  zm2_occupied_size(&s);
  printf("トラックバッファ使用容量: 0x%08x\n", (unsigned int)s.track_usage);
  printf("ADPCMバッファ使用容量: 0x%08x\n", (unsigned int)s.adpcm_usage);
  printf("汎用ワークエリア使用容量: 0x%08x\n", (unsigned int)s.work_usage);

  return EXIT_SUCCESS;
}

static int GetSeqWkList(struct Zm2Tracks* tracks, void** seq_wk_tbl) {
  int count = 0;
  uint32_t track;

  for (track = ZM2_TRACK_MIN; track <= ZM2_TRACK_MAX; track += 1) {
    int isset;
    zm2_tracks_isset(tracks, track, &isset);
    if (isset) {
      struct Zm2PlayWorkResult w = zm2_get_play_work(track);
      *seq_wk_tbl++ = (void*)w.seq_wk;
      count += 1;
    }
  }
  return count;
}

int InputLoopCount(int default_value) {
  char buf[16];
  size_t len;
  int32_t count;

  if (fgets(buf, sizeof(buf), stdin) == 0) return 0;
  len = strlen(buf);
  if (len) buf[len - 1] = '\0';  // remove '\n'

  if (buf[0] == '\0') return default_value;

  if (buf[0] == 'q' || strcmp(buf, "exit") == 0) return 0;

  if (StrToInt32(buf, &count, nullptr) < 0) return 0;
  return (count <= 0 || 1024 < count) ? 0 : count;
}

int CallIntPlayOpe(char** args) {
  struct Zm2Tracks tracks;
  int track_count;
  void* seq_wk_tbl[ZM2_TRACK_COUNT];
  int loop_count = 64;

  if (ParseTrackList(*args++, &tracks) < 0) return EXIT_FAILURE;

  // 独自に演奏トラックワークを用意するのは大変なので
  // Z-MUSICの内部ワークを流用している
  // (サンプルコードとはいえ良くない方法)
  track_count = GetSeqWkList(&tracks, seq_wk_tbl);

  StopMfpFmIrq();

  // ある程度の回数を呼び出さないとなかなか演奏が進行しないので
  // 一度に複数回呼び出せるようにしている
  // (これも、サンプルコードとはいえ良くない方法)
  printf(
      "演奏ルーチンを呼び出す回数を入力してください。\n"
      "  qで終了、空行で前回と同じ回数(初期値%d)。\n",
      loop_count);

  for (;;) {
    int lp;

    loop_count = InputLoopCount(loop_count);
    if (loop_count <= 0) break;

    for (lp = 0; lp < loop_count; lp += 1) {
      int index;
      for (index = 0; index < track_count; index += 1) {
        zm2_call_int_play_ope(seq_wk_tbl[index]);
      }
    }
  }

  // 無理やり内部ワークを使っている影響か、
  // 割り込みを再開しているが演奏は再開されない
  RestartMfpFmIrq();

  return EXIT_SUCCESS;
}

static int UnknownCommand(const char* name) {
  fprintf(stderr, "コマンドが正しくありません: %s\n", name);
  return EXIT_FAILURE;
}

static int PrintCommandUsage(char** args) {
  const Command* cmd;

  if (!*args) {
    printf("Usage: " PROGRAM " help <command>\n");
    return EXIT_FAILURE;
  }
  cmd = GetCommand(*args);
  if (!cmd) return UnknownCommand(*args);

  printf("Usage: " PROGRAM " %s", GetCommandName(cmd));
  if (cmd->usage) printf(" %s", cmd->usage);
  printf("\n");

  return EXIT_SUCCESS;
}

// コマンド名の先頭が'*'ならZ-MUSICのバージョンチェックをしない
static const Command commands[] = {
    {"*help", PrintCommandUsage, "<command>"},
    {"*version", PrintZmusicVersion, nullptr},
    {"*spec", PrintZmusicSpec, nullptr},
    {"m_init", MInit, nullptr},
    {"m_alloc", MAlloc, "<track> <size>"},
    {"m_assign", MAssign, "<channel> <track>"},
    {"m_vget", MVget, "<tone> [file]"},
    {"m_vset", MVset, "<tone> <file>"},
    {"m_tempo", MTempo, "[tempo]"},
    {"m_trk", MTrk, "<track> <mml> …"},
    {"m_free", MFree, "<track>"},
    {"m_play", MPlay, "[track,…]"},
    {"m_stat", MStat, "[channel,…]"},
    {"m_stop", MStop, "[track,…]"},
    {"m_cont", MCont, "[track,…]"},
    {"m_atoi", MAtoi, "<track>"},
    {"init_all", InitAll, nullptr},
    {"int_stop", IntStop, nullptr},
    {"m_play2", MPlay2, nullptr},
    {"adpcm_read", AdpcmRead,
     "<filename|note> <note> [pitch,vol] [delay,note] [offset,size] [reverse] "
     "[offset,mode,level]"},
    {"play_cnv_data", PlayCnvData, "[-f] <filename>"},
    {"se_play", SePlay, "<track> <filename>"},
    {"se_adpcm1", SeAdpcm1, "<filename> [priority,freq,pan]"},
    {"se_adpcm2", SeAdpcm2, "<note> [priority,freq,pan]"},
    {"set_ch_mode", SetChMode, "<mode>"},
    {"midi_rec", MidiRec, nullptr},
    {"midi_rec_end", MidiRecEnd, "<mode>"},
    {"midi_trns", MidiTrns, "[-b] <filename>"},
    {"calc_total", CalcTotal, "[mode]"},
    {"fade_out", FadeOut, "[speed]"},
    {"m_vset2", MVset2, "<tone> <file>"},
    {"send_rd_exc", SendRdExc, "<devid> <modelid> <filename>"},
    {"send_exc", SendExc, "<filename>"},
    // {"sc55_p_rsv", Sc55PRsv, "<devid> <filename>"},
    // {"sc55_reverb", Sc55Reverb, "<devid> <filename>"},
    // {"sc55_chorus", Sc55Chorus, "<devid> <filename>"},
    // {"sc55_part_parameter", "Sc55PartParameter",
    //  "<part> <devid> <filename>"},
    // {"sc55_drum_parameter", "Sc55DrumParameter",
    //  "<map> <note> <devid> <filename>"},
    // {"sc55_print", Sc55Print, "<devid> <message>"},
    // {"sc55_display", Sc55Display, "<devid> <filename>"},
    // {"mt32_p_rsv", Mt32PRsv, "<devid> <filename>"},
    // {"mt32_reverb", Mt32Reverb, "<devid> <filename>"},
    // {"mt32_setup", Mt32Setup, "<devid> <filename>"},
    // {"mt32_drum", Mt32Reverb, "<note> <devid> <filename>"},
    // {"mt32_common", Mt32Common, "<prog> <devid> <filename>"},
    // {"mt32_partial", Mt32Partial, "<prog> <part> <devid> <filename>"},
    // {"mt32_patch", Mt32Patch, "<patch> <devid> <filename>"},
    // {"mt32_print", Mt32Print, "<devid> <message>"},
    // {"u220_setup", U220Setup, "<devid> <filename>"},
    // {"u220_common", U220Common, "<devid> <filename>"},
    // {"u220_d_setup", U220DSetup, "<devid> <filename>"},
    // {"u220_p_setup", U220PSetup, "<part> <devid> <filename>"},
    // {"u220_print", U220Print, "<devid> <message>"},
    // {"u220_timbre", U220Timbre, "<prog> <devid> <filename>"},
    // {"u220_drum", U220Drum, "<note> <devid> <filename>"},
    // {"m1_midi_ch", M1MidiCh, "<filename>"},
    // {"send_to_m1", SendToM1, "<devid>"},
    // {"m1_p_setup", M1PSetup, "<filename>"},
    // {"m1_e_setup", M1ESetup, "<filename>"},
    // {"m1_print", M1Print, "<message>"},
    {"adpcm_block_data", AdpcmBlockData, "<filename>"},
    {"get_trk_tbl", GetTrkTbl, nullptr},
    {"set_loop_time", SetLoopTime, "<count>"},
    {"get_play_work", GetPlayWork, "<track>"},
    {"get_timer_mode", GetTimerMode, nullptr},
    {"set_fm_master_vol", SetFmMasterVol, "<volume>"},
    {"set_timer_value", SetTimerValue, "<timer>"},
    {"release_support", ReleaseSupport, "<filename|code>"},
    {"jump_active", JumpActive, "<mode>"},
    {"set_mclk", SetMclk, "<count>"},
    {"picture_sync", PictureSync, "<mode>"},
    {"mask_channels", MaskChannels, "<channel,…>"},
    {"buffer_info", BufferInfo, nullptr},
    {"set_zpd_tbl", SetZpdTbl, "<filename>"},
    {"set_output_level", SetOutputLevel, "[channel,…] [level]"},
    // {"eox_wait", EoxWait, "<wait>"},
    {"set_wave_form1", SetWaveForm1, "<type,no,point> <filename>"},
    {"set_wave_form2", SetWaveForm2, "<type,no,point> <filename>"},
    {"mask_tracks", MaskTracks, "<-track|0|track>"},
    {"set_output_level2", SetOutputLevel2, "[track] [level]"},
    {"get_loop_time", GetLoopTime, nullptr},
    {"get_1st_comment", Get1stComment, nullptr},
    {"int_start", IntStart, nullptr},
    {"zm_status", ZmStatus, nullptr},
    // {"sc55_init", Sc55Init, "<devid>"},
    // {"mt32_init", Mt32Init, "<devid>"},
    {"relative_uv", RelativeUv, "<mode>"},
    {"intercept_play", InterceptPlay, "<mode>"},
    {"m_inp1", MInp1, "[wait]"},
    {"m_out1", MOut1, "<data>"},
    {"occupied_size", OccupiedSize, nullptr},
    {"call_int_play_ope", CallIntPlayOpe, "<track,…>"},
};

static void PrintCommandList(void) {
  const size_t screen_width = 96;
  size_t col = 1;
  size_t i;

  printf("Commands:\n ");
  for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
    const char* s = GetCommandName(&commands[i]);
    size_t len = strlen(s);

    col += 1 + len;
    if (col >= screen_width) {
      printf("\n ");  // 行を跨ぎそうになったら改行する
      col = 2 + len;
    }
    printf(" %s", s);
  }
  printf("\n");
}

static int PrintUsage(void) {
  printf(PROGRAM " " VERSION
                 "  Copyright (C) 2026 TcbnErik\n"
                 "Usage: " PROGRAM " <command> [arguments...]\n");
  PrintCommandList();
  return EXIT_FAILURE;
};

int main(int argc, char* argv[]) {
  const Command* cmd;
  const char* comamnd_name = (argc >= 2) ? argv[1] : nullptr;

  if (!comamnd_name) return PrintUsage();

  cmd = GetCommand(comamnd_name);
  if (!cmd) return UnknownCommand(comamnd_name);

  if (!IsNoCheckCommand(cmd) && zm2_get_version() < 0) {
    fprintf(stderr, "Z-MUSIC v2が常駐していません。\n");
    return EXIT_FAILURE;
  }
  return cmd->func(argv + 2);
}

static const Command* GetCommand(const char* name) {
  if (name) {
    size_t i;
    for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
      const Command* cmd = &commands[i];
      if (strcmp(GetCommandName(cmd), name) == 0) return cmd;
    }
  }
  return nullptr;
}
