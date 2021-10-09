import cv2
import numpy as np
import requests


#backSub = cv2.createBackgroundSubtractorKNN()
#backSub = cv2.createBackgroundSubtractorMOG2()

#capture = cv2.VideoCapture(0)


#if not capture.isOpened():
#    print('Unable to open')
#    exit(0)
#capture.set(cv2.CAP_PROP_BUFFERSIZE,1)
# RICORDA
#Fallo imparare con posizioni diverse del nastro

def get_image(img_url):
    r = requests.get(img_url, stream=True)
    image = np.asarray(bytearray(r.content), dtype="uint8")
    frame = cv2.imdecode(image, cv2.IMREAD_COLOR)
    return frame




class BackgroundSub:
    background = []
    backSub = None
    img_url = ""

    def __init__(self, img_url, n_photo=10):
        self.img_url = img_url

        print("Taking photos",end="...",flush=True)
        self.take_background_photo(n_photo)
        print("Done. Learning process",end="...",flush=True)
        self.learning()
        print("Done")

    def take_background_photo(self, n_photo):
    	#ret, frame = capture.read()
        frame = get_image(self.img_url)
        for i in range(n_photo):
            print(i, " ",end="")
            frame = get_image(self.img_url)
            #ret, frame = capture.read()

            if frame is None:
                break
            else:
                self.background.append(frame)


    def learning(self):
        self.backSub = cv2.createBackgroundSubtractorKNN()

        for frame in self.background:
            self.backSub.apply(frame)



    def backgroundSub(self):
        print("Taking picture after BGS")
    #while True:
        #ret, frame = capture.read()
        frame = get_image(self.img_url)
        #frame = cv2.resize(frame, (800,800))

        fgMask = frame.copy()
        fgMask = self.backSub.apply(image=frame, fgmask=fgMask, learningRate=0.01)

        kernel = np.ones((3,3), np.uint8)

        fgMask = cv2.erode(fgMask, kernel, iterations=2)
        fgMask = cv2.dilate(fgMask, kernel, iterations=2)

        fgMask[np.abs(fgMask) < 250] = 0
        #cv2.imshow("Mask frame", fgMask)
        #cv2.waitKey(0)
        #cv2.destroyAllWindows()
        #keyboard=input("Wanna retry? y/n")
        #if(keyboard!="y"):
        #    break

        return frame, fgMask
