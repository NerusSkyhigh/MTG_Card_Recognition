import cv2
import numpy as np


def preprocessing(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Blur allows for an easier detection of the image
    # as the contours are more likely to be closed.
    blur = cv2.GaussianBlur(gray,(1,1),0)
    canny = cv2.Canny(blur, 120, 255, 1)

    cv2.imshow('Photo', blur)
    cv2.waitKey()

    return canny



def maxAreaRectangle(image):
    contours, hierarchy = cv2.findContours(image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # Sometimes opencv finds multiple rectangles.
    # I assume that the card is the contours that encloses the biggest area.
    maxArea = -1
    cMax = None

    test = image.copy()
    cv2.drawContours(test, contours, -1, (255,255,255), 1)
    cv2.imshow('TUTTI I CONTORNI', test)
    cv2.waitKey()

    for c in contours:
        rect_center, rect_shape, rect_angle = cv2.minAreaRect(c)

        ratio = max(rect_shape)/(min(rect_shape)+1e-9)

        if (rect_shape[0]*rect_shape[1] > maxArea) and (ratio < 1.5 and ratio > 1.3):
            cMax = c
            maxArea = rect_shape[0]*rect_shape[1]


    test = image.copy()
    cv2.drawContours(test, [cMax], 0, (255,255,255), 3)
    cv2.imshow('cMax', test)
    cv2.waitKey()

    return cMax


def rotate(image, rect_center, rect_angle):
    rot_mat = cv2.getRotationMatrix2D(rect_center, rect_angle, 1.0)
    result = cv2.warpAffine(image, rot_mat, image.shape[1::-1], flags=cv2.INTER_LINEAR)
    return result






image = cv2.imread("frame.png")
fgMask = cv2.imread("fgMask.png")

# Preprocessing of the image
canny = preprocessing(fgMask)

cMax = maxAreaRectangle(canny)

rect_center, rect_shape, rect_angle = cv2.minAreaRect(cMax)


image = rotate(image, rect_center, rect_angle)

# Coordinates of the vertices
maxX = np.int0(rect_center[1]+rect_shape[1]/2)
minX = np.int0(rect_center[1]-rect_shape[1]/2)
maxY = np.int0(rect_center[0]+rect_shape[0]/2)
minY = np.int0(rect_center[0]-rect_shape[0]/2)


if maxX-minX < maxY-minY:
    image = rotate(image, rect_center, -90)

    maxX = np.int0(rect_center[1]+rect_shape[0]/2)
    minX = np.int0(rect_center[1]-rect_shape[0]/2)
    maxY = np.int0(rect_center[0]+rect_shape[1]/2)
    minY = np.int0(rect_center[0]-rect_shape[1]/2)

image = image[minX:maxX, minY:maxY]

cv2.imshow("FInale", image)
cv2.waitKey(0)
