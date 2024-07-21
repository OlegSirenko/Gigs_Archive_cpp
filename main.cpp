#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include "tgbot/tgbot.h"
#include "modules/CommandHandler.h"
#include "boost/filesystem.hpp"

using namespace TgBot;

Logger logger_sigintHandler;

void sigintHandler(int signal) {
    logger_sigintHandler.logInfo("SIGINT received: ", signal);
    std::cout << "SIGINT received. Exiting gracefully..." << std::endl;
    exit(0);
}

int main() {
    std::string workspace(boost::filesystem::current_path().string());
    std::cout<<"Workspace: "<<workspace<<std::endl;
    Bot bot(getenv("TOKEN"));

    CommandHandler commandHandler(bot, workspace);
    commandHandler.register_commands();
    commandHandler.logger.logInfo(__FUNCTION__ , "Commands registered");

    std::signal(SIGINT, sigintHandler);

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    } catch (std::exception& e) {
        commandHandler.logger.logError(__FUNCTION__ , "{!!! WHILE LOOP ENDED WITH ERROR --- \t", e.what(), "\t}");
    }

    return 0;
}