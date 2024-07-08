#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include "tgbot/tgbot.h"
#include "modules/CommandHandler.h"


using namespace TgBot;

int main() {
    std::string token(getenv("TOKEN"));
    std::string workspace(getenv("ENV"));
    printf("Token: %s\n", token.c_str());
    std::cout<<"Workspave: "<<workspace<<std::endl;
    Bot bot(token);

    CommandHandler commandHandler(bot, workspace);
    commandHandler.register_commands();

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot, 100, 1);
        while (true) {
            //printf("Long poll started\n");

            longPoll.start();
        }
    } catch (std::exception& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}