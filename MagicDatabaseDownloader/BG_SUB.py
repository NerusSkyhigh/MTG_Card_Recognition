import cv2 as cv
import numpy as np
import requests
import Cropper


backSub = cv.createBackgroundSubtractorKNN()
#backSub = cv.createBackgroundSubtractorMOG2()


#RICORDA
#Fallo imparare con posizioni diverse del nastro
img_url = "http://10.196.203.224:8080/photo.jpg"

def take_photo():
    r = requests.get(img_url, stream=True)
    image = np.asarray(bytearray(r.content), dtype="uint8")
    image = cv.imdecode(image, cv.IMREAD_COLOR)
    return image

background=[]
def take_background_photo():
        frame=take_photo()
        i=0
        while i<1:
                frame=take_photo()
                if frame is None:
                    break
                else:
                    #    frame = cv.resize(frame, (800,800))
                        background.append(frame)
                i=i+1


def learning(backSub):
        backSub = cv.createBackgroundSubtractorKNN()
        i=0
        for frame in background:
                fgMask = backSub.apply(frame)
        return backSub
print("Library: Done")
take_background_photo()
#backSub = learning(backSub)
print("Learning process: Done")
#append an image to a string
#img.append(cv.imread(str(i)+'.png'))
frame_taken=0
cv.imshow('Frame', background[0])
cv.imshow('FG Mask', background[0])
print("Ready to take a picture")

while True:
    keyboard = 0
    keyboard = cv.waitKey(0)
    print(keyboard)
    if keyboard == 113 or keyboard == "lol":
        break

    frame=take_photo()
    frame_taken=frame_taken+1
#    frame = cv.resize(frame, (800,800))

    fgMask = backSub.apply(frame)


    kernel = np.ones((3,3), np.uint8)

    fgMask = cv.erode(fgMask, kernel, iterations=2)
    fgMask = cv.dilate(fgMask, kernel, iterations=2)


    fgMask[np.abs(fgMask) < 150] = 0

    cv.imshow('Frame', frame)
    cv.imshow('FG Mask', fgMask)
    #cv.imwrite('lol.png',fgMask)
    print("Image written")
    #append an image to a string
#    if frame_taken%3==0:
#        backSub=learning(backSub)

#    image = Cropper.crop(fgMask, grayscale=True)
    #cv2.imwrite("output.png", image)

#    cv.imshow('Photo', image)
    #cv.waitKey()

    if keyboard == 115:
        cv.imwrite('output.png',fgMask)

    if keyboard == 99:
        im = Cropper.crop(fgMask)
        cv.imshow('Tagliata di carne', im)

print("Now Ending...")
print("The And")
