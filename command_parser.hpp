#ifndef _COMMAND_PARSER_H
#define _COMMAND_PARSER_H

#include <vector>
#include <string>
#include <iostream>

typedef struct _CommandVar
{
    int index;
    std::string key;
    int value;
}CommandVar, *PCommandVar;


class CommandParser
{

private:
    static std::vector<CommandVar> CommandVars;
public:
    // CommandParser();
    // ~CommandParser();
    

    static CommandVar GetVar(int index){
        if (index > (CommandVars.size() - 1)){
            std::cout << "GetVar: İndex Hatalı\n";
            return { -1, "", 0 };
        }
        return CommandVars.at(index);
    }

    static CommandVar GetVar(std::string key){
        for (CommandVar var : CommandVars){
            if (var.key == key){
                return var;
            }
        }
        return { -1, "", 0 };
    }

    static bool SetVar(std::string key, int value){
        for(CommandVar var : CommandVars){
            if (var.key == key){
                CommandVars.at(var.index).value = value;
                return true;
            }
        }
        int index = 0;
        if(CommandVars.size()){
            index = CommandVars.size() - 1;
        }
        CommandVars.push_back({ index, key, value });
        return true;
    }

    static bool SetVar(int index, int value){
        if (index > (CommandVars.size() - 1)){
            std::cout << "SetVar: İndex Hatalı\n";
            return false;
        }
        CommandVars.at(index).value = value;
        return true;
    }

    static std::vector<std::string> SplitCommand(const std::string& str, const std::string& delim){
        std::vector<std::string> tokens;
        size_t prev = 0, pos = 0;
        do
        {
            pos = str.find(delim, prev);
            if (pos == std::string::npos) pos = str.length();
            std::string token = str.substr(prev, pos-prev);
            if (!token.empty()) tokens.push_back(token);
            prev = pos + delim.length();
        }
        while (pos < str.length() && prev < str.length());
        return tokens;
    }

    static bool isNumeric(const std::string& str){
        for (char const &c : str) {
            if (std::isdigit(c) == 0) return false;
        }
        return true;
    }

    static bool ParseCommand(std::string line){
        CommandVar FirstVar;
        CommandVar SecondVar;
        std::vector<std::string> tokens = SplitCommand(line, " ");

        if (tokens.size() == 2){
            if (tokens.at(0) == "out"){
                FirstVar = GetVar(tokens.at(1));
                if(FirstVar.index == -1){
                    std::cout << "ParseCommand: Çıkış Değişkeni Bulunamadı\n";
                    return false;
                }
                FILE* f = fopen("out.txt", "w");
                fprintf(f, "%d\n", FirstVar.value);
                fclose(f);
                return true;
            }
            return false;
        }

        if (tokens.size() != 3){
            std::cout << "ParseCommand: Komut Uzunluğu Hatalı\n";
            return false;
        }
        
        if(isNumeric(tokens.at(0))){
            std::cout << "ParseCommand: İlk Değişken Sayı Olamaz\n";
            return false;
        }
        int value;
        bool Numeric = false;
        if(isNumeric(tokens.at(2))){
            Numeric = true;
            value = stoi(tokens.at(2));
        }
        
        FirstVar = GetVar(tokens.at(0));
        if(!Numeric){
            SecondVar = GetVar(tokens.at(2));
            if(SecondVar.index == -1){
                std::cout << "ParseCommand: İkinci Değişken Bulunamadı\n";
                return false;
            }
        }


        if (tokens.at(1) == "="){
            if(Numeric){
                if(SetVar(tokens.at(0), value))
                    return true;
            }
            else{
                if(SetVar(tokens.at(0), SecondVar.value))
                    return true;
            }
            return false;
        }
        
        if(FirstVar.index == -1){
            std::cout << "ParseCommand: İlk Değişken Bulunamadı\n";
            return false;
        }


        if (tokens.at(1) == "+"){
            if(Numeric){
                if(SetVar(FirstVar.index, FirstVar.value + value))
                    return true;
            }
            else{
                if(SetVar(FirstVar.index, FirstVar.value + SecondVar.value))
                    return true;
            }
        }
        else if (tokens.at(1) == "-"){
            if(Numeric){
                if(SetVar(FirstVar.index, FirstVar.value - value))
                    return true;
            }
            else{
                if(SetVar(FirstVar.index, FirstVar.value - SecondVar.value))
                    return true;
            }
        }
        else if (tokens.at(1) == "*"){
            if(Numeric){
                if(SetVar(FirstVar.index, FirstVar.value * value))
                    return true;
            }
            else{
                if(SetVar(FirstVar.index, FirstVar.value * SecondVar.value))
                    return true;
            }
        }
        else if (tokens.at(1) == "/"){
            if(Numeric){
                if(SetVar(FirstVar.index, FirstVar.value / value))
                    return true;
            }
            else{
                if(SetVar(FirstVar.index, FirstVar.value / SecondVar.value))
                    return true;
            }
        }
        else{
            std::cout << "ParseCommand: İşlem Geçersiz\n";
        }
        return false;
    }
};

std::vector<CommandVar> CommandParser::CommandVars;

#endif