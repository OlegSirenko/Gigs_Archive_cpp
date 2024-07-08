//
// Created by tehnokrat on 7/4/24.
//

#include "CommandHandler.h"

CommandHandler::CommandHandler(TgBot::Bot &bot):bot_(bot) {}

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
    std::int64_t chat_id = message->chat->id;
    bot_.getApi().sendMessage(chat_id, Messages::Help::Common::GREETING);
    bot_.getApi().sendMessage(chat_id, Messages::Help::Common::EXAMPLE);
    bot_.getApi().sendPhoto(chat_id, message->chat->photo->bigFileId, "#афиша");
}

void CommandHandler::handleUnknown(const TgBot::Message::Ptr &unknownCommand) {
    bot_.getApi().sendMessage(unknownCommand->chat->id, Messages::Help::UnknownCommands::UNKNOWN_COMMAND + unknownCommand->text);
}

void CommandHandler::handlePoster(const TgBot::Message::Ptr& message){
    std::cout <<"Chat ID " << message->chat->id<<" Chat name "<< message->chat->title<<std::endl;
    if(message->from->isBot){

        return;
    }
    if(message->chat->id == Config::chat_ids::dev_channel_id || message->chat->id == Config::chat_ids::main_channel_id || message->chat->id == Config::chat_ids::main_discussion_id){
        return;
    }
    if(!message->photo.data()){
        bot_.getApi().sendMessage(message->chat->id, Messages::Help::UnknownCommands::POSTER_NO_PHOTO);
        return;
    }

    if((message->caption.empty() != 0)){
        bot_.getApi().sendMessage(message->chat->id, Messages::Help::UnknownCommands::POSTER_NO_CAPTION);
        return;
    }

    if(!(message->caption == Messages::CommonNames::POSTER_RU)){
        bot_.getApi().sendMessage(message->chat->id, Messages::Help::UnknownCommands::POSTER_NO_CAPTION);
        return;
    }

    std::cout<<message->photo.back()->fileId<<std::endl;

    bot_.getApi().sendMessage(message->chat->id, Messages::Moderation::ON_MODERATION);

    sendToApproval(message);
}

void CommandHandler::sendToApproval(const TgBot::Message::Ptr &message) {
    // Setup Inline keyboard
    keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
    TgBot::InlineKeyboardButton::Ptr approveButton = std::make_shared<TgBot::InlineKeyboardButton>(Messages::Moderation::SUCCESS[0].c_str(), "", (Messages::Moderation::SUCCESS[1]+"@"+to_string((message->chat->id))).c_str());
    TgBot::InlineKeyboardButton::Ptr declineButton = std::make_shared<TgBot::InlineKeyboardButton>(Messages::Moderation::DENIED[0].c_str(), "",Messages::Moderation::DENIED[1]+"@"+to_string((message->chat->id)).c_str());
    keyboard->inlineKeyboard.push_back({approveButton, declineButton});

    // send to moderator with Inline keyboard.
    std::cout<<"Message from user ID "<<message->chat->id<<std::endl;
    bot_.getApi().copyMessage(Config::admin_ids::tehnokratgod, message->chat->id, message->messageId,{}, "Markdown", {}, false, nullptr, keyboard);
}

void CommandHandler::handleCallbackQuery(const TgBot::CallbackQuery::Ptr& callback) {
    std::string moderation_result = callback->data.substr(0, callback->data.find('@'));
    std::cout<<"From callback: "<<moderation_result<<std::endl;
    std::string user_id = callback->data.substr(callback->data.find('@')+1);
    std::cout<<"From callback: "<<user_id<<std::endl;

    if(moderation_result == Messages::Moderation::DENIED[1]) {
        bot_.getApi().sendMessage(user_id, Messages::Moderation::DENIED[2]);
    }
    if(moderation_result == Messages::Moderation::SUCCESS[1]){
        bot_.getApi().copyMessage(Config::chat_ids::dev_channel_id, callback->message->chat->id, callback->message->messageId);
        bot_.getApi().sendMessage(user_id, Messages::Moderation::SUCCESS[2]);
    }
    bot_.getApi().deleteMessage(callback->message->chat->id, callback->message->messageId);
}
