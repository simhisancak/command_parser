#ifndef _COMMAND_PARSER_H
#define _COMMAND_PARSER_H

#include <vector>
#include <string>
#include <iostream>
#include "solve.hpp" //https://github.com/shirnschall/cpp-solve

typedef struct _CommandVar
{
    int index;
    std::string key;
    float value;
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

    static bool SetVar(std::string key, float value){
        if(key.size() != 1){
            std::cout << "SetVar: Key Uzunluğu Sadece 1 Karakter Olabilir\n";
            return false;
        }
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

    static bool SetVar(int index, float value){
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

    static std::string PutVars(std::string Text){
        std::string Alp = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        std::string ret = "";
        for (int i = 0; i < Text.size(); i++){
            char token_char = Text.at(i); 
            std::string token = std::string(1, token_char);
            for (int j = 0; j < Alp.size(); j++){
                if(token_char == Alp.at(j)){
                    CommandVar Var = GetVar(token);
                    if(Var.index == -1){
                        std::cout << "PutVars: Değişken Bulunamadı\n";
                        return "";
                    }
                    token = std::to_string(Var.value);
                    break;
                }
            }
            ret += token;
        }
        return ret;
    }

    static bool ParseCommand(std::string line){
        CommandVar Var;
        std::vector<std::string> tokens = SplitCommand(line, " ");

        if (tokens.size() == 2){
            if (tokens.at(0) == "out"){
                Var = GetVar(tokens.at(1));
                if(Var.index == -1){
                    std::cout << "ParseCommand: Çıkış Değişkeni Bulunamadı\n";
                    return false;
                }
                FILE* f = fopen("out.txt", "w");
                fprintf(f, "%f\n", Var.value);
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
            std::cout << "ParseCommand: Değişken Harf Olmalı\n";
            return false;
        }
        
        if(tokens.at(0).size() != 1){
            std::cout << "ParseCommand: Değişken Uzunluğu Sadece 1 Karakter Olabilir\n";
            return false;
        }

        Var = GetVar(tokens.at(0));
        std::string Transaction_Text = tokens.at(2);
        if(Transaction_Text.size() < 1){
            std::cout << "ParseCommand: İşlem Uzunluğu Çok Az\n";
            return false;
        }
        Transaction_Text = PutVars(Transaction_Text);
        if (tokens.at(1) == "="){
            if(SetVar(tokens.at(0), solve(Transaction_Text.c_str(), 0, Transaction_Text.size(), nullptr)))
                return true;
            return false;
        }
        else{
            std::cout << "ParseCommand: İşlem Geçersiz\n";
        }
        return false;
    }
};

std::vector<CommandVar> CommandParser::CommandVars;

#endif