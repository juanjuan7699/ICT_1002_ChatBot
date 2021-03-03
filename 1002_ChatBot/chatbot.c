/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include "chat1002.h"

#define MAX_REPLIES 8

typedef struct userresponse_node {
    char intent[MAX_INTENT];	//char of max intent is 32
    char entity[MAX_ENTITY];	//char of max entity is 64
    char response[MAX_RESPONSE];	// char of response is 256
    struct response_node* next;
} userresponse_node;

//initialise pointers that will point to the head of the intent list
userresponse_node* whoheadpointer, * whatheadpointer, * whereheadpointer;

char* USER_GREETINGS[MAX_REPLIES] = {
        "hello", "hi", "greetings", "sup","hey", "heyyy", "hi!", "helloo"  // contains the strings for smalltalk, change be changed to anything
};
char* USER_THANKS[MAX_REPLIES] = {
        "thanks", "thx", "tq", "thks","tqvm", "tyty", "ty", "thk"  // contains the strings for smalltalk, change be changed to anything
};
char* CHATBOT_GREETINGS[MAX_REPLIES] = {
        "Good %s! How can I help you?", "%s,How can I help you today?", "Hi %s!!","Greetings %s!", "Sup %s?","Hey %s", "Hello %s :D" // contains the responses for the chatbot, change be changed to anything
};
char* CHATBOT_THANKS[MAX_REPLIES] = {
        "You're welcome! How else can I help you?", "No problem. %s!", "%s, no worries.","Don't mention it %s.", "My pleasure!","It was the least I could do. %s", "Hello %s :D" // contains the responses for the chatbot, change be changed to anything
};



/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char* chatbot_botname() {


    return "Chatbot Bob";

}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char* chatbot_username() {

    return "TheZeus";

}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char* inv[], char* response, int n) {

    /* check for empty input */
    if (inc < 1) {
        snprintf(response, n, "");
        return 0;
    }

    /* look for an intent and invoke the corresponding do_* function */
    if (chatbot_is_exit(inv[0]))
        return chatbot_do_exit(inc, inv, response, n);
    else if (chatbot_is_smalltalk(inv[0]))
        return chatbot_do_smalltalk(inc, inv, response, n);
    else if (chatbot_is_load(inv[0]))
        return chatbot_do_load(inc, inv, response, n);
    else if (chatbot_is_question(inv[0]))
        return chatbot_do_question(inc, inv, response, n);
    else if (chatbot_is_reset(inv[0]))
        return chatbot_do_reset(inc, inv, response, n);
    else if (chatbot_is_save(inv[0]))
        return chatbot_do_save(inc, inv, response, n);
    else {
        snprintf(response, n, "I don't understand \"%s\".", inv[0]);
        return 0;
    }

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char* intent) {

    return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;

}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char* inv[], char* response, int n) {

    snprintf(response, n, "Goodbye!");

    return 1;

}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char* intent) {

    if (compare_token(intent, "load") == 0) {
        return 1;
    }
    else {
        return 0;
    }
}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char* inv[], char* response, int n) {
    FILE* fptr;
    int count = 0;

    fptr = inv[1]; // just to test, for debugging

    //file reading, if file is null prompt error
    if ((fptr = fopen(inv[1], "r")) == NULL) {
        snprintf(response, n, "%s cannot be found. Pls try again", inv[1]);
    }
    else { //not null call knowledge_read to read the file
        count = knowledge_read(fptr);
        if (count == -1) {
            snprintf(response, n, "Invalid file. Pls try again.");
        }
        else {
            snprintf(response, n, "Read %d responses from %s", count, inv[1]);
        }
    }



    return 0;
}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char* intent) {

    if (compare_token(intent, "what") == 0) {
        return 1;
    }
    else if (compare_token(intent, "where") == 0) {
        return 1;
    }
    else if (compare_token(intent, "who") == 0) {
        return 1;
    }
    else {
        return 0;
    }


}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char* inv[], char* response, int n) {

    char userintent[MAX_INTENT]; //define  char array to store intent of size MAX_INTENT
    strncpy(userintent, inv[0], sizeof(userintent) / sizeof(userintent[0]));

    char userisare[MAX_INPUT]; //define a char array to store is/are of size MAX_INPUT
    strcpy(userisare, "\0");

    char userentity[MAX_ENTITY]; //define a char array to store entity of size MAX_INTENT
    snprintf(userentity, n, ""); //formats entity

    char fullstring[MAX_RESPONSE]; //define a char array to store suer response of size MAX_RESPONSE
    int KB_code; //Define a int array to store KBcode
    char chatbot_entity[MAX_RESPONSE]; //define a char array to store chatbot response of size MAX_RESPONSE

    //validation checking. checks if intent is one of the 3W and is accompanied with "is" or "are" and entity
    if (inc == 1) {
        if (compare_token(userintent, "what") == 0) {
            snprintf(response, n, "Sorry, I don't understand the word 'what'. Did you meant something like, \"What is ICT?\"");
        }
        else if (compare_token(userintent, "who") == 0) {
            snprintf(response, n, "Sorry, I don't understand the word 'who'. Did you meant something like, \"Who is Frank Guan?\"");
        }
        else if (compare_token(userintent, "where") == 0) {
            snprintf(response, n, "Sorry, I don't understand the word 'where'. Did you meant something like, \"Where is SIT?\"");
        }
        return 0;
    }
    else if ((inc == 2 && compare_token(inv[1], "is") == 0) || (inc == 2 && compare_token(inv[1], "are") == 0)) {
        snprintf(response, n, "Sorry, I don't understand '%s %s'. Please include entities in your question.", inv[0], inv[1]);
        return 0;
    }

    for (int i = 1; i < inc; i++) {
        if ((i == 1 && compare_token(inv[i], "is") == 0) || (i == 1 && compare_token(inv[i], "are") == 0)) {
            strncpy(userisare, inv[i], sizeof(inv[i]) / sizeof(inv[i][0])); //store into userisare
            continue;
        }
        strcat(strcat(userentity, " "), inv[i]); //store into userentity
    }
    memmove(userentity, userentity + 1, strlen(userentity)); //format entity

    //call knowledge_get
    KB_code = knowledge_get(userintent, userentity, chatbot_entity, n);
    if (KB_code == KB_OK) { //if response found
        snprintf(response, n, "%s", chatbot_entity); //response copied to buffer
        return 0;
    }
    else if (KB_code == KB_NOTFOUND) { //else prompt users to input

        prompt_user(fullstring, MAX_INPUT, "I don't understand %s %s %s?", userintent, userisare, userentity);

        // add in knowledge
        KB_code = knowledge_put(userintent, userentity, fullstring);

        //if knowledge added successfully into the linked list
        if (KB_code == KB_OK) {
            snprintf(response, n, "Knowledge added!");
        }

        //if there is no memory in linked list
        else if (KB_code == KB_NOMEM) {
            snprintf(response, n, "Memory allocation failure!");
        }

        //if invalid knowledge is added
        else if (KB_code == KB_INVALID) {
            snprintf(response, n, "Invalid knowledge");
        }
    }

    else if (KB_code == KB_INVALID) {
        snprintf(response, n, "I don't understand '%s'.", userintent);
    }
    return 0;
}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char* intent) {

    if (compare_token(intent, "reset") == 0) {
        return 1;
    }
    else {
        return 0;
    }

}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char* inv[], char* response, int n) {
    knowledge_reset();
    snprintf(response, n, "Chatbot reset.");
    return 0;
    int reset_count = 0;

    /* If who linkedlist is not empty */
    if (whoheadpointer != NULL) {

        /* While it is not end of who linked list, free memory of node whoheadpointer is currently pointed to. */
        userresponse_node* temp = whoheadpointer;
        do {
            printf("\nRemoving Node '%s' '%s\n'", temp->intent, temp->entity);
            temp = temp->next;
            free(whoheadpointer);   /* Free memory allocation of node whoheadpointer is currently pointed to. */
            whoheadpointer = temp;  /* Pointing whoheadpointer to next node. if this node is last in the list, it will point to NULL instead.*/
        } while (temp != NULL);
        reset_count++;
    }

    /* If what linkedlist is not empty */
    if (whatheadpointer != NULL) {
        /* While it is not end of what linked list, free memory of node whatheadpointer is currently pointed to. */
        userresponse_node* temp = whatheadpointer;
        do {
            printf("\nRemoving Node '%s' '%s\n'", temp->intent, temp->entity);
            temp = temp->next;
            free(whatheadpointer);      /* Free memory allocation of node whatheadpointer is currently pointed to. */
            whatheadpointer = temp;     /* Pointing whatheadpointer to next node. If this node is last in the list, it will point to NULL instead.*/
        } while (temp != NULL);
        reset_count++;
    }

    /* If where linkedlist is not empty */
    if (whereheadpointer != NULL) {
        /* While it is not end of where linked list, free memory of node whereheadpointer is currently pointed to. */
        userresponse_node* temp = whereheadpointer;
        do {
            printf("\nRemoving Node '%s' '%s\n'", temp->intent, temp->entity);
            temp = temp->next;
            free(whereheadpointer);     /* Free memory allocation of node whereheadpointer is currently pointed to. */
            whereheadpointer = temp;    /* Pointing whereheadpointer to next node. If this node is last in the list, it will point to NULL instead.*/
        } while (temp != NULL);
        reset_count++;
    }

    if (reset_count == 0) {
        snprintf(response, n, "Nothing to reset.");     /* If there are no responses saved, then chatbot will not reset.*/
    }
    else {
        snprintf(response, n, "Chatbot reset.");        /* Else, chatbot will reset.*/
    }

    return 0; /* Returning 0 will indicate that the chatbot will continue chatting after being reset.*/
}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char* intent) {

    if (compare_token(intent, "save") == 0) {
        return 1;
    }
    else {
        return 0;
    }

}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * for SAVE, it may be "as" or "to".
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char* inv[], char* response, int n) {


    char filename[256]; // Define a char array to store filename of size 256
    //validation checking. checks if intent is accompanied with "as" or ""to" and filename
    if (inc == 1) {
        snprintf(response, n, "Sorry, I don't understand the word 'save'. Did you meant something like, \"save to FILENAME.ini or save as FILENAME.ini?\"");
    }
    else if (compare_token(inv[1], "to") == 1) {
        snprintf(response, n, "Sorry, this is not the correct format to save a file. Did you meant something like, \"save to FILENAME.ini or save as FILENAME.ini?\"");
    }
    else if (inv[2] == NULL) {
        snprintf(response, n, "Filename not inputted!");
    }
    //checks if pointer is empty
    else if (inc == 3) {
        if (whoheadpointer == NULL && whatheadpointer == NULL && whereheadpointer == NULL) {
            snprintf(response, n, "Save unsuccessful! Knowledge base is empty!");
        }
        else {
            strcpy(filename, inv[2]); //copy the third word into filename
            FILE* f; //decalre a pointer to FILE structure
            f = fopen(filename, "w"); //open the file
            if (f == NULL) {
                snprintf(response, n, "Could not open %s.", filename);
            }
            else {
                knowledge_write(f); //write to file
                snprintf(response, n, "My knowledge has been saved to %s.", filename);
                fclose(f);
            }
        }
    }
    //if user inputs more than 3 words
    else {
        snprintf(response, n, "Save unsuccessful! Please do not have spaces in your filename!");
    }

    return 0;
}



/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char* intent) {

    /* to be implemented */
    int greatings, thanks;
    int i = 0;
    int its;
    int goodbye;
    int bye;

    for (i = 0; i < MAX_REPLIES; i++) {
        greatings = compare_token(intent,
            USER_GREETINGS[i]); // compare_token() is in main.c, it compares two strings and return as strcmp()
        thanks = compare_token(intent,
            USER_THANKS[i]);
        its = compare_token(intent, "it's"); // this is used to check if intent contains "it's"

        goodbye = compare_token(intent, "goodbye"); // this is used to check if intent contains goodbye
        bye = compare_token(intent, "bye"); // this is used to check if intent contains bye

        if (greatings == 0) {
            return 1;
        }
        if (thanks == 0) {
            return 1;
        }
        if (its == 0) {
            return 1;
        }
        if (goodbye == 0) {
            return 1;
        }
        if (bye == 0) {
            return 1;
        }
    }


    return 0;

}


/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char* inv[], char* response, int n) {

    /* to be implemented */
    int i;
    int greatings, thanks;
    int its;
    int goodbye;
    int bye;

    for (i = 0; i < MAX_REPLIES; i++) {
        greatings = compare_token(inv[0], USER_GREETINGS[i]); // compare_token() used to compare what was typed with the outputs
        if (greatings == 0) {
            srand(time(NULL)); //// Initialization for seed for random reply, this ensures that the reply is truely random
            int r = rand(); //Returns a pseudo-random integer between 0 and RAND_MAX.
            int randomReply = r % MAX_REPLIES; //this will generate a random int for a random response to return
            snprintf(response, n, CHATBOT_GREETINGS[randomReply], chatbot_username()); // prints out in cmd line the response from chatbot
        }
        thanks = compare_token(inv[0], USER_THANKS[i]); // compare_token() used to compare what was typed with the outputs
        if (thanks == 0) {
            srand(time(NULL)); //// Initialization for seed for random reply, this ensures that the reply is truely random
            int r = rand(); //Returns a pseudo-random integer between 0 and RAND_MAX.
            int randomReply = r % MAX_REPLIES; //this will generate a random int for a random response to return
            snprintf(response, n, CHATBOT_THANKS[randomReply], chatbot_username()); // prints out in cmd line the response from chatbot
        }
        its = compare_token(inv[0], "it's"); //  compare userinput with it's and store it as int its
        if (its == 0) { //if user input contains it's
            snprintf(response, n, "Indeed it is,%s", chatbot_username()); // prints out in cmd line the response from chatbot
        }
        goodbye = compare_token(inv[0], "goodbye"); //compares userinput with goodbye and store as int goodbye
        if (goodbye == 0) { //if user input contains goodbye
            return chatbot_do_exit(inc, inv, response, n); // prints out in cmd line the response from chatbot and exit the chatbot
        }
        bye = compare_token(inv[0], "bye");
        if (bye == 0) {
            return chatbot_do_exit(inc, inv, response, n); // prints out in cmd line the response from chatbot and exit the chatbot
        }
    }


    return 0;


}

