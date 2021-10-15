# Main file that manage all the other files
from Index import Index
import Cropper
from backgroundSub import *

import pytesseract
import pickle
import os
import logging
import configparser
import cv2

from pprint import pprint

import requests
#from PIL import Image
import numpy as np

import sqlite3
from sqlite3 import Error

#cameraID = 1
img_url = "http://192.168.43.167:8080/photo.jpg"


logging.basicConfig(level=logging.INFO)


config = configparser.ConfigParser()
config.read('config.ini')
if 'pickleLocation' in config['MTGSqliteDatabase']:
    pickleLocation = config['MTGSqliteDatabase']['pickleLocation']
else:
    logging.error("'pickle' not found. The program will stop.")
    exit(-1)

if 'saveLocation' in config['MTGSqliteDatabase']:
    dbLocation = config['MTGSqliteDatabase']['saveLocation']
else:
    logging.error("'database' not found. The program will stop.")
    exit(-1)



def rank(image):
    tesseract_text = pytesseract.image_to_string(image)

    tesseract_text = str.rstrip(tesseract_text)
    #print("tesseract text: ", tesseract_text)

    query = index.standardize_keywords(tesseract_text)
    #print("query: ", query)

    if len(query) == 0:
        return [], query


    ranking = index.ranked_search(query)
    ranking = sorted(ranking, key=ranking.get, reverse=True)

    return ranking, query


# Fase 1: Preparo la connessione con il database
def execute_read_query(connection, query):
    cursor = connection.cursor()
    result = None
    try:
        cursor.execute(query)
        result = cursor.fetchall()
        return result
    except Error as e:
        print(f"The error '{e}' occurred")

def create_connection(path):
    connection = None
    try:
        connection = sqlite3.connect(path)
        logging.info("Connection to SQLite DB \'"+path+"\' successful")
    except Error as e:
        logging.error(f"The error '{e}' occurred")

    return connection

connection = create_connection(dbLocation)




# Fase 2: Apro il pickle dell'indice
if ( not os.path.isfile(pickleLocation) ):
    logging.error("\'"+pickleLocation+"\' not found. The program will stop.")
    exit(-1)
else:
    logging.info("Opening the pickle in \'"+pickleLocation+"\'")
    with open(pickleLocation, "rb") as f:
        index = pickle.load(f)


#logging.info("Connecting to cameraId="+str(cameraID) )
#camera = cv2.VideoCapture(cameraID)

# Fase 3: BackgroundSub
bgs = BackgroundSub(img_url)

# Fase 4: Preparo il database
while(True):
    input("Press a key to start the process")

    #_, image = camera.read()
    image, fgMask = bgs.backgroundSub()

    cv2.imshow('Photo', image)
    cv2.waitKey()
    cv2.destroyAllWindows()

    #image = cv2.imread('test.jpg')
    image = Cropper.crop(image, fgMask, grayscale=True)
    #cv2.imwrite("output.png", image)

    cv2.imshow('Photo', image)
    cv2.waitKey()
    cv2.destroyAllWindows()

    ranking, query = rank(image)

    image = Cropper.rotate(image, (image.shape[1]/2, image.shape[0]/2), 180)
    ranking_rot, query_rot = rank(image)

    print("You entered:\n1)", query, "\n2)", query_rot)

    ranking = ranking if len(query) > len(query_rot) else ranking_rot

    if len(ranking) == 0:
        print("\n\n -------- NO CARD FOUND ------------", "\n\n")
        print("No card found")
    else:
        print("\n\n -------- RANKING OUTPUT -----------\tlen:", len(ranking), "\n\n")

        sql_query = "SELECT name, artist, availability, flavorText, name FROM cards WHERE id="+str(ranking[0])
        sql_result= execute_read_query(connection, sql_query)

        print(sql_result)

#camera.release()
