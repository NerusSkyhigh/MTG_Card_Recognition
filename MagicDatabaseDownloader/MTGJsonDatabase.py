import os
import json
import requests

import logging
from hashlib import sha256

import configparser

# This class is responsable for downloading and managing the MTGJson json
#           https://mtgjson.com/
#   https://docs.python.org/3/library/typing.html
class MTGJsonDatabase:
    url_db:str
    saveLocation:str
    sha256url:str
    magic_sets:dict


# ---------------------- CONSTRUCTOR -----------------------
    def __init__(self):
        self._loadConfig()

        if ( not os.path.isfile(self.saveLocation) ):
            logging.info("AllPrintings.json was not available in "+self.saveLocation+". Proceed with the download")
            self._downloadMTGJson()
        else:
            logging.info("AllPrintings.json was found. I'll use the available version.")

        with open(self.saveLocation, 'r', encoding="utf8") as json_file:
          magic_sets = json.load(json_file)
          self.magic_sets = magic_sets['data']

# ---------------------- PRIVATE FUNCTION -------------------
    def _loadConfig(self, configFile:str='config.ini'):
        config = configparser.ConfigParser()
        config.read(configFile)

        default_SaveLocation:bool = False
        default_url_db:bool = False
        default_sha256url:bool = False
        if "MTGJsonDatabase" in config:
            if 'saveLocation' in config['MTGJsonDatabase']:
                self.saveLocation = config['MTGJsonDatabase']['saveLocation']
            else:
                default_SaveLocation = True

            if 'url_db' in config['MTGJsonDatabase']:
                self.url_db = config['MTGJsonDatabase']['url_db']
            else:
                default_url_db = True

            if 'sha256url' in config['MTGJsonDatabase']:
                self.sha256url = config['MTGJsonDatabase']['sha256url']
            else:
                default_sha256url = True
        else:
            default_SaveLocation:bool = True
            default_url_db:bool = True
            default_sha256url:bool = True

        if default_SaveLocation:
            logging.warning("Misconfigured config file. Falling back to default values: saveLocation=./JSONs/AllPrintings.json")
            self.saveLocation = './JSONs/AllPrintings.json'
        if default_url_db:
            logging.warning("Misconfigured config file. Falling back to default values: url_db=https://mtgjson.com/api/v5/AllPrintings.json")
            self.url_db = 'https://mtgjson.com/api/v5/AllPrintings.json'
        if default_sha256url:
            logging.warning("Misconfigured config file. Falling back to default values: sha256url = https://mtgjson.com/api/v5/AllPrintings.json.sha256")
            self.sha256url = "https://mtgjson.com/api/v5/AllPrintings.json.sha256"



    def _downloadMTGJson(self)->None:
        with requests.get(self.url_db, stream=True, allow_redirects=True) as r:
            r.raise_for_status()

            if os.path.exists(self.saveLocation):
                logging.info("An older database exist. I\'ll delete it.")
                os.remove(self.saveLocation)

            if not os.path.exists(os.path.dirname(self.saveLocation)):
                logging.info("The folder \'"+self.saveLocation+"\' does not exist. I'll create it.")
                os.makedirs(os.path.dirname(self.saveLocation))

            with open(self.saveLocation, 'wb') as f:
                try:
                    f.write(r.content)
                except Exception as e:
                    print(e)
                else:
                    logging.info("Download completed successfully")



    def Update(self)->None:
        with requests.get(self.sha256url, stream=True, allow_redirects=True) as r:
            r = requests.get(self.sha256url)
            server_sha256:str = str(r.content)[2:-1]
            logging.info("Server sha256: "+server_sha256)

        with open(self.saveLocation, 'rb') as f:
            try:
                localVersion = f.read()
            except Exception as e:
                print(e)
        local_sha256:str = sha256(localVersion).hexdigest()
        logging.info("Local sha256:  "+local_sha256)

        if not ( server_sha256 == local_sha256):
            logging.info("Update available. I'll download it.")
            self._downloadMTGJson()
        else:
            logging.info("Available version is Uptodate")



    def toDictonary(self, printedOnly:bool=True)->dict:
        if printedOnly:
            magic_sets = self.magic_sets.copy()

            for set in magic_sets.keys():
                for card in magic_sets[set]['cards']:
                    if not 'paper' in card['availability']:
                        magic_sets[set]['cards'].remove(card)
            return magic_sets
        else:
            return self.magic_sets.copy()



if __name__ == "__main__":
    # https://realpython.com/python-logging/
    logging.basicConfig(level=logging.INFO)
    db:MTGJsonDatabase = MTGJsonDatabase()
    db.Update()
    ms = db.toDictonary()
    #print(ms)
