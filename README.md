# libzm2

Z-MUSIC v2のファンクションコールを呼び出すためのC言語用ライブラリです。  

無保証につき各自の責任で使用して下さい。


## Features

* gcc、gcc2、elf2x68kに対応してします。
* ヘッダファイルだけで構成されているため、ライブラリファイルのリンクが不要です。
* インライン関数とインラインアセンブラにより実装されているため、サイズが小さく、
  高速に動作します。
* Z-MUSICのファンクションコールがほぼそのままC言語から使えるようになっており、
  Z-MUSICの機能をフルに活用できます。
* ファンクションコールのエラー時にファイルがクローズされてしまう不具合への対策が
  組み込まれています。
* このライブラリを使用して作成された実行ファイルに対する配布等の制限はありません。


## Usage

> [!IMPORTANT]
> C標準ライブラリにLIBCを使う場合、stdint.hの追加が必要です。  
> 手元にない場合は、とりあえずの対処として
> [ここのstdint.h](https://github.com/kg68k/cheaders/blob/main/libc_include/stdint.h)
> を使って下さい。

> [!IMPORTANT]
> Z-MUSICのファンクションコールについて理解しておく必要があります。Z-MUSICマニュアル、特に
> 『MEASURE10 ZMUSICのファンクションコール』を読んで下さい(一部誤記があるので注意して下さい)。

必要に応じてinclude/ディレクトリにあるヘッダファイルをインクルードして下さい。

* libzm2.h …… 基本のファイルです。
* libzm2const.h …… 各種の定数を定義したファイルです。
* libzm2error.h …… エラー番号を定義したファイルです。
* libzm2midi.h …… MIDI関連のファンクションコールを定義したファイルです。
* libzm2util.h …… ユーティリティ関数を定義したファイルです。
* libzm2work.h …… Z-MUSICのワークの構造を定義したファイルです。

[関数リファレンスマニュアル](FUNCTIONS.md)

ヘッダファイルはASCIIコードだけで書いてあるため、
どのビルド環境でも文字コード変換は不要でそのまま使えます。

libzm2work.hはGCCの拡張機能(匿名構造体や`__attribute__((__packed__))`)を使っているため、
elf2x68k専用です。

libzm2internal.hはライブラリ内部で使用するファイルなので直接のインクルードはしないで下さい。


## zm2call

src/ディレクトリにはサンプルプログラムとして、各ファンクションコールを呼び出すツールzm2call
のソースコードが置かれています。

### elf2x68k (m68k-xelf-gcc)によるビルド

```
mkdir build
cd build
make -f ../src/Makefile
```

### X680x0上でのビルド

PCやネット上での取り扱いを用意にするために、src/内のファイルはUTF-8で記述されています。
X680x0上でビルドする際には、UTF-8からShift_JISへの変換が必要です。

必要ツール: [src2build](https://github.com/kg68k/src2build)

srcディレクトリのある場所で以下のコマンドを実行します。
```
src2build src
make -C build
```

標準ではgccでコンパイルします。makeの引数に`CC=gcc2`を指定すればgcc2を使用します。

src2buildを使わない場合は、src/内のファイルを適当なツールでShift_JISに変換して
別のディレクトリに保存し、そのディレクトリ内で`make`を実行して下さい。


## License

GNU GENERAL PUBLIC LICENSE Version 3 or later.  
\+ GCC Runtime Library Exception


## Author

TcbnErik / https://github.com/kg68k/libzm2
