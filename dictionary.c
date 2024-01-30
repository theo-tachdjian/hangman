#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>


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


struct SecretWord *choose_secret_word_from_file(const char* filename, const char* difficulty, const char* category)
{
    //printf("ARGS: File: %s | Category: %s | Difficulty: %s\n", filename, category, difficulty);
    // open file stream
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s.\n", filename);
        return 0;
    }

    int lineNum = 0;
    int wordCount = 0;
    // create buffer to store our line
    char line[MAX_LINE_BUFF_SIZE];

    // create buffer for all words read
    char words[MAX_WORD_BUFF_SIZE][32];
    // create buffer for all categories read (needed if target category is NULL)
    char categories[MAX_WORD_BUFF_SIZE][32];

    while (fgets(line, sizeof(line), file) && wordCount < MAX_WORD_BUFF_SIZE) {
        lineNum++;

        // skip comments
        if (line[0] == '#') { continue; }

        char wordBuff[32];
        char categoryBuff[32];
        char difficultyBuff[32];

        // Parse word, category and difficulty from line
        int numFields = sscanf(line, "%49[^,],%49[^,],%9s", wordBuff, categoryBuff, difficultyBuff);

        // check if line and difficulty is valid
        if (numFields < 2 || numFields > 3 || (numFields == 3 && strcmp(difficultyBuff, EASY) != 0 && strcmp(difficultyBuff, MEDIUM) != 0 && strcmp(difficultyBuff, HARD) != 0)) {
            printf("Error in dictionary at line %d: %s\n", lineNum, line);
        } else {
            // if the difficulty and category matches
            //printf("LINE: %s\nWord: %s | Category: %s | Difficulty: %s\n", line, wordBuff, categoryBuff, difficultyBuff);

            if (strcmp(difficultyBuff, difficulty) == 0 && (category == NULL || strcmp(categoryBuff, category) == 0))
            {
                strcpy(words[wordCount], wordBuff);
                strcpy(categories[wordCount], categoryBuff);
                wordCount++;
            }
        }
    }

    fclose(file);

    if (wordCount == 0) {
        printf("No words found for the specified category and difficulty.\n");
        return 0;
    }

    // allocate memory for SecretWord struct
    struct SecretWord *sw_ptr = (struct SecretWord*)malloc(sizeof(struct SecretWord));

    // choose random word
    srand(time(NULL));
    int randomIndex = rand() % wordCount;
    strcpy(sw_ptr->word, words[randomIndex]);
    strcpy(sw_ptr->category, categories[randomIndex]);
    strcpy(sw_ptr->difficulty, difficulty);

    return sw_ptr;
}


struct SecretWord *choose_secret_word_auto()
{
    // choose random word from file "default.txt"
    // choose random difficulty
    srand(time(NULL));
    int randomIndex = rand() % 10;
    if (randomIndex < 3)
    {
        return choose_secret_word_from_file("default.txt", EASY, NULL);
    }
    else if (randomIndex >= 8)
    {
        return choose_secret_word_from_file("default.txt", HARD, NULL);
    }
    return choose_secret_word_from_file("default.txt", MEDIUM, NULL);
}

/*
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}


struct SecretWord *choose_secret_word_by_remote(const char* difficulty, const char* category)
{
    CURL *curl_handle;
    static const char *out_filename = "remote_dictionary.txt";
    FILE *pagefile;

    curl_global_init(CURL_GLOBAL_ALL);

    //init the curl session
    curl_handle = curl_easy_init();

    //set URL to get here
    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://erwan-blancard.students-laplateforme.io/dictionnaire.txt");

    //Switch on full protocol/debug output while testing
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

    //disable progress meter, set to 0L to enable it
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

    //send all data to this function
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

    //open the file
    pagefile = fopen(out_filename, "wb");
    if(pagefile) {

        //write the page body to this file handle
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

        //get it!
        curl_easy_perform(curl_handle);

        //close the header file
        fclose(pagefile);
    }

    //cleanup curl stuff
    curl_easy_cleanup(curl_handle);

    curl_global_cleanup();

    return choose_secret_word_from_file(out_filename, difficulty, category);
}
*/
