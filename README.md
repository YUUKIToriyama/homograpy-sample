# homograpy-sample
ホモグラフィ変換のサンプルコード

## 概要
- [YUUKIToriyama/html-canvas-sample](https://github.com/YUUKIToriyama/html-canvas-sample)と組み合わせて使います
- GUIで決めた四点の座標のデータが格納されたJSONファイルを読み込み、それをもとにホモグラフィ変換を行ないます
	- 変換行列の計算はsympyを使い、
	- 画像処理はopenCVを使っています
- 将来的にはこのスクリプトをサーバーで動かし、Webアプリにしたいと考えています。

## 数学的なあれこれ

$$ (x,y)\longmapsto(u,v) $$ なる変換を考える。このとき変換前の座標系$(x,y)$と変換後の座標系$(u,v)$の関係は次のようになる。

$$ u = \frac{ax + by + c}{gx + hy + 1} $$
$$ v = \frac{dx + ey + f}{gx + hy + 1} $$

a,b,c,d,e,f,g,hは変換係数であり、
$$ \left(
\begin{matrix}
u \\
v \\
1
\end{matrix}
\right) = 
\left(
\begin{matrix}
a & b & c \\
d & e & f \\
g & h & 1
\end{matrix}
\right)
\left(
\begin{matrix}
x \\
y \\
1
\end{matrix}
\right)
$$
の右辺の3x3行列のことである。

$(x,y)$ $(u,v)$の組4つが決まれば変換行列$\left(
\begin{matrix}
a & b & c \\
d & e & f \\
g & h & 1
\end{matrix}
\right)$は一意に決まるので、これを計算する。まず、分数になっていると都合が悪いので両辺に$gx + hy + 1$を掛けて移項し次のようにする。

$$ ax + by + c - u(gx + hy + 1) = 0 $$
$$ dx + ey + f - v(gx + hy + 1) = 0 $$

これに$(x,y)$、$(u,v)$の値を入れてa,b,c,d,e,f,g,hについての連立方程式を作って解くのだが、ここは`main.py`で言えば33行目あたり、
```python:main.py
equations = []
for i in range(4):
    coords = coordinates[i]
    equations.append(a * coords["x"] + b * coords["y"] + c - coords["u"] * (g * coords["x"] + h * coords["y"] + 1))
    equations.append(d * coords["x"] + e * coords["y"] + f - coords["v"] * (g * coords["x"] + h * coords["y"] + 1))
```
が対応している。

これを代数演算ライブラリのsympyを使って処理すると変換係数が求められる。
```python:main.py
import sympy as sym

result = sym.solve(equations)
```

これをもとに変換行列をつくって、openCVの函数`cv2.warpPerspective`に用いると、うまいこと処理をやってくれる。
