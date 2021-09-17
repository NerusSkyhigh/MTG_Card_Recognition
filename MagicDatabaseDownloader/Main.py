# Main file that manage all the other files
import MTGJsonDatabase
from Index import Index
import Cropper

import pytesseract
import pickle
import os
import logging
import cv2

from pprint import pprint

import requests
#from PIL import Image
import numpy as np

#cameraID = 1
img_url = "http://192.168.42.129:8080/photo.jpg"


logging.basicConfig(level=logging.INFO)



def rank(image):
    tesseract_text = pytesseract.image_to_string(image)

    tesseract_text = str.rstrip(tesseract_text)
    print("tesseract test: ", tesseract_text)

    query = index.standardize_keywords(tesseract_text)
    print("query: ", query)

    if len(query) == 0:
        return [], query


    ranking = index.ranked_search(query)
    ranking = sorted(ranking, key=ranking.get, reverse=True)

    return ranking, query






# Fase 1: Preparo il database
if ( not os.path.isfile("pickle/mtg_db_en.pkl") ):
    logging.error("\'pickle/mtg_db_en.pkl\' not found. The program will stop.")
    System.exit(-1)
else:
    logging.info("Opening the pickle in \'pickle/mtg_db_en.pkl\'")
    with open("pickle/mtg_db_en.pkl", "rb") as f:
        index = pickle.load(f)


#logging.info("Connecting to cameraId="+str(cameraID) )
#camera = cv2.VideoCapture(cameraID)


# Fase 2: Preparo il database
while(True):
    input("Press a key to start the process")

    # Begin of Copy pasta
    from gpiozero import LED
    from time import sleep

    led = LED(17)

    led.on()
    sleep(1)
    led.off()
    # end of copy pasta

    #_, image = camera.read()

    r = requests.get(img_url, stream=True)
    image = np.asarray(bytearray(r.content), dtype="uint8")
    image = cv2.imdecode(image, cv2.IMREAD_COLOR)

    #cv2.imshow('Photo', image)
    #cv2.waitKey()

    #image = cv2.imread('test.jpg')
    image = Cropper.crop(image, grayscale=True)
    #cv2.imwrite("output.png", image)


    ranking, query = rank(image)

    image = Cropper.rotate(image, (image.shape[1]/2, image.shape[0]/2), 180)
    ranking_rot, query_rot = rank(image)

    print("You entered:\n", query, "\n", query_rot)

    ranking = ranking if len(query) > len(query_rot) else ranking_rot

    if len(ranking) == 0:
        print("\n\n -------- NO CARD FOUND ------------", "\n\n")
        print("No card found")
    else:
        print("\n\n -------- RANKING OUTPUT -----------", len(ranking), "\n\n")
        print("Name:", index.get_card(ranking[0])["name"] )
        pprint(index.get_card(ranking[0]) )

#camera.release()
