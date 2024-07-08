//
// Created by tehnokrat on 7/4/24.
//

#ifndef TELEGRAMBOT_FOR_FINCDING_FACES_COMMANDHANDLER_H
#define TELEGRAMBOT_FOR_FINCDING_FACES_COMMANDHANDLER_H

#include "tgbot/tgbot.h"
#include <map>
#include "string"
#include "../resources/messages.hpp"
#include "../resources/config.hpp"

class CommandHandler {
public:
    explicit CommandHandler(TgBot::Bot&, std::string);
    void register_commands();

private:
    TgBot::Bot& bot_;
    string workspace_;
    TgBot::InlineKeyboardMarkup::Ptr keyboard;
    void handleStart(const TgBot::Message::Ptr& message);
    void handleUnknown(const TgBot::Message::Ptr& unknownCommand);
    void handlePoster(const TgBot::Message::Ptr& message);
    void sendToApproval(const TgBot::Message::Ptr& message);
    void handleCallbackQuery(const TgBot::CallbackQuery::Ptr&);


};


#endif //TELEGRAMBOT_FOR_FINCDING_FACES_COMMANDHANDLER_H
