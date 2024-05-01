/*
 * Copyright (C) 2011 Fredi Machado <https://github.com/fredimachado>
 * IRCClient is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * http://www.gnu.org/licenses/lgpl.html
 */

int global = 0;

#include <regex>
#include "IRCHandler.h"

IRCCommandHandler ircCommandTable[NUM_IRC_CMDS] =
    {
        {"PRIVMSG", &IRCClient::HandlePrivMsg},
        {"NOTICE", &IRCClient::HandleNotice},
        {"JOIN", &IRCClient::HandleChannelJoinPart},
        {"PART", &IRCClient::HandleChannelJoinPart},
        {"NICK", &IRCClient::HandleUserNickChange},
        {"QUIT", &IRCClient::HandleUserQuit},
        {"353", &IRCClient::HandleChannelNamesList},
        {"433", &IRCClient::HandleNicknameInUse},
        {"001", &IRCClient::HandleServerMessage},
        {"002", &IRCClient::HandleServerMessage},
        {"003", &IRCClient::HandleServerMessage},
        {"004", &IRCClient::HandleServerMessage},
        {"005", &IRCClient::HandleServerMessage},
        {"250", &IRCClient::HandleServerMessage},
        {"251", &IRCClient::HandleServerMessage},
        {"252", &IRCClient::HandleServerMessage},
        {"253", &IRCClient::HandleServerMessage},
        {"254", &IRCClient::HandleServerMessage},
        {"255", &IRCClient::HandleServerMessage},
        {"265", &IRCClient::HandleServerMessage},
        {"266", &IRCClient::HandleServerMessage},
        {"366", &IRCClient::HandleServerMessage},
        {"372", &IRCClient::HandleServerMessage},
        {"375", &IRCClient::HandleServerMessage},
        {"376", &IRCClient::HandleServerMessage},
        {"439", &IRCClient::HandleServerMessage},
};

std::vector<std::string> split(const std::string &str, const std::string &delim)
{
    std::vector<std::string> v;
    for (std::size_t last_pos = 0; last_pos < str.size();)
    {
        std::size_t pos = str.find(delim, last_pos);
        if (pos == str.npos)
            pos = str.size();
        v.push_back(str.substr(last_pos, pos - last_pos));
        last_pos = pos + delim.size();
    }
    return v;
}

void IRCClient::HandleCTCP(IRCMessage message)
{
    std::string to = message.parameters.at(0);
    std::string text = message.parameters.at(message.parameters.size() - 1);

    // Remove '\001' from start/end of the string
    text = text.substr(1, text.size() - 2);

    std::cout << "[" + message.prefix.nick << " requested CTCP " << text << "]" << std::endl;

    if (to == _nick)
    {
        if (text == "VERSION") // Respond to CTCP VERSION
        {
            SendIRC("NOTICE " + message.prefix.nick + " :\001VERSION Open source IRC client by Fredi Machado - https://github.com/fredimachado/IRCClient \001");
            return;
        }

        // CTCP not implemented
        SendIRC("NOTICE " + message.prefix.nick + " :\001ERRMSG " + text + " :Not implemented\001");
    }
}

void IRCClient::HandlePrivMsg(IRCMessage message)
{

    std::string mainstring = "PRIVMSG ";

    std::string to = message.parameters.at(0);
    mainstring += to + " :";
    std::string text = message.parameters.at(message.parameters.size() - 1);
    auto lol = std::string("Barbotage");
    // Handle Client-To-Client Protocol
    if (text[0] == '\001')
    {
        HandleCTCP(message);
        return;
    }
    using namespace std;
    if (to[0] == '#')
        std::cout << "From " + message.prefix.nick << " @ " + to + ": " << text << std::endl;
    else
        std::cout << "From " + message.prefix.nick << ": " << text << std::endl;

    std::string orig = message.prefix.nick;

    const std::regex dep("mklutra");
    const std::regex coeur("\u2665");
    // XAVIER
    std::smatch sm;
    if (std::regex_search(orig, sm, dep) && text.find("Inventaire") == std::string::npos && text.find("déposé dans le slot") == std::string::npos && text.find("ramassé") == std::string::npos)
    {
        string del = "\u2665";

        if (std::regex_search(text, sm, coeur))
        {
            std::cout << "coeur trouvé" << sm.size() << endl;

            std::vector<std::string> words = split(text, del);
            cout << words.size() << endl;
            for (auto a : words)
            {
                const std::regex nombre("[0-9]*");
                auto pos1 = a.rfind("]");
                auto pos2 = a.rfind("[");
                if (pos1 != string::npos && pos2 != string::npos)
                {

                    string att = a.substr(pos2 + 1, pos1 - pos2 - 1);
                    string digit = "";

                    for (auto chdar : att)
                    {
                        cout << "ascii " << chdar;
                        cout << "isdigit" << isdigit(chdar) << endl;
                        if (isdigit(chdar))
                        {
                            digit = digit + chdar;
                        }
                    }
                    if (!digit.empty())
                    {
                        std::string answer(mainstring + "!pick " + digit);
                        SendIRC(answer);
                    }

                    /*if (std::regex_search(att, sm, nombre))
                    {
                        for (auto aa : sm)
                        {

                            for (auto chdar : aa.str())
                            {
                                cout << "ascii " << chdar;
                                cout << isdigit(chdar) << endl;
                            }
                            cout << "word" << aa.str() << endl;
                            response = IrcMessage("PRIVMSG", "", std::vector<std::string>(original.params()), "!pick " + aa.str());
                            writeFn(m_Parser.parseMessage(response));
                        }
                    }*/
                }
            }
        }
    }

    const std::regex victoire("VICTOIRE. Pendant la bagarre j'ai vendu");

    if (text.find(lol) != std::string::npos)
    {
        string zoologist = "zoologist";
        string destiny = "destiny";

        if (text.find("stock") != std::string::npos)
        {

            std::string answer(mainstring + "!inventory");
            SendIRC(answer);
        }
        else if (text.find("sers") != std::string::npos)
        {
            std::string pattern("sers [0-9]");
            const std::regex rx(pattern);

            std::cout << lol << text;
            std::smatch sm;
            if (std::regex_search(text, sm, rx))
            {
                int number = (sm[0].str()).back() - '0';
                std::cout << "lol" << number;

                std::string answer(mainstring + "!drop " + std::to_string(number));
                SendIRC(answer);
            }
            else
            {
                std::cout << "fail";
            }
        }
    }
    else
    {
        std::cout << "nope";
    }
}

void IRCClient::HandleNotice(IRCMessage message)
{
    std::string from = message.prefix.nick != "" ? message.prefix.nick : message.prefix.prefix;
    std::string text;

    if (!message.parameters.empty())
        text = message.parameters.at(message.parameters.size() - 1);

    if (!text.empty() && text[0] == '\001')
    {
        text = text.substr(1, text.size() - 2);
        if (text.find(" ") == std::string::npos)
        {
            std::cout << "[Invalid " << text << " reply from " << from << "]" << std::endl;
            return;
        }
        std::string ctcp = text.substr(0, text.find(" "));
        std::cout << "[" << from << " " << ctcp << " reply]: " << text.substr(text.find(" ") + 1) << std::endl;
    }
    else
        std::cout << "-" << from << "- " << text << std::endl;
}

void IRCClient::HandleChannelJoinPart(IRCMessage message)
{
    std::string channel = message.parameters.at(0);
    std::string action = message.command == "JOIN" ? "joins" : "leaves";
    std::cout << message.prefix.nick << " " << action << " " << channel << std::endl;
}

void IRCClient::HandleUserNickChange(IRCMessage message)
{
    std::string newNick = message.parameters.at(0);
    std::cout << message.prefix.nick << " changed his nick to " << newNick << std::endl;
}

void IRCClient::HandleUserQuit(IRCMessage message)
{
    std::string text = message.parameters.at(0);
    std::cout << message.prefix.nick << " quits (" << text << ")" << std::endl;
}

void IRCClient::HandleChannelNamesList(IRCMessage message)
{
    std::string channel = message.parameters.at(2);
    std::string nicks = message.parameters.at(3);
    std::cout << "People on " << channel << ":" << std::endl
              << nicks << std::endl;
}

void IRCClient::HandleNicknameInUse(IRCMessage message)
{
    std::cout << message.parameters.at(1) << " " << message.parameters.at(2) << std::endl;
}

void IRCClient::HandleServerMessage(IRCMessage message)
{
    if (message.parameters.empty())
        return;

    std::vector<std::string>::const_iterator itr = message.parameters.begin();
    ++itr; // skip the first parameter (our nick)
    for (; itr != message.parameters.end(); ++itr)
        std::cout << *itr << " ";
    std::cout << std::endl;
}
