#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include "tgbot/tgbot.h"
#include "modules/CommandHandler.h"


using namespace TgBot;

int main() {
    std::string token("5609971091:AAGoD_Xe7IpZ0QAoWCxBfV34H5MCcZ8_oVs");
    printf("Token: %s\n", token.c_str());

    Bot bot(token);

    CommandHandler commandHandler(bot);
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