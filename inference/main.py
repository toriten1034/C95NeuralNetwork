# -*- coding: utf-8 -*-
import cv2
import numpy as np
import subprocess

#sx, syは線の始まりの位置
sx, sy = 0, 0
# ペンの太さ
thickness = 2
# マウスの操作があるとき呼ばれる関数


def callback(event, x, y, flags, param):
    global img, sx, sy, color, thickness
    # マウスの左ボタンがクリックされたとき
    if event == cv2.EVENT_LBUTTONDOWN:
        sx, sy = x, y
    # マウスの左ボタンがクリックされていて、マウスが動いたとき
    if flags == cv2.EVENT_FLAG_LBUTTON and event == cv2.EVENT_MOUSEMOVE:
        cv2.line(img, (sx, sy), (x, y), (10, 10, 10), 2)
        sx, sy = x, y


# 画像を読み込む
img = np.full((56, 56, 1), 255, dtype=np.uint8)
# ウィンドウの名前を設定
cv2.namedWindow("img", cv2.WINDOW_NORMAL)
# マウス操作のコールバック関数の設定
cv2.setMouseCallback("img", callback)

while(1):
    cv2.imshow("img", img)
    k = cv2.waitKey(1)
    # Escキーを押すと終了
    if k == 27:
        break
    # Cキーを押すとクリア
    if k == ord("c"):
        img = np.full((56, 56, 1), 255, dtype=np.uint8)

    # Eキーを押すと推論
    if k == ord("e"):
        blur = cv2.blur(img, (3, 3))
        tmp = cv2.resize(img, (28, 28))
        argment = './a.out '
        for j in range(28):
            for i in range(28):
                pixelValue = tmp[j, i]
                argment = argment + str((255 - pixelValue)) + ", "

        print(argment)
        res = subprocess.call(argment.split())
        # => lsコマンドの結果
        print(res)

#				argment = argment + str((255 - pixelValue[0])*2) + ', '

    #	print(argment)
