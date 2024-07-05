#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace Messages {
    namespace CommonNames{
        const string POSTER = "#афиша";
    }
    namespace Help {
        namespace Common {
            const string GREETING = "Дарова, этот бот потсит афиши в канал https://t.me/GigsArchive ";
            const string EXAMPLE = "Чтобы добавить новую афишу, вам требуется написать #афиша и свой текст, добавить фотографию афиши, например: ";
            const string AVAILABLE_COMMANDS = "Список доступных комманд:\n";
        }

        namespace UnknownCommands{
            const string UNKNOWN_COMMAND = "Извини, но в боте нет такой команды: ";

        }

    }
    namespace Moderation {
        const string ON_MODERATION = "Ваша афиша на модерации. Пожалуйста, подождите... ";
        const vector<string> SUCCESS = {"Approve", "Approved"};
        const vector<string> DENIED = {"Decline", "Declined"};
    }

    // Add other message constants here
}