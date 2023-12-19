# これは何？

Cafetalkで開設しているレッスン、「基礎から応用まで！はじめて学ぶC/C++」の C++ パートで開発するお絵書きアプリです。

# ビルドインストラクション

* vcpkg のインストールが必要です
* 以下の環境変数をセットしておいてください
  * CMAKE_TOOLCHAIN_FILE: `vcpkg.cmake` へのパス
  * VCPKG_TARGET_TRIPLET: 環境に応じたビルド設定(Windows: `x64-windows-static-md`, macOS: `arm64-osx`, など)

# 出典
wxWidgetsの利用に際してサンプルプログラムを改造しています。
詳細はLICENSE.txtをご覧ください。

# ライセンス
MIT. ただし、一部のコードはwxWidgetsのサンプルプログラムを改造している部分があります。
COPYING.md をご覧ください。
