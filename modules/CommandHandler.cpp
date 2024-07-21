//
// Created by tehnokrat on 7/4/24.
//

#include "CommandHandler.h"

#include <utility>


CommandHandler::CommandHandler(TgBot::Bot &bot, std::string workspace):bot_(bot),workspace_(std::move(workspace)) {

}

void CommandHandler::register_commands() {
    bot_.getEvents().onUnknownCommand([this](const TgBot::Message::Ptr& unknownCommand){
        this->handleUnknown(unknownCommand);
    });
    bot_.getEvents().onCommand({"start", "help"}, [this](const TgBot::Message::Ptr& message) {
        this->handleStart(message);
    });

    bot_.getEvents().onNonCommandMessage([this](const TgBot::Message::Ptr& message){
        this->handlePoster(message);
    });

    bot_.getEvents().onCallbackQuery([this](const TgBot::CallbackQuery::Ptr& callback){
        this->handleCallbackQuery(callback);
    });
}

// HANDLERS
void CommandHandler::handleStart(const TgBot::Message::Ptr& message){
    try {
        logger.logInfo(__FUNCTION__, "Chat ID on start command: ", to_string(message->chat->id).c_str());
        std::int64_t chat_id = message->chat->id;
        bot_.getApi().sendMessage(chat_id, Messages::Help::Common::GREETING);
        bot_.getApi().sendMessage(chat_id, Messages::Help::Common::EXAMPLE);
        logger.logInfo(__FUNCTION__, "Sending photo");
        bot_.getApi().sendPhoto(chat_id,
                                "https://github.com/OlegSirenko/Gigs_Archive_cpp/blob/main/resources/img.png?raw=true",
                                "#афиша");
        logger.logInfo(__FUNCTION__, "Start command successful");
    }
    catch (std::exception& exception){
        logger.logError(__FUNCTION__, exception.what());
    }
}

void CommandHandler::handleUnknown(const TgBot::Message::Ptr &unknownCommand) {
    bot_.getApi().sendMessage(unknownCommand->chat->id, Messages::Help::UnknownCommands::UNKNOWN_COMMAND + unknownCommand->text);
}

void CommandHandler::handlePoster(const TgBot::Message::Ptr& message){
    //std::cout <<"Chat ID " << message->chat->id<<" Chat name "<< message->chat->title<<std::endl;

    try{
        if(message->from->isBot){
            logger.logWarn(__FUNCTION__, "Message from bot");
            return;
        }
        if(message->chat->id == Config::chat_ids::dev_channel_id || message->chat->id == Config::chat_ids::main_channel_id || message->chat->id == Config::chat_ids::main_discussion_id){
            logger.logInfo(__FUNCTION__, "Message from main channel");
            return;

        }
        if(!message->photo.data()){
            logger.logWarn(__FUNCTION__, "No photo");
            bot_.getApi().sendMessage(message->chat->id, Messages::Help::UnknownCommands::POSTER_NO_PHOTO);
            return;
        }

        if((message->caption.empty() != 0)){
            logger.logWarn(__FUNCTION__, "Caption empty");
            bot_.getApi().sendMessage(message->chat->id, Messages::Help::UnknownCommands::POSTER_NO_CAPTION);
            return;
        }

        if(message->caption.find(Messages::CommonNames::POSTER_RU) == std::string::npos){
            logger.logWarn(__FUNCTION__, "Caption not valid: ", message->caption.c_str() , " from: ", (message->chat->firstName).c_str());
            bot_.getApi().sendMessage(message->chat->id, Messages::Help::UnknownCommands::POSTER_NO_CAPTION);
            return;
        }

        bot_.getApi().sendMessage(message->chat->id, Messages::Moderation::ON_MODERATION);

        sendToApproval(message);
        logger.logInfo(__FUNCTION__, "Handle poster successful");
    }
    catch (std::exception& exception){
        logger.logError(__FUNCTION__, exception.what());
    }
}

void CommandHandler::sendToApproval(const TgBot::Message::Ptr &message) {
    try {
        // Setup Inline keyboard
        keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
        TgBot::InlineKeyboardButton::Ptr approveButton = std::make_shared<TgBot::InlineKeyboardButton>(
                Messages::Moderation::SUCCESS[0].c_str(), "",
                (Messages::Moderation::SUCCESS[1] + "@" + to_string((message->chat->id))).c_str());
        TgBot::InlineKeyboardButton::Ptr declineButton = std::make_shared<TgBot::InlineKeyboardButton>(
                Messages::Moderation::DENIED[0].c_str(), "",
                Messages::Moderation::DENIED[1] + "@" + to_string((message->chat->id)));
        keyboard->inlineKeyboard.push_back({approveButton, declineButton});

        logger.logInfo(__FUNCTION__, "Sent to moderation");
        // send to moderator with Inline keyboard.
        logger.logInfo(__FUNCTION__, "Message from user ID: ", message->chat->id);
        bot_.getApi().copyMessage(Config::admin_ids::tehnokratgod, message->chat->id, message->messageId, {},
                                  "Markdown", {}, false, nullptr, keyboard);
        logger.logInfo(__FUNCTION__, "On moderation");
    }
    catch (std::exception& exception){
        logger.logError(__FUNCTION__, exception.what());
    }
}

void CommandHandler::handleCallbackQuery(const TgBot::CallbackQuery::Ptr& callback) {
    try {
        std::string moderation_result = callback->data.substr(0, callback->data.find('@'));
        logger.logInfo(__FUNCTION__, "From callback: ", moderation_result.c_str());
        std::string user_id = callback->data.substr(callback->data.find('@') + 1);
        logger.logInfo(__FUNCTION__, "From callback: ", user_id.c_str());

        if (moderation_result == Messages::Moderation::DENIED[1]) {
            bot_.getApi().sendMessage(user_id, Messages::Moderation::DENIED[2]);
        }
        if (moderation_result == Messages::Moderation::SUCCESS[1]) {
            bot_.getApi().copyMessage(Config::chat_ids::dev_channel_id, callback->message->chat->id,
                                      callback->message->messageId);
            bot_.getApi().sendMessage(user_id, Messages::Moderation::SUCCESS[2]);
        }
        bot_.getApi().deleteMessage(callback->message->chat->id, callback->message->messageId);
    } catch (std::exception& exception) {
        logger.logError(__FUNCTION__, exception.what());
    }
}
