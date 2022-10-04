#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

void MainMenu();
void PlayHangman();
void GuessLetter(char* guess, char* word, int limit);
void LoseGame(char* guess, char* word);
void WinGame(char* guess, char* word);
string StringToLower(string str);

int maxGuesses = 5;
int remainingGuesses = maxGuesses;
bool enableHelp = false;
bool failedGuessToggle = true;

//int main()
//{
//    MainMenu();
//}

void MainMenu()
{
    string input = "";

    system("CLS");

    cout << "===========================" << endl;
    cout << "HANG MAN - MAIN MENU" << endl;
    cout << "===========================" << endl << endl;

    if (enableHelp == false)
    {
        cout << "Type Help For A List Of Commands" << endl;
    }
    else if (enableHelp == true)
    {
        cout << "play - Starts the game" << endl;
        cout << "guess - Allows you to enter a guess" << endl;
        cout << "config - Allows you to enter a guess" << endl;
        cout << "quit - Exits the application" << endl << endl;

        cout << "Enter Help Again To Hide The Help Menu" << endl << endl;
    }
    
    cout << "Enter A Command Now..." << endl << endl;
    cin >> input;

    input = StringToLower(input);

    if (input == "help")
    {
        enableHelp = !enableHelp;
    }
    else if (input == "play")
    {
        PlayHangman();
    }
    else if (input == "config")
    {
        int tempGuess = 0;

        cout << "Current Max Guesses: " << maxGuesses << endl;
        cout << "Enter new Max Guesses Now." << endl;
        cin >> tempGuess;

        maxGuesses = tempGuess;
    }
    else if (input == "quit")
    {
        return;
    }


    MainMenu();
}

string StringToLower(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    return str;
}

void PlayHangman()
{
    string hangWord = "";

    cout << "Enter Your Word" << endl;
    cin >> hangWord;

    hangWord = StringToLower(hangWord);

    int limit = hangWord.length() + 1;

    char* hangWordArr = new char[limit];

    strcpy(hangWordArr, hangWord.c_str());

    char* guessWordArr = new char[limit];

    for (int i = 0; i < limit; i++)
    {
        guessWordArr[i] = '*';

        if (i == limit - 1)
        {
            guessWordArr[i] = '\0';
        }
    }

    GuessLetter(guessWordArr, hangWordArr, limit);
}

void GuessLetter(char* guess, char* word, int limit)
{
    char input;
    
    system("CLS");

    if (remainingGuesses <= 0)
    {
        LoseGame(guess, word);
    }
    else if (!strcmp(guess, word))
    {
        WinGame(guess, word);
    }

    cout << "===========================" << endl;
    cout << "HANG MAN" << endl;
    cout << "===========================" << endl;
    cout << "[" << guess << "]" << endl << endl;

    cout << "Remaining Guesses: " << remainingGuesses << "/" << maxGuesses << endl;
    cout << "===========================" << endl << endl;

    cout << "Enter a letter" << endl;
    cin >> input;

    input = tolower(input);

    for (int i = 0; i < limit - 1; i++)
    {
        if (word[i] == input)
        {
            guess[i] = input;

            failedGuessToggle = false;
        }
    }

    if (failedGuessToggle == true)
    {
        remainingGuesses--;
    }

    failedGuessToggle = true;
    GuessLetter(guess, word, limit);
}

void LoseGame(char* guess, char* word)
{
    string temp;
    remainingGuesses = maxGuesses;

    system("CLS");

    cout << "===========================" << endl;
    cout << "YOU LOSE - WORTHLESS HUMAN GARBAGE" << endl;
    cout << "===========================" << endl << endl;

    cout << "Press Enter To Return To The Main Menu" << endl;
    system("pause");

    MainMenu();

    delete[] guess;
    delete[] word;
}

void WinGame(char* guess, char* word)
{
    string temp;
    remainingGuesses = maxGuesses;

    system("CLS");

    cout << "===========================" << endl;
    cout << "YOU WIN - YOU ARE A GENTLEMAN AND A SCHOLAR" << endl;
    cout << "===========================" << endl << endl;

    cout << "Press Enter To Return To The Main Menu" << endl;
    system("pause");

    MainMenu();

    delete[] guess;
    delete[] word;
}
