#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include "tgbot/tgbot.h"
#include "modules/CommandHandler.h"
#include "boost/filesystem.hpp"

using namespace TgBot;


int main() {
    //std::string token("");
    std::string workspace(boost::filesystem::current_path().string());
    //printf("Token: %s\n", token.c_str());
    std::cout<<"Workspave: "<<workspace<<std::endl;
    Bot bot(getenv("TOKEN"));

    CommandHandler commandHandler(bot, workspace);
    commandHandler.register_commands();
    commandHandler.logger.logInfo(__FUNCTION__ , "Commands registered");

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            //printf("Long poll started\n");
            commandHandler.logger.logInfo(__FUNCTION__ , "Long poll started");

            longPoll.start();
        }
    } catch (std::exception& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}