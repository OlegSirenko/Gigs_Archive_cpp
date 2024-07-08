#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace Messages {
    namespace CommonNames{
        const string POSTER_RU = "#афиша";
        const string POSTER_EN = "#poster";
    }
    namespace Help {
        namespace Common {
            const string GREETING = "Дарова, этот бот потсит афиши в канал https://t.me/GigsArchive ";
            const string EXAMPLE = "Чтобы добавить новую афишу, вам требуется написать #афиша и свой текст, добавить фотографию афиши, например: ";
            const string AVAILABLE_COMMANDS = "Список доступных комманд:\n";
        }

        namespace UnknownCommands{
            const string UNKNOWN_COMMAND = "Извини, но в боте нет такой команды: ";
            const string POSTER_NO_CAPTION = "Ожидается \"#афиша\" как подпись к картинке. \nИспользуй /help если что-то не получается. ";
            const string POSTER_NO_PHOTO = "Ожидается \"#афиша\" как подпись к картинке. Отправь картинку вместе с подписью. Пример в: /help";
        }

    }
    namespace Moderation {
        const string ON_MODERATION = "Ваша афиша на модерации. Пожалуйста, подождите... ";
        const vector<string> SUCCESS = {"Approve", "Approved", "Ваша афиша отправлена в канал https://t.me/GigsArchive."};
        const vector<string> DENIED = {"Decline", "Declined", "Модератор отклонил ваше сообщение"};
    }

    // Add other message constants here
}