#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include "tgbot/tgbot.h"
#include "modules/CommandHandler.h"
#include "boost/filesystem.hpp"

using namespace TgBot;


void sigintHandler(int signal) {
    std::cout << "SIGINT received. Exiting gracefully..." << std::endl;
    exit(0);
}

int main() {
    std::string workspace(boost::filesystem::current_path().string());
    std::cout<<"Workspace: "<<workspace<<std::endl;

    ifstream fileHandler(workspace + "/resources/token.txt");
    if(!fileHandler.is_open()){
        std::cerr << "Could not open file" << workspace + "/resources/token.txt" <<std::endl;
        return 1;
    }
    std::string token;
    getline(fileHandler, token);

    Bot bot(token);

    CommandHandler commandHandler(bot, "Workspace");
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