import cv2
import numpy as np


def preprocessing(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Blur allows for an easier detection of the image
    # as the contours are more likely to be closed.
    blur = cv2.GaussianBlur(gray,(1,1),0)
    canny = cv2.Canny(blur, 120, 255, 1)

    return canny



def maxAreaRectangle(image):
    contours, hierarchy = cv2.findContours(image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # Sometimes opencv finds multiple rectangles.
    # I assume that the card is the contours that encloses the biggest area.
    maxArea = -1
    cMax = None
    #test = image.copy()
    #cv2.drawContours(test, contours, -1, (255,255,255), 1)
    #cv2.imshow('TUTTI I CONTORNI', test)
    #cv2.waitKey()
    for c in contours:
        rect_center, rect_shape, rect_angle = cv2.minAreaRect(c)

        ratio = max(rect_shape)/(min(rect_shape)+1e-9)

        if (rect_shape[0]*rect_shape[1] > maxArea) and (ratio < 1.5 and ratio > 1.3):
            #print(rect_shape, ratio)
            cMax = c
            maxArea = rect_shape[0]*rect_shape[1]
    #test = image.copy()
    #cv2.drawContours(test, [cMax], 0, (255,255,255), 3)
    #cv2.imshow('cMax', test)
    #cv2.waitKey()
    return cMax



def crop(image, grayscale=False):
    # Preprocessing of the image
    canny = preprocessing(image)

    cMax = maxAreaRectangle(canny)

    rect_center, rect_shape, rect_angle = cv2.minAreaRect(cMax)
    maxArea = rect_shape[0]*rect_shape[1]

    # Find the (rotated) rectangle with the minimum area that encloses the
    # whole contour
    rect = cv2.minAreaRect(cMax)

    area = maxArea # area is needed later to find the rotate image once again
    box = np.int0( cv2.boxPoints(rect) ) # Get vertices coordinates and transform them in integers

    # Rotate the image
    # Box vertices are given counter-clockwise
    dy = box[0][1] - box[1][1]
    dx = box[0][0] - box[1][0]
    angle = np.arctan(dy/dx)*180/np.pi
    #print("\n\t\t", dx, dy, angle, rect, "\n\n")

    # Center of the rectangle in order to perform the rotation
    cX = np.abs( (box[0][0] + box[2][0])//2 )
    cY = np.abs( (box[0][1] + box[2][1])//2 )

    # Rotate the image around the center of the rectangle
    rot_mat = cv2.getRotationMatrix2D( (cX, cY), angle, 1.0)
    result = cv2.warpAffine(image, rot_mat, image.shape[1::-1], flags=cv2.INTER_LINEAR)


    # ------------------------------------------------------------
    # REPEAT THE PROCESS ON THE ROTATED IMAGE AND CROP THE CARD
    # ------------------------------------------------------------
    gray = cv2.cvtColor(result, cv2.COLOR_BGR2GRAY)
    canny = cv2.Canny(gray, 120, 255, 1)

    # Find contours
    contours, hierarchy = cv2.findContours(canny, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    for c in contours:
        rect_center, rect_shape, rect_angle = cv2.minAreaRect(c)
        cArea = rect_shape[0]*rect_shape[1]
        # 1% tollerance on the area in case its not exacly the same after the
        # rotation. I look for a similar area
        if cArea < 1.1*area and cArea > 0.9*area:
            break

    # Find the (rotated) rectangle with the minimum area
    rect = cv2.minAreaRect(c)
    box = np.int0( cv2.boxPoints(rect) ) # Get vertices coordinates and transform them in integers

    # Coordinates of the vertices
    minX = min(box[:, 1])
    maxX = max(box[:, 1])
    minY = min(box[:, 0])
    maxY = max(box[:, 0])

    result = result[minX:maxX, minY:maxY]
    if maxX-minX < maxY-minY:
        rot_mat = cv2.getRotationMatrix2D( ( (maxX-minX)/2, (maxX-minX)/2), -90, 1.0)

        result = cv2.warpAffine(result, rot_mat, (maxX-minX, maxY-minY), flags=cv2.INTER_LINEAR)

    if grayscale:
        return cv2.cvtColor(result, cv2.COLOR_BGR2GRAY)
    else:
        return result
