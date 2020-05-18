#!/usr/bin/python
# ホモグラフィ変換
# sympyを使って連立方程式を解き、その解を用いてopenCVでホモグラフィ変換を行なう
#
# Copyright 2020 YUUKIToriyama

import cv2
import sympy as sym
import numpy as np
import json
import math

# Webページから送られてきたJSONファイルの読み込み
tmp = open("test.json", "r")
json = json.load(tmp)

ab = math.floor(np.sqrt((json[0]["x"] - json[1]["x"])**2 + (json[0]["y"] - json[1]["y"])**2))
bc = math.floor(np.sqrt((json[1]["x"] - json[2]["x"])**2 + (json[1]["y"] - json[2]["y"])**2))
print((ab,bc))

json[0].update({"u": 0, "v": 0})
json[1].update({"u": ab, "v": 0})
json[2].update({"u": ab, "v": bc})
json[3].update({"u": 0, "v": bc})



# 連立方程式を解いて変換行列を求める
a,b,c,d,e,f,g,h = sym.symbols("a b c d e f g h")

"""
coordinates = [
    {"x": 750, "y": 717, "u": 0, "v": 0},
    {"x": 1573, "y": 325, "u": 500, "v": 0},
    {"x": 2578, "y": 1481, "u": 500, "v": 500},
    {"x": 1664, "y": 2140, "u": 0, "v": 500}
]
"""
coordinates = json
equations = []

for i in range(4):
    coords = coordinates[i]
    equations.append(a * coords["x"] + b * coords["y"] + c - coords["u"] * (g * coords["x"] + h * coords["y"] + 1))
    equations.append(d * coords["x"] + e * coords["y"] + f - coords["v"] * (g * coords["x"] + h * coords["y"] + 1))


result = sym.solve(equations)

matrix = np.array([
    [result[a], result[b], result[c]],
    [result[d], result[e], result[f]],
    [result[g], result[h], 1]
], dtype=np.float32)



# 画像を読み込み変形させる
image = cv2.imread("tmp/IMG_4081.JPG")
new_image = cv2.warpPerspective(image, matrix, (ab,bc))

cv2.imwrite("tmp/output.png", new_image)
