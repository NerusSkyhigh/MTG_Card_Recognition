import cv2
import numpy as np

# Image read with
# image = cv2.imread('test.jpg')
def crop(image, grayscale=False):
    # Preprocessing of the image
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (3, 3), 0)
    canny = cv2.Canny(blurred, 120, 255, 1)

    # Find contours
    contours, hierarchy = cv2.findContours(canny, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Sometimes opencv finds multiple rectangles.
    # I assume that the card is the contours that encloses the biggest area.
    maxArea = -1
    for c in contours:
        if cv2.contourArea(c) > maxArea:
            cMax = c
            maxArea = cv2.contourArea(c)


    # Find the (rotated) rectangle with the minimum area that encloses the
    # whole contour
    rect = cv2.minAreaRect(c)
    area = maxArea # area is needed later to find the rotate image once again
    box = np.int0( cv2.boxPoints(rect) ) # Get vertices coordinates and transform them in integers


    # Rotate the image
    # Box vertices are given counter-clockwise
    dy = box[0][1] - box[1][1]
    dx = box[0][0] - box[1][0]
    angle = np.arctan(dy/dx)*180/np.pi

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
    blurred = cv2.GaussianBlur(gray, (3, 3), 0)
    canny = cv2.Canny(blurred, 120, 255, 1)

    # Find contours
    contours, hierarchy = cv2.findContours(canny, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    for c in contours:
        cArea = cv2.contourArea(c)
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

    if grayscale:
        return gray[minX:maxX, minY:maxY]
    else:
        return result[minX:maxX, minY:maxY]
