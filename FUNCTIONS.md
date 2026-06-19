# libzm2 関数リファレンスマニュアル

--------

## libzm2.h - メイン関数

### `zm2_get_version_super()`
```c
int32_t zm2_get_version_super(void)
```
* 説明: Z-MUSICのバージョン番号を取得します。
  * この関数はスーパーバイザーモードで呼び出さなければなりません。
  * この関数はZ-MUSICの常駐確認を行うためのユーティリティ関数です。
* 戻り値: バージョン番号(`0x0000XYTZ`でX.YZがバージョン、Tがタイプ)、非常駐時は-1

### `zm2_get_version()`
```c
int32_t zm2_get_version(void)
```
* 説明: Z-MUSICのバージョン番号を取得します。
  * この関数はスーパーバイザーモード、ユーザーモードのいずれでも呼び出せます。
  * この関数はZ-MUSICの常駐確認を行うためのユーティリティ関数です。
* 戻り値: バージョン番号(`0x0000XYTZ`でX.YZがバージョン、Tがタイプ)、非常駐時は-1

### `zm2_get_spec_super()`
```c
struct Zm2DriverSpec zm2_get_spec_super(void)
```
* 説明: Z-MUSICのバージョン番号や種類を取得します。
  * この関数はスーパーバイザーモードで呼び出さなければなりません。
  * この関数はZ-MUSICの詳細を取得するためのユーティリティ関数です。
* 戻り値: `struct Zm2DriverSpec`、非常駐時はすべてのメンバーが-1
  * `version`: `0x0000XY0Z`でX.YZがバージョン(タイプの桁が0になっている)。
  * `type`: libzm2const.hの`ZM2_DRIVER_TYPE_*`を参照。
  * `is_tiny`: 0ならzmusic.x、1ならzmsc.x。

### `zm2_get_spec()`
```c
struct Zm2DriverSpec zm2_get_spec(void)
```
* 説明: Z-MUSICのバージョン番号や種類を取得します。
  * この関数はスーパーバイザーモード、ユーザーモードのいずれでも呼び出せます。
  * この関数はZ-MUSICの詳細を取得するためのユーティリティ関数です。
* 戻り値: `struct Zm2DriverSpec`、非常駐時はすべてのメンバーが-1
  * `version`: `0x0000XY0Z`でX.YZがバージョン(タイプの桁が0になっている)。
  * `type`: libzm2const.hの`ZM2_DRIVER_TYPE_*`を参照。
  * `is_tiny`: 0ならzmusic.x、1ならzmsc.x。

### `zm2_m_init()`
```c
void zm2_m_init(void)
```
* 説明: m_init

### `zm2_m_alloc()`
```c
int32_t zm2_m_alloc(uint16_t track, uint16_t size)
```
* 説明: m_alloc
* 引数:
  * `track`: トラック番号
  * `size`: 割り当てサイズ
* 戻り値: Z-MUSICエラーコード

### `zm2_m_assign()`
```c
int32_t zm2_m_assign(uint16_t channel, uint16_t track)
```
* 説明: m_assign
* 引数:
  * `channel`: チャネル番号
  * `track`: トラック番号
* 戻り値: Z-MUSICエラーコード

### `zm2_m_vget()`
```c
int32_t zm2_m_vget(uint32_t no, Zm2FmTone* buf)
```
* 説明: m_vget
* 引数:
  * `no`: 音色番号
  * `buf`: 55バイトのバッファへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_m_vset()`
```c
int32_t zm2_m_vset(uint32_t tone, const Zm2FmTone* data)
```
* 説明: m_set
* 引数:
  * `tone`: 音色番号
  * `data`: 55バイトのFM音色データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_m_tempo_get()`
```c
struct Zm2TempoResult zm2_m_tempo_get(void)
```
* 説明: m_tempo (テンポの取り出し)
  * テンポの取得と設定で戻り値の型が異なるため関数が分かれています。
* 戻り値: `struct Zm2TempoResult`
  * `tempo`: テンポ値
  * `timer`: タイマー値

### `zm2_m_tempo_set()`
```c
int32_t zm2_m_tempo_set(uint32_t tempo)
```
* 説明: m_tempo (テンポの設定)
  * テンポの取得と設定で戻り値の型が異なるため関数が分かれています。
* 引数:
  * `tempo`: テンポ値
* 戻り値: Z-MUSICエラーコード

### `zm2_m_trk()`
```c
int32_t zm2_m_trk(uint32_t track, const char* mml)
```
* 説明: m_trk
* 引数:
  * `track`: トラック番号
  * `mml`: MML文字列へのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_m_free()`
```c
int32_t zm2_m_free(uint32_t track)
```
* 説明: m_free
* 引数:
  * `track`: トラック番号
* 戻り値: 空き容量

### `zm2_m_play()`
```c
int32_t zm2_m_play(struct Zm2Tracks* tracks)
```
* 説明: m_play (指定トラック対象)
  * 全トラックを対象とする場合は`zm2_m_play_all()`を使う方が簡潔です。
* 引数:
  * `tracks`: 対象トラックを指定する`struct Zm2Tracks`へのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_m_play_all()`
```c
int32_t zm2_m_play_all(void)
```
* 説明: m_play (全トラック対象)
* 戻り値: Z-MUSICエラーコード

### `zm2_m_stop()`
```c
int32_t zm2_m_stop(struct Zm2Tracks* tracks)
```
* 説明: m_stop (指定トラック対象)
  * 全トラックを対象とする場合は`zm2_m_stop_all()`を使う方が簡潔です。
* 引数:
  * `tracks`: 対象トラックを指定する`struct Zm2Tracks`へのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_m_stop_all()`
```c
int32_t zm2_m_stop_all(void)
```
* 説明: m_stop (全トラック対象)
* 戻り値: Z-MUSICエラーコード

### `zm2_m_cont()`
```c
int32_t zm2_m_cont(struct Zm2Tracks* tracks)
```
* 説明: m_cont (指定トラック対象)
  * 全トラックを対象とする場合は`zm2_m_cont_all()`を使う方が簡潔です。
* 引数:
  * `tracks`: 対象トラックを指定する`struct Zm2Tracks`へのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_m_cont_all()`
```c
int32_t zm2_m_cont_all(void)
```
* 説明: m_cont (全トラック対象)
* 戻り値: Z-MUSICエラーコード

### `zm2_m_stat()`
```c
int32_t zm2_m_stat(uint32_t channels)
```
* 説明: m_stat
* 引数:
  * `channels`: チャネルビットマスク
* 戻り値: Z-MUSICエラーコード

### `zm2_m_atoi()`
```c
void* zm2_m_atoi(uint32_t track)
```
* 説明: m_atoi
* 引数:
  * `track`: トラック番号
* 戻り値: トラックバッファ先頭へのポインタ

### `zm2_init_all()`
```c
void zm2_init_all(void)
```
* 説明: init_all

### `zm2_int_stop()`
```c
void zm2_int_stop(void)
```
* 説明: int_stop

### `zm2_m_play2()`
```c
int32_t zm2_m_play2(void)
```
* 説明: m_play2
* 戻り値: Z-MUSICエラーコード

### `zm2_adpcm_read()`
```c
int32_t zm2_adpcm_read(const struct Zm2AdpcmConf* conf, const char* filename)
```
* 説明: adpcm_read
  * 引数が多いため、ファイル名以外の引数は構造体に入れてポインタで渡します。
  * Z-MUSICの不具合により、エラー発生時に引数の`struct Zm2AdpcmConf`の`cut_size`メンバー
    の値(`d5`レジスタにロードされる値)と同じ数値のファイルハンドルがクローズされます。
* 引数:
  * `conf`: `struct Zm2AdpcmConf`へのポインタ
  * `filename`: ファイル名文字列へのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_play_cnv_data()`
```c
int32_t zm2_play_cnv_data(uint32_t size, const uint8_t* zmd)
```
* 説明: play_cnv_data。指定したデータをZ-MUSICの内部バッファに転送してから再生します。
  * 内部バッファに転送せず即演奏(高速応答)する場合は`zm2_play_cnv_data_fast()`を使います。
* 引数:
  * `size`: データサイズ
  * `zmd`: ZMDデータへのポインタ(ファイルヘッダを飛ばしたアドレス、奇数アドレス)
* 戻り値: Z-MUSICエラーコード

### `zm2_play_cnv_data_fast()`
```c
int32_t zm2_play_cnv_data_fast(const uint8_t* zmd)
```
* 説明: play_cnv_data。Z-MUSICの内部バッファに転送せず即演奏(高速応答)します。
* 引数:
  * `zmd`: ZMDデータへのポインタ(ファイルヘッダを飛ばしたアドレス)
* 戻り値: Z-MUSICエラーコード

### `zm2_se_play()`
```c
void zm2_se_play(uint32_t track, const uint8_t* zmd)
```
* 説明: se_play
  * Z-MUSICでは曲データが演奏されていないときに演奏用のタイマーが停止していることが
    あります。Z-MUSICの不具合により、`se_play`ファンクションを呼び出しても停止中の
    タイマーは開始されず効果音が演奏されません。
    そのような場合は事前にタイマーを開始しておくことで、曲が演奏中でなくても効果音を
    演奏させることができます。以下の関数を呼ぶことによりタイマーを開始できます。
    * `zm2_m_tempo_set()`
    * `zm2_m_play()`、`zm2_m_play_all()` (曲データが未指定でも有効、contとplay2も同じ)
    * `zm2_m_cont()`、`zm2_m_cont_all()`
    * `zm2_m_play2()`
    * `zm2_play_cnv_data()`、`zm2_play_cnv_data_fast()` (いずれも成功した場合のみ)
    * `zm2_set_timer_value()`
* 引数:
  * `track`: トラック番号
  * `zmd`: ZMDデータへのポインタ(ファイルヘッダと共通コマンドを飛ばしたアドレス、偶数アドレス)

### `zm2_se_adpcm1()`
```c
void zm2_se_adpcm1(uint32_t size, uint8_t priority, uint8_t freq, uint8_t pan,
                   const uint8_t* pcm)
```
* 説明: se_adpcm1
* 引数:
  * `size`: データサイズ
  * `priority`: 優先度
  * `freq`: 周波数
  * `pan`: パンポット
  * `pcm`: ADPCMデータへのポインタ

### `zm2_se_adpcm2()`
```c
void zm2_se_adpcm2(uint32_t note, uint8_t priority, uint8_t freq, uint8_t pan)
```
* 説明: se_adpcm2
* 引数:
  * `note`: ノート番号
  * `priority`: 優先度
  * `freq`: 周波数
  * `pan`: パンポット

### `zm2_set_ch_mode()`
```c
void zm2_set_ch_mode(uint32_t mode)
```
* 説明: set_ch_mode
* 引数:
  * `mode`: モード

### `zm2_calc_total()`
```c
int32_t zm2_calc_total(uint32_t mode)
```
* 説明: calc_total
* 引数:
  * `mode`: 表示モード
* 戻り値: エラーコード(0:正常終了、-1:計算不可、-2:演奏データなし)

### `zm2_fade_out()`
```c
int32_t zm2_fade_out(int32_t speed)
```
* 説明: fade_out
* 引数:
  * `speed`: 音量の増減スピードまたは0
* 戻り値: エラーコード(0:成功、-1:失敗)

### `zm2_m_vset2()`
```c
int32_t zm2_m_vset2(uint32_t tone, const Zm2FmTone* data)
```
* 説明: m_vset2
* 引数:
  * `tone`: 音色番号
  * `data`: 55バイトのFM音色データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_adpcm_block_data()`
```c
int32_t zm2_adpcm_block_data(const char* filename)
```
* 説明: adpcm_block_data
  * block_adpcm_dataという名称だと「ADPCMデータをブロックする機能」に見えるため
    語順を変えています。
* 引数:
  * `filename`: ファイル名文字列へのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_get_trk_tbl()`
```c
struct Zm2TrkTblResult zm2_get_trk_tbl(void)
```
* 説明: get_trk_tbl
  * 戻り値の各ポインタが指す先はスーパーバイザ領域の可能性があります。
* 戻り値: `struct Zm2TrkTblResult`
  * `real_ch_tbl`: 絶対チャンネルテーブルへのポインタ
  * `play_trk_tbl`: 演奏トラックテーブルへのポインタ

### `zm2_loop_time_set()`
```c
void zm2_loop_time_set(uint8_t count, void* address)
```
* 説明: set_loop_time (サブルーチンの設定)
  * 設定と解除で引数が異なるため関数も分かれています。
  * 設定したサブルーチンの解除には`zm2_loop_time_cancel()`を使います。
* 引数:
  * `count`: ループ回数
  * `address`: サブルーチンへのポインタ

### `zm2_loop_time_cancel()`
```c
void zm2_loop_time_cancel(void)
```
* 説明: set_loop_time (登録済みのサブルーチンの解除)
  * 設定と解除で引数が異なるため関数も分かれています。
  * `zm2_loop_time_set()`で登録したサブルーチンを解除します。

### `zm2_get_play_work()`
```c
struct Zm2PlayWorkResult zm2_get_play_work(uint8_t track)
```
* 説明: get_play_work
  * 戻り値の各ポインタの内容を利用する場合はlibzm2work.hをインクルードして下さい。
    ただし`attribute((packed))`を使用する都合上、elf2x68k専用です。
  * 戻り値の各ポインタが指す先はスーパーバイザ領域の可能性があります。
* 引数:
  * `track`: トラック番号
* 戻り値: `struct Zm2PlayWorkResult`
  * `cnv_wk`: コンパイルワークへのポインタ
  * `seq_wk`: 演奏ワークへのポインタ。

### `zm2_get_timer_mode()`
```c
int32_t zm2_get_timer_mode(void)
```
* 説明: get_timer_mode
* 戻り値: タイマー種別(`ZM2_TIMER_MODE_A`、`ZM2_TIMER_MODE_B`)

### `zm2_set_fm_master_vol()`
```c
int32_t zm2_set_fm_master_vol(uint8_t volume)
```
* 説明: set_fm_master_vol
* 引数:
  * `volume`: ボリューム値
* 戻り値: Z-MUSICエラーコード

### `zm2_set_timer_value()`
```c
int32_t zm2_set_timer_value(uint32_t timer)
```
* 説明: set_timer_value
* 引数:
  * `timer`: タイマー値
* 戻り値: Z-MUSICエラーコード

### `zm2_release_support_set()`
```c
int32_t zm2_release_support_set(const char* filename)
```
* 説明: release_support (登録)
  * 設定と解除で引数が異なるため関数も分かれています。
  * 設定したプログラムの解除には`zm2_release_support_cancel()`を使います。
* 引数:
  * `filename`: ファイル名文字列へのポインタ
* 戻り値: 0または正数で成功(登録番号)、-1でエラー

### `zm2_release_support_cancel()`
```c
int32_t zm2_release_support_cancel(int32_t code)
```
* 説明: release_support (解除)
  * 設定と解除で引数が異なるため関数も分かれています。
  * プログラムの設定には`zm2_release_support_set()`を使います。
* 引数:
  * `code`: 登録番号
* 戻り値: エラーコード(0で成功、-1でエラー)

### `zm2_jump_active()`
```c
void zm2_jump_active(uint8_t mode)
```
* 説明: jump_active
* 引数:
  * `mode`: モード(0:無効 0以外:有効)

### `zm2_set_mclk()`
```c
int32_t zm2_set_mclk(uint32_t count)
```
* 説明: set_mclk
* 引数:
  * `count`: 絶対音調
* 戻り値: Z-MUSICエラーコード

### `zm2_picture_sync()`
```c
int32_t zm2_picture_sync(uint32_t mode)
```
* 説明: picture_sync
* 引数:
  * `mode`: 同期モード(0:映像同期モードオフ 以外:オン)
* 戻り値: エラーコード(0:成功: -1:失敗)

### `zm2_mask_channels()`
```c
int32_t zm2_mask_channels(uint32_t channels)
```
* 説明: mask_channels
* 引数:
  * `channels`: マスク対象のチャンネルのビットマスク指定
* 戻り値: Z-MUSICエラーコード

### `zm2_buffer_info()`
```c
struct Zm2BufferInfo* zm2_buffer_info(void)
```
* 説明: buffer_info
  * 戻り値のポインタが指す先はスーパーバイザ領域の可能性があります。
* 戻り値: `struct Zm2BufferInfo`へのポインタ

### `zm2_set_zpd_tbl()`
```c
int32_t zm2_set_zpd_tbl(const void* zpd)
```
* 説明: set_zpd_tbl
* 引数:
  * `zpd`: ZPDデータへのポインタ(ファイルヘッダを飛ばしたアドレス、偶数アドレス)
* 戻り値: Z-MUSICエラーコード

### `zm2_set_output_level()`
```c
int32_t zm2_set_output_level(uint32_t channels, int8_t level)
```
* 説明: set_output_level
* 引数:
  * `channels`: チャンネルのビットマスク指定
  * `level`: 出力レベル
* 戻り値: Z-MUSICエラーコード

### `zm2_set_wave_form1()`
```c
int32_t zm2_set_wave_form1(uint32_t size, uint8_t no, uint8_t type,
                            uint16_t point, const uint8_t* wave)
```
* 説明: set_wave_form1
* 引数:
  * `size`: 波形データのサイズ(バイト数)
  * `no`: 波形番号
  * `type`: ループタイプ
  * `point`: ループポイント
  * `wave`: 波形データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_set_wave_form2()`
```c
int32_t zm2_set_wave_form2(uint32_t size, uint8_t no, uint8_t type,
                            uint16_t point, const uint8_t* wave)
```
* 説明: set_wave_form2
* 引数:
  * `size`: 波形データのサイズ(ワード数)
  * `no`: 波形番号
  * `type`: ループタイプ
  * `point`: ループポイント
  * `wave`: 波形データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_mask_tracks()`
```c
void zm2_mask_tracks(uint32_t track)
```
* 説明: mask_tracks
* 引数:
  * `track`: 0:全トラックマスク解除、1～80:マスク解除、-1～-80:マスク設定

### `zm2_set_output_level2()`
```c
void zm2_set_output_level2(uint32_t track, int8_t level)
```
* 説明: set_output_level2
* 引数:
  * `track`: トラック番号
  * `level`: 出力レベル

### `zm2_get_loop_time()`
```c
int32_t zm2_get_loop_time(void)
```
* 説明: get_loop_time
* 戻り値: 現在の繰り返し回数(負数なら演奏終了または停止中)

### `zm2_get_1st_comment()`
```c
const char* zm2_get_1st_comment(void)
```
* 説明: get_1st_comment
  * 戻り値のポインタが指す先はスーパーバイザ領域の可能性があります。
* 戻り値: 文字列へのポインタ

### `zm2_int_start()`
```c
void zm2_int_start(void)
```
* 説明: int_start

### `zm2_zm_status()`
```c
void* zm2_zm_status(void)
```
* 説明: zm_status
  * 戻り値のポインタが指す先はスーパーバイザ領域の可能性があります。
* 戻り値: Z-MUSICの常駐状況の情報テーブルへのポインタ

### `zm2_get_status()`
```c
struct Zm2Status* zm2_get_status(void)
```
* 説明: Z-MUSICの`zm_status`ファンクションコールで得た生のポインタを
  `struct Zm2Status`へのポインタに変換して返します。
  * この関数はZ-MUSICの情報テーブルを操作するためのユーティリティ関数です。
  * 戻り値のポインタが指す先はスーパーバイザ領域の可能性があります。
* 戻り値: `struct Zm2Status`へのポインタ

### `zm2_relative_uv()`
```c
void zm2_relative_uv(uint32_t mode)
```
* 説明: relative_uv
* 引数:
  * `mode`: モード(0:相対音量 1:相対ベロシティ)

### `zm2_intercept_play()`
```c
void zm2_intercept_play(int32_t mode)
```
* 説明: intercept_play
* 引数:
  * `mode`: モード(-1:設定、0:解除して演奏開始、1:解除)

### `zm2_occupied_size()`
```c
void zm2_occupied_size(struct Zm2OccupiedSize* buf)
```
* 説明: occupied_size
* 引数:
  * `buf`: `struct Zm2OccupiedSize`へのポインタ
    * `track_usage`: トラックバッファの使用容量
    * `adpcm_usage`: ADPCMバッファの使用容量
    * `work_usage`: 汎用ワークエリアの使用容量

### `zm2_call_int_play_ope()`
```c
void zm2_call_int_play_ope(struct Zm2SeqWk* seq_wk)
```
* 説明: call_int_play_ope
* 引数:
  * `seq_wk`: 演奏トラックワークへのポインタ


--------

## libzm2midi.h - MIDI関連関数

### `zm2_midi_rec()`
```c
void zm2_midi_rec(void)
```
* 説明: midi_rec

### `zm2_midi_rec_end()`
```c
struct Zm2MidiRecResult zm2_midi_rec_end(uint32_t mode)
```
* 説明: midi_rec_end
* 引数:
  * `mode`: データ加工モード
* 戻り値: `struct Zm2MidiRecResult`
  * `size`: データサイズ
  * `data`: データへのポインタ

### `zm2_midi_trns()`
```c
int32_t zm2_midi_trns(uint32_t size, const uint8_t* data)
```
* 説明: midi_trns
* 引数:
  * `size`: データサイズ(0の場合はアスキー文字列モード)
  * `data`: MIDIデータへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_send_rd_exc()`
```c
int32_t zm2_send_rd_exc(uint32_t size, uint16_t devid, uint16_t modelid,
                        const uint8_t* data)
```
* 説明: send_rd_exc
* 引数:
  * `size`: データサイズ
  * `devid`: デバイスID
  * `modelid`: モデルID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_send_exc()`
```c
int32_t zm2_send_exc(uint32_t size, const uint8_t* data)
```
* 説明: send_exc
* 引数:
  * `size`: データサイズ
  * `data`: エクスクルーシブデータへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_sc55_p_rsv()`
```c
int32_t zm2_sc55_p_rsv(uint32_t size, uint8_t devid, const uint8_t* data)
```
* 説明: sc55_p_rsv
* 引数:
  * `size`: データサイズ
  * `devid`: デバイスID
  * `data`: パラメータデータへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_sc55_reverb()`
```c
int32_t zm2_sc55_reverb(uint32_t size, uint8_t devid, const uint8_t* data)
```
* 説明: sc55_reverb
* 引数:
  * `size`: データサイズ
  * `devid`: デバイスID
  * `data`: パラメータデータへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_sc55_chorus()`
```c
int32_t zm2_sc55_chorus(uint32_t size, uint8_t devid, const uint8_t* data)
```
* 説明: sc55_chorus
* 引数:
  * `size`: データサイズ
  * `devid`: デバイスID
  * `data`: パラメータデータへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_sc55_part_setup()`
```c
int32_t zm2_sc55_part_setup(uint32_t size, uint16_t part, uint16_t devid,
                             const uint8_t* data)
```
* 説明: sc55_part_setup
* 引数:
  * `size`: データサイズ
  * `part`: パート番号
  * `devid`: デバイスID
  * `data`: パラメータデータへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_sc55_drum_setup()`
```c
int32_t zm2_sc55_drum_setup(uint32_t size, uint8_t map, uint8_t note,
                             uint16_t devid, const uint8_t* data)
```
* 説明: sc55_drum_setup
* 引数:
  * `size`: データサイズ
  * `map`: マップ番号
  * `note`: ノート番号
  * `devid`: デバイスID
  * `data`: パラメータデータへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_sc55_print()`
```c
int32_t zm2_sc55_print(uint32_t size, uint8_t devid, const char* message)
```
* 説明: sc55_print
* 引数:
  * `size`: 文字列長
  * `devid`: デバイスID
  * `message`: 文字列へのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_sc55_display()`
```c
int32_t zm2_sc55_display(uint8_t devid, const Zm2Sc55Display* pattern)
```
* 説明: sc55_display
* 引数:
  * `devid`: デバイスID
  * `pattern`: データへのポインタ(必ず偶数であること)
* 戻り値: Z-MUSICエラーコード

### `zm2_mt32_p_rsv()`
```c
int32_t zm2_mt32_p_rsv(uint32_t size, uint8_t devid, const uint8_t* data)
```
* 説明: mt32_p_rsv
* 引数:
  * `size`: データサイズ
  * `devid`: デバイスID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_mt32_reverb()`
```c
int32_t zm2_mt32_reverb(uint32_t size, uint8_t devid, const uint8_t* data)
```
* 説明: mt32_reverb
* 引数:
  * `size`: データサイズ
  * `devid`: デバイスID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_mt32_setup()`
```c
int32_t zm2_mt32_setup(uint32_t size, uint8_t devid, const uint8_t* data)
```
* 説明: mt32_setup
* 引数:
  * `size`: データサイズ
  * `devid`: デバイスID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_mt32_drum()`
```c
int32_t zm2_mt32_drum(uint32_t size, uint16_t note, uint16_t devid,
                      const uint8_t* data)
```
* 説明: mt32_drum
* 引数:
  * `size`: データサイズ
  * `note`: ノート番号
  * `devid`: デバイスID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_mt32_common()`
```c
int32_t zm2_mt32_common(uint32_t size, uint16_t prog, uint16_t devid,
                        const uint8_t* data)
```
* 説明: mt32_common
* 引数:
  * `size`: データサイズ
  * `prog`: プログラム番号
  * `devid`: デバイスID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_mt32_partial()`
```c
int32_t zm2_mt32_partial(uint32_t size, uint8_t prog, uint8_t part,
                         uint16_t devid, const uint8_t* data)
```
* 説明: mt32_partial
* 引数:
  * `size`: データサイズ
  * `prog`: プログラム番号
  * `part`: パーシャル番号
  * `devid`: デバイスID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_mt32_patch()`
```c
int32_t zm2_mt32_patch(uint32_t size, uint16_t patch, uint16_t devid,
                       const uint8_t* data)
```
* 説明: mt32_patch
* 引数:
  * `size`: データサイズ
  * `patch`: パッチ番号
  * `devid`: デバイスID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_mt32_print()`
```c
int32_t zm2_mt32_print(uint32_t size, uint8_t devid, const char* message)
```
* 説明: mt32_print
* 引数:
  * `size`: 文字列長
  * `devid`: デバイスID
  * `message`: 文字列へのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_u220_setup()`
```c
int32_t zm2_u220_setup(uint32_t size, uint16_t devid, const uint8_t* data)
```
* 説明: u220_setup
* 引数:
  * `size`: データサイズ
  * `devid`: デバイスID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_u220_common()`
```c
int32_t zm2_u220_common(uint32_t size, uint16_t devid, const uint8_t* data)
```
* 説明: u220_common
* 引数:
  * `size`: データサイズ
  * `devid`: デバイスID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_u220_d_setup()`
```c
int32_t zm2_u220_d_setup(uint32_t size, uint16_t devid, const uint8_t* data)
```
* 説明: u220_d_setup
* 引数:
  * `size`: データサイズ
  * `devid`: デバイスID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_u220_p_setup()`
```c
int32_t zm2_u220_p_setup(uint32_t size, uint16_t part, uint16_t devid,
                         const uint8_t* data)
```
* 説明: u220_p_setup
* 引数:
  * `size`: データサイズ
  * `part`: パート番号
  * `devid`: デバイスID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_u220_print()`
```c
int32_t zm2_u220_print(uint32_t size, uint8_t devid, const char* message)
```
* 説明: u220_print
* 引数:
  * `size`: 文字列長
  * `devid`: デバイスID
  * `message`: 文字列へのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_u220_timbre()`
```c
int32_t zm2_u220_timbre(uint32_t size, uint16_t prog, uint16_t devid,
                        const uint8_t* data)
```
* 説明: u220_timbre
* 引数:
  * `size`: データサイズ
  * `prog`: プログラム番号
  * `devid`: デバイスID
  * `data`: 設定データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_u220_drum()`
```c
int32_t zm2_u220_drum(uint32_t size, uint16_t note, uint16_t devid,
                      const uint8_t* data)
```
* 説明: u220_drum
* 引数:
  * `size`: データサイズ
  * `note`: ノート番号
  * `devid`: デバイスID
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_m1_midi_ch()`
```c
int32_t zm2_m1_midi_ch(const Zm2M1MidiCh* data)
```
* 説明: m1_midi_ch
* 引数:
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_send_to_m1()`
```c
int32_t zm2_send_to_m1(uint8_t devid)
```
* 説明: send_to_m1
* 引数:
  * `devid`: デバイスID
* 戻り値: Z-MUSICエラーコード

### `zm2_m1_p_setup()`
```c
int32_t zm2_m1_p_setup(const Zm2M1TrackParam* data)
```
* 説明: m1_p_setup
* 引数:
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_m1_e_setup()`
```c
int32_t zm2_m1_e_setup(const Zm2M1EffectParam* data)
```
* 説明: m1_e_setup
* 引数:
  * `data`: データへのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_m1_print()`
```c
int32_t zm2_m1_print(uint32_t size, const char* message)
```
* 説明: m1_print
* 引数:
  * `size`: 文字列長
  * `message`: 文字列へのポインタ
* 戻り値: Z-MUSICエラーコード

### `zm2_eox_wait()`
```c
int32_t zm2_eox_wait(uint16_t wait)
```
* 説明: eox_wait
* 引数:
  * `wait`: 待機時間
* 戻り値: Z-MUSICエラーコード

### `zm2_sc55_init()`
```c
int32_t zm2_sc55_init(uint8_t devid)
```
* 説明: sc55_init
* 引数:
  * `devid`: デバイスID
* 戻り値: Z-MUSICエラーコード

### `zm2_mt32_init()`
```c
int32_t zm2_mt32_init(uint8_t devid)
```
* 説明: mt32_init
* 引数:
  * `devid`: デバイスID
* 戻り値: Z-MUSICエラーコード

### `zm2_m_inp1()`
```c
int32_t zm2_m_inp1(uint32_t wait)
```
* 説明: m_inp1
* 引数:
  * `wait`: 待機時間
* 戻り値: 0～255なら入力データ、負数ならエラー

### `zm2_m_out1()`
```c
int32_t zm2_m_out1(uint8_t data)
```
* 説明: m_out1
* 引数:
  * `data`: 出力データ
* 戻り値: Z-MUSICエラーコード

--------

## libzm2util.h - ユーティリティ関数

### `zm2_tracks_set()`
```c
int zm2_tracks_set(struct Zm2Tracks* tracks, uint32_t track)
```
* 説明: 指定したトラック番号のビットを`1`にします。
  * この関数は`struct Zm2Tracks`を操作するためのユーティリティ関数です。
* 引数:
  * `tracks`: `struct Zm2Tracks`へのポインタ
  * `track`: トラック番号 (1-80)
* 戻り値: 成功時は0、失敗時は-1

### `zm2_tracks_clear()`
```c
int zm2_tracks_clear(struct Zm2Tracks* tracks, uint32_t track)
```
* 説明: 指定したトラック番号のビットを`1`にします。
  * この関数は`struct Zm2Tracks`を操作するためのユーティリティ関数です。
* 引数:
  * `tracks`: `struct Zm2Tracks`へのポインタ
  * `track`: トラック番号 (1-80)
* 戻り値: 成功時は0、失敗時は-1

### `zm2_tracks_isset()`
```c
int zm2_tracks_isset(struct Zm2Tracks* tracks, uint32_t track, int* isset)
```
* 説明: 指定されたトラック番号のビットが`1`かどうかを調べます。
  * この関数は`struct Zm2Tracks`を操作するためのユーティリティ関数です。
* 引数:
  * `tracks`: `struct Zm2Tracks`へのポインタ
  * `track`: トラック番号 (1-80)
  * `isset`: 結果を格納するポインタ (0または1)
* 戻り値: 成功時は0、失敗時は-1

### `zm2_is_zmd_data()`
```c
int zm2_is_zmd_data(const uint8_t* buf, size_t filesize)
```
* 説明: 指定されたバッファ上のデータがZMD形式かどうかを調べます。
* 引数:
  * `buf`: バッファへのポインタ(偶数アドレス)
  * `filesize`: データサイズ

### `zm2_is_zpd_data()`
```c
int zm2_is_zpd_data(const uint8_t* buf, size_t filesize)
```
* 説明: 指定されたバッファ上のデータがZPD形式かどうかを調べます。
* 引数:
  * `buf`: バッファへのポインタ(偶数アドレス)
  * `filesize`: データサイズ

### `zm2_get_zmd_common_size()`
```c
int32_t zm2_get_zmd_common_size(const uint8_t* buf, size_t filesize)
```
* 説明: 指定されたバッファ上のZMDデータの、ヘッダと共通コマンド群のバイト数を調べます。
* 引数:
  * `buf`: バッファへのポインタ(偶数アドレス)
  * `filesize`: データサイズ
* 戻り値: 正数ならバイト数、正しいZMDデータでない場合は-1

--------
