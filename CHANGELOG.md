# 変更履歴

## 2.0.0 (2026-07-16)
* MITライセンスに変更。


## 1.1.0 (2026-06-21)

### libzm2internal.h
* C23以外でも`nullptr`を使用していた不具合を修正(`NULL`に変更)。

### zm2call
* se_playコマンドで.zmdファイルからヘッダと共通コマンド群を削除したデータの再生に対応。
* elf2x68kで`-ldos`オプションを指定するせいでビルドが失敗する不具合を修正。


## 1.0.0 (2026-05-24)
* 正式リリース。


## 0.3.0 (preview)

### libzm2midi.h
* 定数`ZM2_USE_PREVIOUS_DEVID`を追加。
* 各関数の引数の`devid`の型を`int8_t devid`に変更(-1で前回の値を使用する仕様のため)。
* `zm2_send_rd_exc()`: `devid`を正しく渡せない不具合を修正(Z-MUSICマニュアルの誤記による)。
* `zm2_sc55_part_setup()`: 引数`part`の型を`uint8_t`に変更。
* `zm2_mt32_drum()`: 引数`note`の型を`uint8_t`に変更。
* `zm2_mt32_common()`: 引数`prog`の型を`uint8_t`に変更。
* `zm2_mt32_patch()` : 引数`patch`の型を`uint8_t`に変更。
* `zm2_u220_p_setup()`: 引数`part`の型を`uint8_t`に変更。
* `zm2_u220_timbre()`: 引数`prog`の型を`uint8_t`に変更。
* `zm2_u220_drum()`: 引数`note`の型を`uint8_t`に変更。

### libzm2util.h
* `zm2_is_zmd_data()`、`zm2_is_zpd_data()`、`zm2_get_zmd_common_size()`を追加。

### zm2call
* 未実装だったMIDI制御系のコマンドをすべて追加。


## 0.2.0 (preview)

### FUNCTIONS.md
* [関数リファレンスマニュアル](FUNCTIONS.md)を追加。

### libzm2work.h
* 各種ワークの構造を定義するヘッダファイルを追加。

### libzm2.h
* `zm2_adpcm_read()`の`conf`引数に`const`を追加。
* `struct Zm2Status* zm2_get_status(void)`を追加。
* 関数の戻り値や引数の型をlibzm2work.hで定義する構造体に変更。

### libzm2midi.h
* `zm2_sc55_init()`、`zm2_mt32_init()`の引数`devid`の型を`int8_t`に変更
  (-1で前回の値を使用する仕様のため)。

### zm2call
* Z-MUSICをデバイスドライバとして組み込んでいると、get_trk_tblとget_1st_comment
  でバスエラーが発生する不具合を修正。
* 自分自身のPSPを求める方法を、ランタイムの`_PSTA`変数の参照から`_dos_getpdb()`に変更。
* status、sc55_init、mt32_initコマンドを追加。


## 0.1.0 (preview)

* 新規作成。
