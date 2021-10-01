import sys
import os
import json
import requests

import sqlite3
from sqlite3 import Error

import logging
from hashlib import sha256

import configparser

# This class is responsable for downloading and managing the MTGJson json
#           https://mtgjson.com/
#   https://docs.python.org/3/library/typing.html
class MTGSqliteDatabase:
    url_db:str
    saveLocation:str
    sha256url:str
    magic_sets:dict


# ---------------------- CONSTRUCTOR -----------------------
    def __init__(self):
        self._loadConfig()

        if ( not os.path.isfile(self.saveLocation) ):
            logging.error("The database was not available in "+self.saveLocation+". I'll download it.")
            logging.error("Note that this process may cause some disalignment if the versions do not match")
            self._downloadMTGSqlite()
            #sys.exit(-1)
        else:
            logging.info("Database found in "+self.saveLocation+". I'll use the available version.")


        connection = None
        try:
            connection = sqlite3.connect(self.saveLocation)
            print("Connection to SQLite DB successful")
        except Error as e:
            print(f"The error '{e}' occurred")





# ---------------------- PRIVATE FUNCTION -------------------
    def _loadConfig(self, configFile:str='config.ini'):
        config = configparser.ConfigParser()
        config.read(configFile)

        default_SaveLocation:bool = False
        default_url_db:bool = False
        default_sha256url:bool = False

        if "MTGSqliteDatabase" in config:
            if 'saveLocation' in config['MTGSqliteDatabase']:
                self.saveLocation = config['MTGSqliteDatabase']['saveLocation']
            else:
                default_SaveLocation = True

            if 'url_db' in config['MTGSqliteDatabase']:
                self.url_db = config['MTGSqliteDatabase']['url_db']
            else:
                default_url_db = True

            if 'sha256url' in config['MTGSqliteDatabase']:
                self.sha256url = config['MTGSqliteDatabase']['sha256url']
            else:
                default_sha256url = True
        else:
            default_SaveLocation:bool = True
            default_url_db:bool = True
            default_sha256url:bool = True

        if default_SaveLocation:
            logging.warning("Misconfigured config file. Falling back to default values: saveLocation=./db/AllPrintings.sqlite")
            self.saveLocation = './db/AllPrintings.sqlite'
        if default_url_db:
            logging.warning("Misconfigured config file. Falling back to default values: url_db=https://mtgjson.com/api/v5/AllPrintings.sqlite")
            self.url_db = 'https://mtgjson.com/api/v5/AllPrintings.sqlite'
        if default_sha256url:
            logging.warning("Misconfigured config file. Falling back to default values: sha256url = https://mtgjson.com/api/v5/AllPrintings.sqlite.sha256")
            self.sha256url = "https://mtgjson.com/api/v5/AllPrintings.sqlite.sha256"



    def _downloadMTGSqlite(self)->None:
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
            self._downloadMTGSqlite()
        else:
            logging.info("Available version is Uptodate")
