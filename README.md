# homograpy-sample
ホモグラフィ変換のサンプルコード

## 概要
- [YUUKIToriyama/html-canvas-sample](https://github.com/YUUKIToriyama/html-canvas-sample)と組み合わせて使います
- GUIで決めた四点の座標のデータが格納されたJSONファイルを読み込み、それをもとにホモグラフィ変換を行ないます
	- 変換行列の計算はsympyを使い、
	- 画像処理はopenCVを使っています
- 将来的にはこのスクリプトをサーバーで動かし、Webアプリにしたいと考えています。

## 数学的なあれこれ
