TOKEN = "1802576970:AAHCkWfxPUHc5KuH-CVdeQHK5K0m5_8B7Fs"

#!/usr/bin/env python
# pylint: disable=C0116,W0613
# This program is dedicated to the public domain under the CC0 license.

"""
First, a few callback functions are defined. Then, those functions are passed to
the Dispatcher and registered at their respective places.
Then, the bot is started and runs until we press Ctrl-C on the command line.

Usage:
Example of a bot-user conversation using ConversationHandler.
Send /start to initiate the conversation.
Press Ctrl-C on the command line or send a signal to the process to stop the
bot.
"""

import logging

from telegram import ReplyKeyboardMarkup, ReplyKeyboardRemove, Update
from telegram.ext import (
    Updater,
    CommandHandler,
    MessageHandler,
    Filters,
    ConversationHandler,
    CallbackContext,
)

# Enable logging
logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s', level=logging.INFO
)

logger = logging.getLogger(__name__)

GENDER, PHOTO, LOCATION, BIO = range(4)


def start(update: Update, context: CallbackContext) -> int:
    """Starts the conversation and tell the use what the Bot does."""
    update.message.reply_text('Hi! I\'m CardSorterBot. If you send me photo with a Magic card I\'ll try to recognize it.')



def photo(update: Update, context: CallbackContext) -> None:
    print("you son of a bitch I'm in")
    """Stores the photo and asks for a location."""
    user = update.message.from_user
    photo_file = update.message.photo[-1].get_file()
    photo_file.download('user_photo.jpg')
    logger.info("Photo of %s: %s", user.first_name, 'user_photo.jpg')
    update.message.reply_text(
        "Purtroppo sono cieco. Ma non preoccuparti, presto cambierò nazionalità e riuscirò a vedere le tue immagini."
    )

    video = open("GGET{SUP3R_S3CRET_V1DE0}.mp4", 'rb')
    update.message.reply_video(video)



def card_from_text(update: Update, context: CallbackContext) -> None:
    query = update.message.text
    query = index.standardize_keywords(query)

    logger.info(query)
#    update.message.reply_text(
#        'Gorgeous! Now, send me your location please, or send /skip if you don\'t want to.'
#    )

    ranking = index.ranked_search(query)
    print("\n\n -------RANKING OUTPUT------------\n", ranking, "\n\n")

    ranking = sorted(ranking, key=ranking.get, reverse=True)
    if len(ranking) == 0:
        update.message.reply_text("No card found")
    else:
        card = index.get_card(ranking[0])
        update.message.reply_text("Probably you searched the card: " + card['name'] + " from the set "+card['setCode'] + ".")

        update.message.reply_text("Wanna buy it? " + card['purchaseUrls']['cardmarket'])



from Index import Index
index = Index()

def main() -> None:

    import MTGJsonDatabase


    db:MTGJsonDatabase = MTGJsonDatabase.MTGJsonDatabase()
    #db.Update()
    magic_sets = db.toDictonary()

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


    """Run the bot."""
    # Create the Updater and pass it your bot's token.
    updater = Updater(TOKEN)

    # Get the dispatcher to register handlers
    dispatcher = updater.dispatcher

    dispatcher.add_handler(CommandHandler("start", start))

    dispatcher.add_handler(MessageHandler(Filters.photo, photo))

    dispatcher.add_handler(MessageHandler(Filters.text & (~Filters.command), card_from_text))

    # Start the Bot
    updater.start_polling()

    # Run the bot until you press Ctrl-C or the process receives SIGINT,
    # SIGTERM or SIGABRT. This should be used most of the time, since
    # start_polling() is non-blocking and will stop the bot gracefully.
    updater.idle()


if __name__ == '__main__':
    main()
