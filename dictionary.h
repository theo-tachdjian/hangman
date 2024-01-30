#ifndef DICTIONARY
#define DICTIONARY

#define EASY "facile"
#define MEDIUM "moyen"
#define HARD "difficile"

#define MAX_WORD_BUFF_SIZE 256
#define MAX_LINE_BUFF_SIZE 256


struct SecretWord {
    char word[32];
    char difficulty[32];
    char category[32];
};

struct SecretWord *choose_secret_word_from_file(const char* file, const char* difficulty, const char* category);
struct SecretWord *choose_secret_word_auto();
//struct SecretWord *choose_secret_word_by_remote(const char* difficulty, const char* category);

#endif // DICTIONARY
