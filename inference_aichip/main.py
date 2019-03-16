# -*- coding: utf-8 -*-
import cv2
import numpy as np
import subprocess
import serial
from time import sleep


ser = serial.Serial('/dev/ttyACM1', 9600, timeout=0.01)

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


def write_int(x):
    print("data is ", x)

    sleep(0.01)
    #print(hex((x) & 0xFF))
    ser.write((x & 0xFF).to_bytes(1, 'big'))
    sleep(0.005)
    #print(hex((x >> 8) & 0xFF))
    ser.write(((x >> 8) & 0xFF).to_bytes(1, 'big'))
    sleep(0.005)
    #print(hex((x >> 16) & 0xFF))
    ser.write(((x >> 16) & 0xFF).to_bytes(1, 'big'))

    sleep(0.05)
    line = ser.readline()
    print(line)


# 画像を読み込む
img = np.full((56, 56, 1), 255, dtype=np.uint8)
# ウィンドウの名前を設定
cv2.namedWindow("img", cv2.WINDOW_NORMAL)
# マウス操作のコールバック関数の設定
cv2.setMouseCallback("img", callback)

while(1):
    cv2.imshow("img", img)

    line = ser.readline()
    if(line != b""):
        print(line)

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

        line = ser.readline()
        print("clear")

        for j in range(28):
            for i in range(28):
                pixelValue = tmp[j, i]
                write_int(int(255 - pixelValue))
                # write_int(j*28+i)

                #argment = argment + str((255 - pixelValue)*2) + ", "

        # print(argment)
        #res = subprocess.call(argment.split())
        # => lsコマンドの結果
        # print(res)

ser.close()

#				argment = argment + str((255 - pixelValue[0])*2) + ', '

#	print(argment)
