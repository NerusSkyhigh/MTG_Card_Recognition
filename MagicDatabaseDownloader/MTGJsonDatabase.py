import os
import json
import requests

import logging
from hashlib import sha256

from tqdm import tqdm

# This class is responsable for downloading and managing the MTGJson json
#           https://mtgjson.com/
#
class MTGJsonDatabase:
    url_db:str
    saveLocation:str # Default save location

    # Variable used for the image download
    image_id_wildcard = '${card.identifiers.scryfallId}'
    image_url = 'https://api.scryfall.com/cards/${card.identifiers.scryfallId}?format=image'

    #Variables actually used in the computation
    magic_sets:dict;
# ---------------------- CONSTRUCTOR -----------------------
    #TODO: In the future I could set these default strings in an external config file!
    def __init__(self, saveLocation:str='./JSONs/AllPrintings.json', url_db:str='https://mtgjson.com/api/v5/AllPrintings.json'):
        self.saveLocation = saveLocation
        self.url_db = url_db

        if ( not os.path.isfile(saveLocation) ):
            logging.info("AllPrintings.json was not available in "+saveLocation+". Proceed with the download")
            self._downloadMTGJson(self.url_db, self.saveLocation)
        else:
            logging.info("AllPrintings.json was found. I'll use the available version.")

        with open(saveLocation, 'r', encoding="utf8") as json_file:
          magic_sets = json.load(json_file)
          self.magic_sets = magic_sets['data']

# ---------------------- PRIVATE FUNCTION -------------------
    def _downloadMTGJson(self, url_db:str, saveLocation:str):
        #r = requests.get(url_db, allow_redirects=True)
        #open(saveLocation, 'wb').write(r.content);
        with requests.get(url_db, stream=True, allow_redirects=True) as r:
            r.raise_for_status()

            if os.path.exists(saveLocation):
                logging.info("I'll delete the old file")
                os.remove(saveLocation)

            if not os.path.exists(os.path.dirname(saveLocation)):
                logging.info("The folder \'"+saveLocation+"\' does not exist. I'll create it.")
                os.makedirs(os.path.dirname(saveLocation))

            with open(saveLocation, 'wb') as f:
                try:
                    f.write(r.content)
                except Exception as e:
                    print(e)
                else:
                    logging.info("Download completed successfully")


#TODO: Function that checks for updates using the sha265:
#   https://mtgjson.com/api/v5/AllPrintings.json.sha256
#   https://mtgjson.com/faq/#can-i-verify-the-downloaded-files
#    def Update():


if __name__ == "__main__":
    # https://realpython.com/python-logging/
    logging.basicConfig(level=logging.INFO)
    db:MTGJsonDatabase = MTGJsonDatabase()
