/*
Program: ASCII ART Generator
Description: Design text with ascii art
Author: Lord Hypersonic
Email: lordhypersonic.522@gmail.com
Website: www.lordhypersonic.blogspot.com
*/
#include <iostream>
#include <string>

using namespace std;

string ascii_art(string input)
{
    string mssg;
    //loop will print first layer
    for (int i=0; i < input.size(); i++)
    {
        if (input[i] == 'A' || input[i]== 'a')
            mssg += "  ___   ";
        if (input[i] == 'B' || input[i] == 'b')
            mssg += " _____  ";
        if (input[i] == 'C' || input[i] == 'c')
            mssg += " _____  ";
        if (input[i] == 'D' || input[i] == 'd')
            mssg += " _____  ";
        if (input[i] == 'E' || input[i] == 'e')
            mssg += " _____  ";
        if (input[i] == 'F' || input[i] == 'f')
            mssg += " _____  ";
        if (input[i] == 'G' || input[i] == 'g')
            mssg += " _____  ";
        if (input[i] == 'H' || input[i] == 'h')
            mssg += " _   _  ";
        if (input[i] == 'I' || input[i] == 'i')
            mssg += " _____  ";
        if (input[i] == 'J' || input[i] == 'j')
            mssg += "   ___  ";
        if (input[i] == 'K' || input[i] == 'k')
            mssg += " _   __ ";
        if (input[i] == 'L' || input[i] == 'l')
            mssg += " _      ";
        if (input[i] == 'M' || input[i] == 'm')
            mssg += " __  __  ";
        if (input[i] == 'N' || input[i] == 'n')
            mssg += " _   _  ";
        if (input[i] == 'O' || input[i] == 'o')
            mssg += " _____  ";
        if (input[i] == 'P' || input[i] == 'p')
            mssg += " _____  ";
        if (input[i] == 'Q' || input[i] == 'q')
            mssg += " _____  ";
        if (input[i] == 'R' || input[i] == 'r')
            mssg += " _____  ";
        if (input[i] == 'S' || input[i] == 's')
            mssg += " _____  ";
        if (input[i] == 'T' || input[i] == 't')
            mssg += " _____  ";
        if (input[i] == 'U' || input[i] == 'u')
            mssg += " _   _  ";
        if (input[i] == 'V' || input[i] == 'v')
            mssg += " _   _  ";
        if (input[i] == 'W' || input[i] == 'w')
            mssg += " _    _  ";
        if (input[i] == 'X' || input[i] == 'x')
            mssg += "__   __ ";
        if (input[i] == 'Y' || input[i] == 'y')
            mssg += "__   __ ";
        if (input[i] == 'Z' || input[i]== 'z')
            mssg += " ______ ";
        if (input[i] == ' ')
            mssg += "  ";
        if (input[i] == '`')
            mssg += " _  ";
        if (input[i] == '~')
            mssg += "      ";
        if (input[i] == '1')
            mssg += " __   ";
        if (input[i]== '2')
            mssg += " _____  ";
        if (input[i]== '3')
            mssg += " _____  ";
        if (input[i] == '4')
            mssg += "   ___  ";
        if (input[i] == '5')
            mssg += " _____  ";
        if (input[i] == '6')
            mssg += "  ____  ";
        if (input[i] == '7')
            mssg += " ______ ";
        if(input[i] == '.')
            mssg += "    ";
        if (input[i] == '8')
            mssg += " _____  ";
        if (input[i] == '9')
            mssg += " _____  ";
        if (input[i] == '0')
            mssg += " _____  ";
        if (input[i] == '!')
            mssg += " _  ";
        if (input[i] == '@')
            mssg += "   ____   ";
        if (input[i] == '#')
            mssg += "   _  _    ";
        if (input[i] == '$')
            mssg += "  _   ";
        if (input[i] == '%')
            mssg += " _   __ ";
        if (input[i] == '^')
            mssg += " /\\  ";
        if (input[i] == '&')
            mssg += "         ";
        if (input[i] == '*')
            mssg += "    _     ";
        if (input[i] == '(')
            mssg += "  __ ";
        if (input[i] == ')')
            mssg += "__   ";
        if (input[i] == '-')
            mssg += "         ";
        if (input[i] == '_')
            mssg += "         ";
        if (input[i] == '=')
            mssg += "         ";
        if (input[i] == '+')
            mssg += "        ";
        if (input[i] == '[')
            mssg += " ___  ";
        if (input[i] == '{')
            mssg += "   __ ";
        if (input[i]== ']')
            mssg += " ___  ";
        if (input[i] == '}')
            mssg += "__    ";
        if (input[i] == '|')
            mssg += " _  ";
        if (input[i] == '\\')
            mssg += "__      ";
        if (input[i] == ';')
            mssg += " _  ";
        if (input[i] == ':')
            mssg += "    ";
        if (input[i] == '\'')
            mssg += " _  ";
        if (input[i] == '"')
            mssg += " _ _  ";
        if (input[i] == '<')
            mssg += "   __ ";
        if (input[i] == ',')
            mssg += "    ";
        if (input[i] == '>')
            mssg += "__    ";
        if (input[i] == '/')
            mssg += "     __ ";
        if (input[i] == '?')
            mssg += " ___   ";
    }
    mssg += "\n";
    //loop will print second layer
    for (int i=0; i < input.size(); i++)
    {
        if (input[i] == 'A' || input[i]== 'a')
            mssg += " / _ \\  ";
        if (input[i] == 'B' || input[i] == 'b')
            mssg += "| ___ \\ ";
        if (input[i] == 'C' || input[i] == 'c')
            mssg += "/  __ \\ ";
        if (input[i] == 'D' || input[i] == 'd')
            mssg += "|  _  \\ ";
        if (input[i] == 'E' || input[i] == 'e')
            mssg += "|  ___| ";
        if (input[i] == 'F' || input[i] == 'f')
            mssg += "|  ___| ";
        if (input[i] == 'G' || input[i] == 'g')
            mssg += "|  __ \\ ";
        if (input[i] == 'H' || input[i] == 'h')
            mssg += "| | | | ";
        if (input[i] == 'I' || input[i] == 'i')
            mssg += "|_   _| ";
        if (input[i] == 'J' || input[i] == 'j')
            mssg += "  |_  | ";
        if (input[i] == 'K' || input[i] == 'k')
            mssg += "| | / / ";
        if (input[i] == 'L' || input[i] == 'l')
            mssg += "| |     ";
        if (input[i] == 'M' || input[i] == 'm')
            mssg += "|  \\/  | ";
        if (input[i] == 'N' || input[i] == 'n')
            mssg += "| \\ | | ";
        if (input[i] == 'O' || input[i] == 'o')
            mssg += "|  _  | ";
        if (input[i] == 'P' || input[i] == 'p')
            mssg += "| ___ \\ ";
        if (input[i] == 'Q' || input[i] == 'q')
            mssg += "|  _  | ";
        if (input[i] == 'R' || input[i] == 'r')
            mssg += "| ___ \\ ";
        if (input[i] == 'S' || input[i] == 's')
            mssg += "/  ___| ";
        if (input[i] == 'T' || input[i] == 't')
            mssg += "|_   _| ";
        if (input[i] == 'U' || input[i] == 'u')
            mssg += "| | | | ";
        if (input[i] == 'V' || input[i] == 'v')
            mssg += "| | | | ";
        if (input[i] == 'W' || input[i] == 'w')
            mssg += "| |  | | ";
        if (input[i] == 'X' || input[i] == 'x')
            mssg += "\\ \\ / / ";
        if (input[i] == 'Y' || input[i] == 'y')
            mssg += "\\ \\ / / ";
        if (input[i] == 'Z' || input[i]== 'z')
            mssg += "|___  / ";
        if (input[i] == ' ')
            mssg += "  ";
        if (input[i] == '`')
            mssg += "( ) ";
        if (input[i] == '~')
            mssg += "      ";
        if (input[i] == '1')
            mssg += "/  |  ";
        if (input[i]== '2')
            mssg += "/ __  \\ ";
        if (input[i]== '3')
            mssg += "|____ | ";
        if (input[i] == '4')
            mssg += "  /   | ";
        if (input[i] == '5')
            mssg += "|  ___| ";
        if (input[i] == '6')
            mssg += " / ___| ";
        if (input[i] == '7')
            mssg += "|___  / ";
        if(input[i] == '.')
            mssg += "    ";
        if (input[i] == '8')
            mssg += "|  _  | ";
        if (input[i] == '9')
            mssg += "|  _  | ";
        if (input[i] == '0')
            mssg += "|  _  | ";
        if (input[i] == '!')
            mssg += "| | ";
        if (input[i] == '@')
            mssg += "  / __ \\  ";
        if (input[i] == '#')
            mssg += " _| || |_  ";
        if (input[i] == '$')
            mssg += " | |  ";
        if (input[i] == '%')
            mssg += "(_) / / ";
        if (input[i] == '^')
            mssg += "|/\\| ";
        if (input[i] == '&')
            mssg += "  ___    ";
        if (input[i] == '*')
            mssg += " /\\| |/\\  ";
        if (input[i] == '(')
            mssg += " / / ";
        if (input[i] == ')')
            mssg += "\\ \\  ";
        if (input[i] == '-')
            mssg += "         ";
        if (input[i] == '_')
            mssg += "         ";
        if (input[i] == '=')
            mssg += " ______  ";
        if (input[i] == '+')
            mssg += "   _    ";
        if (input[i] == '[')
            mssg += "|  _| ";
        if (input[i] == '{')
            mssg += "  / / ";
        if (input[i]== ']')
            mssg += "|_  | ";
        if (input[i] == '}')
            mssg += "\\ \\   ";
        if (input[i] == '|')
            mssg += "| | ";
        if (input[i] == '\\')
            mssg += "\\ \\     ";
        if (input[i] == ';')
            mssg += "(_) ";
        if (input[i] == ':')
            mssg += " _  ";
        if (input[i] == '\'')
            mssg += "( ) ";
        if (input[i] == '"')
            mssg += "( | ) ";
        if (input[i] == '<')
            mssg += "  / / ";
        if (input[i] == ',')
            mssg += "    ";
        if (input[i] == '>')
            mssg += "\\ \\   ";
        if (input[i] == '/')
            mssg += "    / / ";
        if (input[i] == '?')
            mssg += "|__ \\  ";
    }
    mssg += "\n";
    //loop will print third layer
    for (int i=0; i < input.size(); i++)
    {
        if (input[i] == 'A' || input[i]== 'a')
            mssg += "/ /_\\ \\ ";
        if (input[i] == 'B' || input[i] == 'b')
            mssg += "| |_/ / ";
        if (input[i] == 'C' || input[i] == 'c')
            mssg += "| /  \\/ ";
        if (input[i] == 'D' || input[i] == 'd')
            mssg += "| | | | ";
        if (input[i] == 'E' || input[i] == 'e')
            mssg += "| |__   ";
        if (input[i] == 'F' || input[i] == 'f')
            mssg += "| |_    ";
        if (input[i] == 'G' || input[i] == 'g')
            mssg += "| |  \\/ ";
        if (input[i] == 'H' || input[i] == 'h')
            mssg += "| |_| | ";
        if (input[i] == 'I' || input[i] == 'i')
            mssg += "  | |   ";
        if (input[i] == 'J' || input[i] == 'j')
            mssg += "    | | ";
        if (input[i] == 'K' || input[i] == 'k')
            mssg += "| |/ /  ";
        if (input[i] == 'L' || input[i] == 'l')
            mssg += "| |     ";
        if (input[i] == 'M' || input[i] == 'm')
            mssg += "| .  . | ";
        if (input[i] == 'N' || input[i] == 'n')
            mssg += "|  \\| | ";
        if (input[i] == 'O' || input[i] == 'o')
            mssg += "| | | | ";
        if (input[i] == 'P' || input[i] == 'p')
            mssg += "| |_/ / ";
        if (input[i] == 'Q' || input[i] == 'q')
            mssg += "| | | | ";
        if (input[i] == 'R' || input[i] == 'r')
            mssg += "| |_/ / ";
        if (input[i] == 'S' || input[i] == 's')
            mssg += "\\ `--.  ";
        if (input[i] == 'T' || input[i] == 't')
            mssg += "  | |   ";
        if (input[i] == 'U' || input[i] == 'u')
            mssg += "| | | | ";
        if (input[i] == 'V' || input[i] == 'v')
            mssg += "| | | | ";
        if (input[i] == 'W' || input[i] == 'w')
            mssg += "| |  | | ";
        if (input[i] == 'X' || input[i] == 'x')
            mssg += " \\ V /  ";
        if (input[i] == 'Y' || input[i] == 'y')
            mssg += " \\ V /  ";
        if (input[i] == 'Z' || input[i]== 'z')
            mssg += "   / /  ";
        if (input[i] == ' ')
            mssg += "  ";
        if (input[i] == '`')
            mssg += " \\| ";
        if (input[i] == '~')
            mssg += " /\\/| ";
        if (input[i] == '1')
            mssg += "`| |  ";
        if (input[i]== '2')
            mssg += "`' / /' ";
        if (input[i]== '3')
            mssg += "    / / ";
        if (input[i] == '4')
            mssg += " / /| | ";
        if (input[i] == '5')
            mssg += "|___ \\  ";
        if (input[i] == '6')
            mssg += "/ /___  ";
        if (input[i] == '7')
            mssg += "   / /  ";
        if(input[i] == '.')
            mssg += "    ";
        if (input[i] == '8')
            mssg += " \\ V /  ";
        if (input[i] == '9')
            mssg += "| |_| | ";
        if (input[i] == '0')
            mssg += "| |/' | ";
        if (input[i] == '!')
            mssg += "| | ";
        if (input[i] == '@')
            mssg += " / / _` | ";
        if (input[i] == '#')
            mssg += "|_  __  _| ";
        if (input[i] == '$')
            mssg += "/ __) ";
        if (input[i] == '%')
            mssg += "   / /  ";
        if (input[i] == '^')
            mssg += "     ";
        if (input[i] == '&')
            mssg += " ( _ )   ";
        if (input[i] == '*')
            mssg += " \\ ` ' /  ";
        if (input[i] == '(')
            mssg += "| |  ";
        if (input[i] == ')')
            mssg += " | | ";
        if (input[i] == '-')
            mssg += " ______  ";
        if (input[i] == '_')
            mssg += "         ";
        if (input[i] == '=')
            mssg += "|______| ";
        if (input[i] == '+')
            mssg += " _| |_  ";
        if (input[i] == '[')
            mssg += "| |   ";
        if (input[i] == '{')
            mssg += " | |  ";
        if (input[i]== ']')
            mssg += "  | | ";
        if (input[i] == '}')
            mssg += " | |  ";
        if (input[i] == '|')
            mssg += "| | ";
        if (input[i] == '\\')
            mssg += " \\ \\    ";
        if (input[i] == ';')
            mssg += "    ";
        if (input[i] == ':')
            mssg += "(_) ";
        if (input[i] == '\'')
            mssg += "|/  ";
        if (input[i] == '"')
            mssg += " V V  ";
        if (input[i] == '<')
            mssg += " / /  ";
        if (input[i] == ',')
            mssg += "    ";
        if (input[i] == '>')
            mssg += " \\ \\  ";
        if (input[i] == '/')
            mssg += "   / /  ";
        if (input[i] == '?')
            mssg += "   ) | ";
    }
    mssg += "\n";
    //loop will print fourth layer
    for (int i=0; i < input.size(); i++)
    {
        if (input[i] == 'A' || input[i]== 'a')
            mssg += "|  _  | ";
        if (input[i] == 'B' || input[i] == 'b')
            mssg += "| ___ \\ ";
        if (input[i] == 'C' || input[i] == 'c')
            mssg += "| |     ";
        if (input[i] == 'D' || input[i] == 'd')
            mssg += "| | | | ";
        if (input[i] == 'E' || input[i] == 'e')
            mssg += "|  __|  ";
        if (input[i] == 'F' || input[i] == 'f')
            mssg += "|  _|   ";
        if (input[i] == 'G' || input[i] == 'g')
            mssg += "| | __  ";
        if (input[i] == 'H' || input[i] == 'h')
            mssg += "|  _  | ";
        if (input[i] == 'I' || input[i] == 'i')
            mssg += "  | |   ";
        if (input[i] == 'J' || input[i] == 'j')
            mssg += "    | | ";
        if (input[i] == 'K' || input[i] == 'k')
            mssg += "|    \\  ";
        if (input[i] == 'L' || input[i] == 'l')
            mssg += "| |     ";
        if (input[i] == 'M' || input[i] == 'm')
            mssg += "| |\\/| | ";
        if (input[i] == 'N' || input[i] == 'n')
            mssg += "| . ` | ";
        if (input[i] == 'O' || input[i] == 'o')
            mssg += "| | | | ";
        if (input[i] == 'P' || input[i] == 'p')
            mssg += "|  __/  ";
        if (input[i] == 'Q' || input[i] == 'q')
            mssg += "| | | | ";
        if (input[i] == 'R' || input[i] == 'r')
            mssg += "|    /  ";
        if (input[i] == 'S' || input[i] == 's')
            mssg += " `--. \\ ";
        if (input[i] == 'T' || input[i] == 't')
            mssg += "  | |   ";
        if (input[i] == 'U' || input[i] == 'u')
            mssg += "| | | | ";
        if (input[i] == 'V' || input[i] == 'v')
            mssg += "| | | | ";
        if (input[i] == 'W' || input[i] == 'w')
            mssg += "| |/\\| | ";
        if (input[i] == 'X' || input[i] == 'x')
            mssg += " / ^ \\  ";
        if (input[i] == 'Y' || input[i] == 'y')
            mssg += "  \\ /   ";
        if (input[i] == 'Z' || input[i]== 'z')
            mssg += "  / /   ";
        if (input[i] == ' ')
            mssg += "  ";
        if (input[i] == '`')
            mssg += "    ";
        if (input[i] == '~')
            mssg += "|/\\/  ";
        if (input[i] == '1')
            mssg += " | |  ";
        if (input[i]== '2')
            mssg += "  / /   ";
        if (input[i]== '3')
            mssg += "    \\ \\ ";
        if (input[i] == '4')
            mssg += "/ /_| | ";
        if (input[i] == '5')
            mssg += "    \\ \\ ";
        if (input[i] == '6')
            mssg += "| ___ \\ ";
        if (input[i] == '7')
            mssg += "  / /   ";
        if(input[i] == '.')
            mssg += "    ";
        if (input[i] == '8')
            mssg += " / _ \\  ";
        if (input[i] == '9')
            mssg += "\\____ | ";
        if (input[i] == '0')
            mssg += "|  /| | ";
        if (input[i] == '!')
            mssg += "| | ";
        if (input[i] == '@')
            mssg += "| | (_| | ";
        if (input[i] == '#')
            mssg += " _| || |_  ";
        if (input[i] == '$')
            mssg += "\\__ \\ ";
        if (input[i] == '%')
            mssg += "  / /   ";
        if (input[i] == '^')
            mssg += "     ";
        if (input[i] == '&')
            mssg += " / _ \\/\\ ";
        if (input[i] == '*')
            mssg += "|_     _| ";
        if (input[i] == '(')
            mssg += "| |  ";
        if (input[i] == ')')
            mssg += " | | ";
        if (input[i] == '-')
            mssg += "|______| ";
        if (input[i] == '_')
            mssg += "         ";
        if (input[i] == '=')
            mssg += " ______  ";
        if (input[i] == '+')
            mssg += "|_   _| ";
        if (input[i] == '[')
            mssg += "| |   ";
        if (input[i] == '{')
            mssg += "< <   ";
        if (input[i]== ']')
            mssg += "  | | ";
        if (input[i] == '}')
            mssg += "  > > ";
        if (input[i] == '|')
            mssg += "| | ";
        if (input[i] == '\\')
            mssg += "  \\ \\   ";
        if (input[i] == ';')
            mssg += " _  ";
        if (input[i] == ':')
            mssg += "    ";
        if (input[i] == '\'')
            mssg += "    ";
        if (input[i] == '"')
            mssg += "      ";
        if (input[i] == '<')
            mssg += "< <   ";
        if (input[i] == ',')
            mssg += " _  ";
        if (input[i] == '>')
            mssg += "  > > ";
        if (input[i] == '/')
            mssg += "  / /   ";
        if (input[i] == '?')
            mssg += "  / /  ";
    }
    mssg += "\n";
    //loop will print fifth layer
    for (int i=0; i < input.size(); i++)
    {
        if (input[i] == 'A' || input[i]== 'a')
            mssg += "| | | | ";
        if (input[i] == 'B' || input[i] == 'b')
            mssg += "| |_/ / ";
        if (input[i] == 'C' || input[i] == 'c')
            mssg += "| \\__/\\ ";
        if (input[i] == 'D' || input[i] == 'd')
            mssg += "| |/ /  ";
        if (input[i] == 'E' || input[i] == 'e')
            mssg += "| |___  ";
        if (input[i] == 'F' || input[i] == 'f')
            mssg += "| |     ";
        if (input[i] == 'G' || input[i] == 'g')
            mssg += "| |_\\ \\ ";
        if (input[i] == 'H' || input[i] == 'h')
            mssg += "| | | | ";
        if (input[i] == 'I' || input[i] == 'i')
            mssg += " _| |_  ";
        if (input[i] == 'J' || input[i] == 'j')
            mssg += "/\\__/ / ";
        if (input[i] == 'K' || input[i] == 'k')
            mssg += "| |\\  \\ ";
        if (input[i] == 'L' || input[i] == 'l')
            mssg += "| |____ ";
        if (input[i] == 'M' || input[i] == 'm')
            mssg += "| |  | | ";
        if (input[i] == 'N' || input[i] == 'n')
            mssg += "| |\\  | ";
        if (input[i] == 'O' || input[i] == 'o')
            mssg += "\\ \\_/ / ";
        if (input[i] == 'P' || input[i] == 'p')
            mssg += "| |     ";
        if (input[i] == 'Q' || input[i] == 'q')
            mssg += "\\ \\/' / ";
        if (input[i] == 'R' || input[i] == 'r')
            mssg += "| |\\ \\  ";
        if (input[i] == 'S' || input[i] == 's')
            mssg += "/\\__/ / ";
        if (input[i] == 'T' || input[i] == 't')
            mssg += "  | |   ";
        if (input[i] == 'U' || input[i] == 'u')
            mssg += "| |_| | ";
        if (input[i] == 'V' || input[i] == 'v')
            mssg += "\\ \\_/ / ";
        if (input[i] == 'W' || input[i] == 'w')
            mssg += "\\  /\\  / ";
        if (input[i] == 'X' || input[i] == 'x')
            mssg += "/ / \\ \\ ";
        if (input[i] == 'Y' || input[i] == 'y')
            mssg += "  | |   ";
        if (input[i] == 'Z' || input[i]== 'z')
            mssg += "./ /___ ";
        if (input[i] == ' ')
            mssg += "  ";
        if (input[i] == '`')
            mssg += "    ";
        if (input[i] == '~')
            mssg += "      ";
        if (input[i] == '1')
            mssg += "_| |_ ";
        if (input[i]== '2')
            mssg += "./ /___ ";
        if (input[i]== '3')
            mssg += ".___/ / ";
        if (input[i] == '4')
            mssg += "\\___  | ";
        if (input[i] == '5')
            mssg += "/\\__/ / ";
        if (input[i] == '6')
            mssg += "| \\_/ | ";
        if (input[i] == '7')
            mssg += "./ /    ";
        if(input[i] == '.')
            mssg += " _  ";
        if (input[i] == '8')
            mssg += "| |_| | ";
        if (input[i] == '9')
            mssg += ".___/ / ";
        if (input[i] == '0')
            mssg += "\\ |_/ / ";
        if (input[i] == '!')
            mssg += "|_| ";
        if (input[i] == '@')
            mssg += " \\ \\__,_| ";
        if (input[i] == '#')
            mssg += "|_  __  _| ";
        if (input[i] == '$')
            mssg += "(   / ";
        if (input[i] == '%')
            mssg += " / / _  ";
        if (input[i] == '^')
            mssg += "     ";
        if (input[i] == '&')
            mssg += "| (_>  < ";
        if (input[i] == '*')
            mssg += " / , . \\  ";
        if (input[i] == '(')
            mssg += "| |  ";
        if (input[i] == ')')
            mssg += " | | ";
        if (input[i] == '-')
            mssg += "         ";
        if (input[i] == '_')
            mssg += " ______  ";
        if (input[i] == '=')
            mssg += "|______| ";
        if (input[i] == '+')
            mssg += "  |_|   ";
        if (input[i] == '[')
            mssg += "| |_  ";
        if (input[i] == '{')
            mssg += " | |  ";
        if (input[i]== ']')
            mssg += " _| | ";
        if (input[i] == '}')
            mssg += " | |  ";
        if (input[i] == '|')
            mssg += "| | ";
        if (input[i] == '\\')
            mssg += "   \\ \\  ";
        if (input[i] == ';')
            mssg += "( ) ";
        if (input[i] == ':')
            mssg += " _  ";
        if (input[i] == '\'')
            mssg += "    ";
        if (input[i] == '"')
            mssg += "      ";
        if (input[i] == '<')
            mssg += " \\ \\  ";
        if (input[i] == ',')
            mssg += "( ) ";
        if (input[i] == '>')
            mssg += " / /  ";
        if (input[i] == '/')
            mssg += " / /    ";
        if (input[i] == '?')
            mssg += " |_|   ";
    }
    mssg += "\n";
    //loop will print sixth layer
    for (int i=0; i < input.size(); i++)
    {
        if (input[i] == 'A' || input[i]== 'a')
            mssg += "\\_| |_/ ";
        if (input[i] == 'B' || input[i] == 'b')
            mssg += "\\____/  ";
        if (input[i] == 'C' || input[i] == 'c')
            mssg += " \\____/ ";
        if (input[i] == 'D' || input[i] == 'd')
            mssg += "|___/   ";
        if (input[i] == 'E' || input[i] == 'e')
            mssg += "\\____/  ";
        if (input[i] == 'F' || input[i] == 'f')
            mssg += "\\_|     ";
        if (input[i] == 'G' || input[i] == 'g')
            mssg += " \\____/ ";
        if (input[i] == 'H' || input[i] == 'h')
            mssg += "\\_| |_/ ";
        if (input[i] == 'I' || input[i] == 'i')
            mssg += " \\___/  ";
        if (input[i] == 'J' || input[i] == 'j')
            mssg += "\\____/  ";
        if (input[i] == 'K' || input[i] == 'k')
            mssg += "\\_| \\_/ ";
        if (input[i] == 'L' || input[i] == 'l')
            mssg += "\\_____/ ";
        if (input[i] == 'M' || input[i] == 'm')
            mssg += "\\_|  |_/ ";
        if (input[i] == 'N' || input[i] == 'n')
            mssg += "\\_| \\_/ ";
        if (input[i] == 'O' || input[i] == 'o')
            mssg += " \\___/  ";
        if (input[i] == 'P' || input[i] == 'p')
            mssg += "\\_|     ";
        if (input[i] == 'Q' || input[i] == 'q')
            mssg += " \\_/\\_\\ ";
        if (input[i] == 'R' || input[i] == 'r')
            mssg += "\\_| \\_| ";
        if (input[i] == 'S' || input[i] == 's')
            mssg += "\\____/  ";
        if (input[i] == 'T' || input[i] == 't')
            mssg += "  \\_/   ";
        if (input[i] == 'U' || input[i] == 'u')
            mssg += " \\___/  ";
        if (input[i] == 'V' || input[i] == 'v')
            mssg += " \\___/  ";
        if (input[i] == 'W' || input[i] == 'w')
            mssg += " \\/  \\/  ";
        if (input[i] == 'X' || input[i] == 'x')
            mssg += "\\/   \\/ ";
        if (input[i] == 'Y'  || input[i] == 'y')
            mssg += "  \\_/   ";
        if (input[i] == 'Z' || input[i]== 'z')
            mssg += "\\_____/ ";
        if (input[i] == ' ')
            mssg += "  ";
        if (input[i] == '`')
            mssg += "    ";
        if (input[i] == '~')
            mssg += "      ";
        if (input[i] == '1')
            mssg += "\\___/ ";
        if (input[i]== '2')
            mssg += "\\_____/ ";
        if (input[i]== '3')
            mssg += "\\____/  ";
        if (input[i] == '4')
            mssg += "    |_/ ";
        if (input[i] == '5')
            mssg += "\\____/  ";
        if (input[i] == '6')
            mssg += "\\_____/ ";
        if (input[i] == '7')
            mssg += "\\_/     ";
        if(input[i] == '.')
            mssg += "(_) ";
        if (input[i] == '8')
            mssg += "\\_____/ ";
        if (input[i] == '9')
            mssg += "\\____/  ";
        if (input[i] == '0')
            mssg += " \\___/  ";
        if (input[i] == '!')
            mssg += "(_) ";
        if (input[i] == '@')
            mssg += "  \\____/  ";
        if (input[i] == '#')
            mssg += "  |_||_|   ";
        if (input[i] == '$')
            mssg += " |_|  ";
        if (input[i] == '%')
            mssg += "/_/ (_) ";
        if (input[i] == '^')
            mssg += "     ";
        if (input[i] == '&')
            mssg += " \\___/\\/ ";
        if (input[i] == '*')
            mssg += " \\/|_|\\/  ";
        if (input[i] == '(')
            mssg += " \\_\\ ";
        if (input[i] == ')')
            mssg += "/_/  ";
        if (input[i] == '-')
            mssg += "         ";
        if (input[i] == '_')
            mssg += "|______| ";
        if (input[i] == '=')
            mssg += "         ";
        if (input[i] == '+')
            mssg += "        ";
        if (input[i] == '[')
            mssg += "|___| ";
        if (input[i] == '{')
            mssg += "  \\_\\ ";
        if (input[i]== ']')
            mssg += "|___| ";
        if (input[i] == '}')
            mssg += "/_/   ";
        if (input[i] == '|')
            mssg += "|_| ";
        if (input[i] == '\\')
            mssg += "    \\_\\ ";
        if (input[i] == ';')
            mssg += "|/  ";
        if (input[i] == ':')
            mssg += "(_) ";
        if (input[i] == '\'')
            mssg += "    ";
        if (input[i] == '"')
            mssg += "      ";
        if (input[i] == '<')
            mssg += "  \\_\\ ";
        if (input[i] == ',')
            mssg += "|/  ";
        if (input[i] == '>')
            mssg += "/_/   ";
        if (input[i] == '/')
            mssg += "/_/     ";
        if (input[i] == '?')
            mssg += " (_)   ";
    }
    mssg += "\n";
    return mssg;
}

int main()
{

    std::string sd;
    sd = ascii_art("zenon");
    cout<< sd;
    cout<<endl;
    return 0;
}