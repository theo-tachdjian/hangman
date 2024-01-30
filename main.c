#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"


void print_help();


struct GameState {
    char letters_typed[26];
    struct SecretWord *secret_word;
};


/*int is_letter_in_alphabet(char c)
{
    return ((c >= "A" && c <= "Z") || (c >= "a" && c <= "z"));
}*/


void print_letters_typed(char* letters, int len)
{
    printf("Letters typed: ");
    for (int i = 0; i < len; i++)
    {
        if (letters[i] != '\0')
        {
            printf("%c", letters[i]);
        }
    }
    printf("\n");
}


int char_in_letters_typed(char c, char* letters, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (letters[i] == c)
        {
            return 1;
        }
    }
    return 0;
}


void print_ui(struct GameState* state, char* userGuess, int incorrectGuesses)
{
    printf("\n");
    printf("Category: %s | Difficulty: %s\n", state->secret_word->category, state->secret_word->difficulty);

    printf("  ----  \n");
    printf("  |  |  \n");
    switch(incorrectGuesses)
    {
    case 0:
        printf("     |  \n");
        printf("     |  \n");
        printf("     |  \n");
        break;
    case 1:
        printf("  0  |  \n");
        printf("     |  \n");
        printf("     |  \n");
        break;
    case 2:
        printf("  0  |  \n");
        printf("  |  |  \n");
        printf("     |  \n");
        break;
    case 3:
        printf("  0  |  \n");
        printf(" /|  |  \n");
        printf("     |  \n");
        break;
    case 4:
        printf("  0  |  \n");
        printf(" /|\\ |  \n");
        printf("     |  \n");
        break;
    default:
        printf("  0  |  \n");
        printf(" /|\\ |  \n");
        printf(" /   |  \n");
        break;
    }
    printf("     |  \n");
    printf("________\n");
    printf("Guess: %s\n", userGuess);
}


void game_loop(struct GameState state)
{
    int secretWordLen = strlen(state.secret_word->word);
    char userGuess[secretWordLen];
    // copy word to userGuess
    strcpy(userGuess, state.secret_word->word);

    // fill userGuess with asterisks
    for (int i = 0; i < secretWordLen; i++)
    {
        // do not hide 1st and last letters or if current letter equals to 1st or last letter
        if (!((i == 0 || i == secretWordLen-1) || !isalpha(state.secret_word->word[i]) || (isalpha(state.secret_word->word[i]) && (state.secret_word->word[i] == state.secret_word->word[0] || state.secret_word->word[i] == state.secret_word->word[secretWordLen-1]))))
        {
            userGuess[i] = '*';
        }
    }

    int incorrectGuesses = 0;

    while (incorrectGuesses < 6) {
        print_ui(&state, userGuess, incorrectGuesses);
        print_letters_typed(state.letters_typed, secretWordLen);
        printf("Attempts remaining: %d\n", 6 - incorrectGuesses);

        char guess;
        printf("Type a letter: ");
        scanf(" %c", &guess);
        guess = tolower(guess);

        // check if letter is valid and if it has not already been typed
        if (isalpha(guess) && !char_in_letters_typed(guess, state.letters_typed, secretWordLen)) {
            state.letters_typed[guess-'a'] = guess;

            int isCorrectGuess = 0;

            for (int i = 0; i < secretWordLen; i++) {
                if (state.secret_word->word[i] == guess) {
                    userGuess[i] = guess;
                    isCorrectGuess = 1;
                }
            }

            if (!isCorrectGuess) {
                incorrectGuesses++;
            }

            if (strcmp(state.secret_word->word, userGuess) == 0) {
                printf("\nCongrats! You guessed the secret word \"%s\" !\n", state.secret_word->word);
                break;
            }
        } else {
            printf("\nPlease enter a valid letter that was not already guessed.\n");
        }
    }

    if (incorrectGuesses == 6) {
        printf("\n  ----  \n");
        printf("  |  |  \n");
        printf("  0  |  \n");
        printf(" /|\\ |  \n");
        printf(" / \\ |  \n");
        printf("     |  \n");
        printf("________\n");
        printf("\nGame Over! The secret word was: \"%s\".\n", state.secret_word->word);
    }

    // END OF GAME LOOP
    // clear letters_typed
    memset(state.letters_typed, '\0', 26);
    // free mem
    free(state.secret_word);
    return;
}


int main(int argc, char const *argv[])
{
    struct GameState state;
    // remove garbage values
    memset(state.letters_typed, '\0', 26);

    if (argc == 1)
    {
        // choose random word from file "default.txt"
        state.secret_word = choose_secret_word_auto();
    }
    else if (argc > 4)
    {
        printf("Too much arguments.");
        print_help();
        return 1;
    }
    else if (argc >= 2)
    {
        if (argc == 3)
        {
            state.secret_word = choose_secret_word_from_file(argv[1], argv[2], argv[3]);
        }
        else
        {
            state.secret_word = choose_secret_word_from_file(argv[1], argv[2], NULL);
        }
    }
    else
    {
        printf("Invalid arguments.");
        print_help();
        return 2;
    }

    if (state.secret_word != 0)
    {
        int running = 1;
        do
        {
            game_loop(state);
            char resp;
            printf("Play again ? [y/n]: ");
            scanf(" %c", &resp);
            if (tolower(resp) == 'y')
            {
                if (argc == 2)
                {
                    state.secret_word = choose_secret_word_from_file(argv[1], argv[2], NULL);
                }
                else if (argc == 3)
                {
                    state.secret_word = choose_secret_word_from_file(argv[1], argv[2], argv[3]);
                }
                else
                {
                    // choose random word from file "default.txt"
                    state.secret_word = choose_secret_word_auto();
                }
                running = 1;
            }
            else
            {
                running = 0;
            }
        } while (running);
    }
    else
    {
        return -1;
    }

    return 0;
}


void print_help()
{
    printf("Usages:\n./hangman\n");
    printf("./hangman <dictionnary-file> [facile/moyen/difficile]\n");
    printf("./hangman <dictionnary-file> [facile/moyen/difficile] <category>\n");
}
