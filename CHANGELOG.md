# 変更履歴

## Unreleased

### FUNCTIONS.md
* [関数リファレンスマニュアル](FUNCTIONS.md)を追加。

### libzm2work.h
* 各種ワークの構造を定義するヘッダファイルを追加。

### libzm2.h
* `zm2_adpcm_read()`の`conf`引数に`const`を追加。
* `struct Zm2Status* zm2_get_status(void)`を追加。
* 関数の戻り値や引数の型をlibzm2work.hで定義する構造体に変更。

### zm2call
* Z-MUSICをデバイスドライバとして組み込んでいると、get_trk_tblとget_1st_comment
  でバスエラーが発生する不具合を修正。
* 自分自身のPSPを求める方法を、ランタイムの`_PSTA`変数の参照から`_dos_getpdb()`に変更。
* statusコマンドを追加。


## 0.1.0 (preview)

* 新規作成。
