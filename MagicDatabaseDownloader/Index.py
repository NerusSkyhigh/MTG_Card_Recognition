import re
import logging

class Index:
    def __init__(self):
        self.index = {}
        self.cards = {}

    def add(self, card:dict, card_id:str, keywords:list):
        if card_id not in self.cards:
            self.cards[card_id] = card

        for keyword in keywords:
            if len(keyword) > 3:
                if keyword not in self.index:
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
    def standardize_keywords(self, text):
        std_text = text.lower()
        # 0-9+\/
        std_text = re.sub('[^a-z\s]+', '', std_text)
        std_text = re.sub('\n', ' ', std_text)

        keywords = set(std_text.split(' ') )
        keywords.discard('')

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

# -------------------------------------------------------------------

if __name__ == "__main__":
    import MTGJsonDatabase
    # Create object
    logging.basicConfig(level=logging.INFO)

    db:MTGJsonDatabase = MTGJsonDatabase.MTGJsonDatabase()
    db.Update()
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
