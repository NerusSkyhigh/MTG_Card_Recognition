# Main file that manage all the other files
import MTGJsonDatabase
from Index import Index
import Cropper

import pickle
import os
import logging
import cv2

import configparser



# Fase 1: Preparo il database
logging.basicConfig(level=logging.INFO)


if ( not os.path.isfile("pickle/mtg_db_en.pkl") ):
    db:MTGJsonDatabase = MTGJsonDatabase.MTGJsonDatabase()
    #db.Update()
    magic_sets = db.toDictonary()

    index = Index()
    # Add cards into the database
    for ms in magic_sets:
        for card in magic_sets[ms]['cards']:
            if 'paper' in card['availability']:
                flavorText = card['flavorText'] if 'flavorText' in card else ''
                originalText = card['originalText'] if 'originalText' in card else ''
                text = card['text'] if 'text' in card else ''
                name = card['name'] if 'name' in card else ''
                flavorName = card['flavorName'] if 'flavorName' in card else ''
                card_type = card['type'] if 'type' in card else ''
                artist = card['artist'] if 'artist' in card else ''

                all_text = flavorText+' '+ originalText+' '+ text+' '+ name+' '+\
                           flavorName+' '+ card_type + ' '+ artist;
                keywords = index.standardize_keywords(all_text)

                card_id = ms+'-'+card['number']

                index.add(card, card_id, keywords)

    with open("pickle/mtg_db_en.pkl", "wb") as f:
        pickle.dump(index, f)

else:
    with open("pickle/mtg_db_en.pkl", "rb") as f:
        index = pickle.load(f)



query = input("I'll give you the first results for a query. Enter #exit to leave:\n")#.split()
query = index.standardize_keywords(query)

while(not ("#exit" in query)):
    print("You entered:", query)
    ranking = index.ranked_search(query)
    print("\n\n -------RANKING OUTPUT------------\n", ranking, "\n\n")

    ranking = sorted(ranking, key=ranking.get, reverse=True)
    if len(ranking) == 0:
        print("No card found")
    else:
        print(index.get_card(ranking[0]) )
    query = input("I'll give you the first results for a query. Enter #exit to leave:\n")#.split()
    query = index.standardize_keywords(query)
