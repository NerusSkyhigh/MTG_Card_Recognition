import re
import logging

class Index:



    def __init__(self):
        self.index = {}
        self.cards = {}
        self.word_list = set()

    def add(self, card:dict, card_id:str, keywords:list):
        if card_id not in self.cards:
            self.cards[card_id] = card

        for keyword in keywords:
            if len(keyword) > 3:
                if keyword not in self.index:
                    self.word_list.add(keyword)
                    self.index[keyword] = set()

                self.index[keyword].add(card_id)


    # Search functions
    def _results(self, query):
        if isinstance(query, str):
            return [self.index.get(query, set())]
        else:
            return [self.index.get(keyword, set()) for keyword in query]

    def search(self, query):
        # Provare a fare il ranking invece che con un punteggio sulle carte singole con
        # Il numer di insiemi in cui una parola chiave è presente
        results = self._results(query)
        cards = [self.cards[card_id] for card_id in set.intersection(*results)]
        return cards


    def ranked_search(self, query):
        results = self._results(query)
        ranking = dict.fromkeys( set.union(*results), 0)

        for result in results:
            for card_id in result:
                ranking[card_id] += 1

        return ranking


    # Utility functions
    def standardize_keywords(self, text, ADDING_CARD=False):
        std_text = text.lower()
        # 0-9+\/
        std_text = re.sub('[^a-z\s]+', '', std_text)
        std_text = re.sub('\n', ' ', std_text)

        keywords = set(std_text.split(' ') )
        keywords.discard('')

        if not ADDING_CARD:
            for kw in keywords.copy():
                if not (kw in self.word_list):
                    keywords.remove(kw)

        return keywords


    def in_dictionary(self, keyword):
        if keyword in 'wizards of the coast'+'your mana pool':
           return False;
        if keyword in self.index.keys():
            return True
        else:
            return False


    def get_card(self, card_id):
        return self.cards.get(card_id, set())
