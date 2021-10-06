## Simple On Memory FileSystem

OSなし組み込み系向けのオンメモリタイプの簡易ファイルシステム

ファイルとディレクトリのオープン、リードが出来る。書き込みは出来ない。
ファイルシステムラッパーを作って、OSあり、OSなしどちらでも動くものを
作成する必要がある場合に便利。

#### サンプル実行手順

1. データファイル作成
  `create_fs.sh`を実行
  sampleディレクトリからオブジェクトとテーブルヘッダーを作成
  （sample_table.h, sample_data.o）
  `memfs.py`を使用。使い方はmemfs.pyと打てばUsageが出る
1. ビルド
  memfs、sample.cpp、及び作成したsample_data.oをリンク
    ```
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    ```
1. 実行
  ファイルリスト取得、幾つかのファイル検査、リードしたファイルを幾つか取り出すサンプルになっている
    ```
    $ ./sample
    ```
