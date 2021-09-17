import MTGJsonDatabase
from Index import Index
import pickle
import logging

logging.basicConfig(level=logging.INFO)

db:MTGJsonDatabase = MTGJsonDatabase.MTGJsonDatabase()
db.Update()
magic_sets = db.toDictonary()

index = Index()
# Add cards into the database
for ms in magic_sets:
    for card in magic_sets[ms]['cards']:
        card.pop("foreignData", None)

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
            keywords = index.standardize_keywords(all_text, ADDING_CARD=True)

            card_id = ms+'-'+card['number']

            index.add(card, card_id, keywords)

with open("pickle/mtg_db_en.pkl", "wb") as f:
    pickle.dump(index, f)
