# libzm2

Z-MUSIC v2のファンクションコールを呼び出すためのC言語用ライブラリです。  

無保証につき各自の責任で使用して下さい。

現在はプレビュー版です。  
リファレンスマニュアルは用意できていないので、ヘッダファイルを見て下さい。


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
> [ここのstdint.h](https://github.com/kg68k/x68.memo/blob/main/prog/libc/include/stdint.h)
> を使って下さい。

必要に応じてinclude/ディレクトリにあるヘッダファイルをインクルードして下さい。

* libzm2.h …… 基本のファイルです。
* libzm2const.h …… 各種の定数を定義したファイルです。
* libzm2error.h …… エラー番号を定義したファイルです。
* libzm2midi.h …… MIDI関連のファンクションコールを定義したファイルです。

ヘッダファイルはASCIIコードだけで書いてあるため、
どのビルド環境でも文字コード変換は不要でそのまま使えます。

なお、libzm2internal.hはライブラリ内部で使用するファイルなので
直接のインクルードはしないで下さい。


## zm2call

src/ディレクトリにはサンプルプログラムzm2callのソースコードが置かれています。

各ファンクションコールを呼び出すツールですが、
MIDI関連の機能は今のところ一部しか実装していません。

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

標準ではgcc2でコンパイルします。makeの引数に`CC=gcc`を指定すればgccを使用します。

src2buildを使わない場合は、src/内のファイルを適当なツールでShift_JISに変換して
別のディレクトリに保存し、そのディレクトリ内で`make`を実行して下さい。


## License

GNU GENERAL PUBLIC LICENSE Version 3 or later.  
\+ GCC Runtime Library Exception


## Author
TcbnErik / https://github.com/kg68k/libzm2
