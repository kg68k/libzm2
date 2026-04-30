# 変更履歴

## Unreleased

### libzm2util.h
* `zm2_is_zmd_data()`、`zm2_is_zpd_data()`、`zm2_get_zmd_common_size()`を追加。


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
