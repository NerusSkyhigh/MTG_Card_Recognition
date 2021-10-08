print("Library:",end="...",flush=True)
import cv2 as cv
import numpy as np
import requests

img_url = "http://192.168.137.191:8080/photo.jpg"
r = requests.get(img_url, stream=True)


backSub = cv.createBackgroundSubtractorKNN()
#backSub = cv.createBackgroundSubtractorMOG2()

#capture = cv.VideoCapture(0)


#if not capture.isOpened():
#    print('Unable to open')
#    exit(0)
#capture.set(cv.CAP_PROP_BUFFERSIZE,1)
# RICORDA
#Fallo imparare con posizioni diverse del nastro

def get_image(img_url):
    r = requests.get(img_url, stream=True)
    image = np.asarray(bytearray(r.content), dtype="uint8")
    frame = cv.imdecode(image, cv.IMREAD_COLOR)
    return frame

background=[]

def take_background_photo():
	#ret, frame = capture.read()
    i=0
    frame = get_image(img_url)

    while i<10:
		#ret, frame = capture.read()
        frame = get_image(img_url)

        if frame is None:
            break
        else:
			#frame = cv.resize(frame, (800,800))
            background.append(frame)
        i=i+1


def learning(backSub):
	backSub = cv.createBackgroundSubtractorKNN()
	i=0
	for frame in background:
		fgMask = backSub.apply(frame)
	return backSub


print("Done")
print("Taking photos:",end="...",flush=True)
take_background_photo()
print("Done")
print("Learning process:",end="...",flush=True)
backSub = learning(backSub)
print("Done")
#append an image to a string
#img.append(cv2.imread(str(i)+'.png'))
frame_taken=0
#cv.imshow('Frame', background[0])
#cv.imshow('FG Mask', background[0])
print("Ready to take a picture")

while True:
    keyboard = input()
    if keyboard == "q":
        break

    #ret, frame = capture.read()
    frame = get_image(img_url)

    frame_taken=frame_taken+1

    #frame = cv.resize(frame, (800,800))

    fgMask = 0
    if(keyboard=="c"):
    	fgMask = backSub.apply(frame=frame, fgMask=fgMask, learningRate=0.01)
    else:
       	fgMask = backSub.apply(frame)


    kernel = np.ones((3,3), np.uint8)
    lol=fgMask.copy()
    fgMask = cv.erode(fgMask, kernel, iterations=2)
    fgMask = cv.dilate(fgMask, kernel, iterations=2)


    fgMask[np.abs(fgMask) < 250] = 0

#    cv.imshow('Frame', frame)
#    cv.imshow('FG Mask', fgMask)
#    cv.imshow('FG without',lol)
    cv.imwrite('fgMask.png',fgMask)
    cv.imwrite('frame.png',frame)
    print(frame_taken)
    print("Image written")
    #append an image to a string
    #if frame_taken%3==0:
    #    backSub=learning(backSub)
