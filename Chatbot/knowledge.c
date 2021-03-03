/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * knowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"

typedef struct userresponse_node {
    char intent[MAX_INTENT];	//char of max intent is 32
    char entity[MAX_ENTITY];	//char of max entity is 64
    char response[MAX_RESPONSE];	// char of response is 256
    struct response_node* next;
} userresponse_node;

//initialise pointers that will point to the head of the intent list
userresponse_node* whoheadpointer, * whatheadpointer, * whereheadpointer;

//initialise pointer that will go through each list
userresponse_node* tempofwho, * tempofwhat, * tempofwhere;

userresponse_node* whoIter, * whatIter, * whereIter;

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */


int knowledge_get(const char* intent, const char* entity, char* response, int n) {

    if (compare_token(intent, "who") == 0) // check if the intent is who
    {
        if (whoheadpointer != NULL)        // check if who linked-list is available.
        {
            tempofwho = whoheadpointer;        //tempofwho is point to the head of whoheadpointer
            do {
                if (compare_token(tempofwho->entity, entity) == 0)    // check the entity
                {
                    strcpy(response, tempofwho->response);    //if its true, the response is copied to the response buffer
                    return KB_OK;    //return if a response was found for the intent and entity
                }
                tempofwho = tempofwho->next;
            } while (tempofwho);

            return KB_NOTFOUND;        // return if no entity matches
        }
        else
            return KB_NOTFOUND;        // return if no respose is found
    }
if (compare_token(intent, "what") == 0) // check if the intent is who
    {
        if (whatheadpointer != NULL)        // check if who linked-list is available.
        {
            tempofwhat = whatheadpointer;        //tempofwho is point to the head of whoheadpointer
            //while (tempofwhat == true)
            do
            {
                if (compare_token(tempofwhat->entity, entity) == 0)    // check the entity
                {
                    strcpy(response, tempofwhat->response);    //if its true, the response is copied to the response buffer
                    return KB_OK;    //return if a response was found for the intent and entity
                }
                tempofwhat = tempofwhat->next;
            } while (tempofwhat);

            return KB_NOTFOUND;        // return if no entity matches
        }
        else
            return KB_NOTFOUND;        // return if no response is found
    }
if (compare_token(intent, "where") == 0) // check if the intent is who
    {
        if (whereheadpointer != NULL)        // check if who linked-list is available.
        {
            tempofwhere = whereheadpointer;        //tempofwho is point to the head of whoheadpointer
            do
            {
                if (compare_token(tempofwhere->entity, entity) == 0)    // check the entity
                {
                    strcpy(response, tempofwhere->response);    //if its true, the response is copied to the response buffer
                    return KB_OK;    //return if a response was found for the intent and entity
                }
                tempofwhere = tempofwhere->next;
            } while (tempofwhere);

            return KB_NOTFOUND;        // return if no entity matches
        }
        else
            return KB_NOTFOUND;        // return if no respose is found
    }
    else
        return KB_INVALID;        // the intent word is not recognise
}

/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
void knowledgeput_whowhatwhere(const char* intent, const char* entity, const char* response, char whowhatwhere[6])
{
    //create node for linked list
    userresponse_node* temphead = malloc(sizeof(userresponse_node));
    if (temphead == NULL) {
        return KB_NOMEM; // if temphead is empty returns no memory / -3

    }

    strcpy(temphead->intent, intent); //set the intent node to intent value
    strcpy(temphead->entity, entity); //set the entity node to entity value
    strcpy(temphead->response, response); //set the response node to response value

    if (whowhatwhere == "who") {
        //which linked list the head is pointing to
        temphead->next = whoheadpointer;
        whoheadpointer = temphead;
    }
    else if (whowhatwhere == "what") {
        //which linked list the head is pointing to
        temphead->next = whatheadpointer;
        whatheadpointer = temphead;
    }
    else if (whowhatwhere == "where") {
        //which linked list the head is pointing to
        temphead->next = whereheadpointer;
        whereheadpointer = temphead;
    }

}
int knowledge_put(const char* intent, const char* entity, const char* response) {



    if (compare_token(intent, "who") == 0) {
        //calling of function to create and insert node
        knowledgeput_whowhatwhere(intent, entity, response, "who");
        return KB_OK;

    }

    else if (compare_token(intent, "what") == 0) {
        //calling of function to create and insert node
        knowledgeput_whowhatwhere(intent, entity, response, "what");
        return KB_OK;
    }

    else if (compare_token(intent, "where") == 0) {
        //calling of function to create and insert node
        knowledgeput_whowhatwhere(intent, entity, response, "where");
        return KB_OK;
    }

    return KB_INVALID; //return invalid if the intend do not match

}

/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE* f)
{
    char intent[MAX_INPUT];
    char entity[MAX_ENTITY];
    char sampleresponse[MAX_RESPONSE];
    char* output;
    const char ch = '=';
    int linescount = 0;
    //int i = 0;
    //int e = 0;
    userresponse_node* str = MAX_INTENT;
    size_t size = MAX_RESPONSE;

    userresponse_node* temphead = malloc(size * sizeof(char));
    if (temphead == NULL)        // check if temphead is not empty
    {
        return 0; // if temphead is empty returns no memory
    }
    while (fgets(temphead, size, f) != NULL)        //read each line
    {
        //if (str[0] == '[') {                        / is first char '[]'? /
        //if (sscanf(str, "[%32[^]]", intent) == 1) {
        if (strstr(temphead, "what"))        // find the first occurence of what in temphead
        {
            strcpy(intent, "what");            // set the intent to what until next intent is found
            //break;                          / if so break loop */
        }
        else if (strstr(temphead, "where"))        // find the first occurence of where in temphead
        {
            strcpy(intent, "where");            // set the intent to where until next intent is found
        }
        else if (strstr(temphead, "who"))        // find the first occurence of who in temphead
        {
            strcpy(intent, "who");                // set the intent to who until next intent is found
        }
if (strchr(temphead, ch))        // search for = in temphead
        {
            output = strtok(temphead, "=");        // split the string with "="
            strcpy(entity, output);                // the word before "=" is the entity, copy the entity to output
            output = strtok(NULL, "=");        // Obtain the Response from line of file

            // calculates the length of the number of characters before the
            //1st occurrence of character present in both the string
            output[strcspn(output, "\n")] = 0;    //remove space aft the chatbot reply and before user ask question
            strcpy(sampleresponse, output);            // copy the output to sampleresponse
            knowledge_put(intent, entity, sampleresponse); // Send to Knowledge_Put to insert into List
            linescount++;        //count the number of pair intent/entity
        }


    }

    fflush(stdout); // Flush any unecessary remaining input
    free(temphead); // Free buffer dynamic memory

    return linescount;
}

/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {

    /* To empty who linked list */
    if (whoheadpointer != NULL) {                       /* If whoheadpointer is not NULL value - indicates that linked list is not empty */
        tempofwho = whoheadpointer;                     /* Point tempofwho to whoheadpointer's current pointed location */

        while (whoheadpointer != NULL) {                /* While whoheadpointer is not NULL */
            whoheadpointer = whoheadpointer->next;      /* Point whoheadpointer to next node */
            free(tempofwho);                            /* Free node currently pointed to by the tempofwho */
            tempofwho = whoheadpointer;                 /* Update the tempofwho to point to next node */
        }
    }

    if (whatheadpointer != NULL) {                      /* If whatheadpointer is not NULL value - indicates that linked list is not empty */
        tempofwhat = whatheadpointer;                   /* Point tempofwhat to whatheadpointer's current pointed location */

        while (whatheadpointer != NULL) {               /* While whatheadpointer is not NULL */
            whatheadpointer = whatheadpointer->next;    /* Point whatheadpointer to next node */
            free(tempofwhat);                           /* Free node currently pointed to by the tempofwhat */
            tempofwhat = whatheadpointer;               /* Update the tempofwhat to point to next node */
        }
    }

    if (whereheadpointer != NULL) {                       /* If whereheadpointer is not NULL value - indicates that linked list is not empty */
        tempofwhere = whereheadpointer;                   /* Point tempofwhere to whereheadpointer's current pointed location */

        while (whereheadpointer != NULL) {                /* While whereheadpointer is not NULL */
            whereheadpointer = whereheadpointer->next;    /* Point whereheadpointer to next node */
            free(tempofwhere);                            /* Free node currently pointed to by the tempofwhere */
            tempofwhere = whereheadpointer;               /* Update the tempofwhere to point to next node */
        }
    }
}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledgewrite_whowhatwhere(FILE* f, userresponse_node* headpointer, char whowhatwhere[6])
{
    //checks is pointer is null
    if (headpointer != NULL) {
        userresponse_node* temp = headpointer; //temp pointer pointing to head
        fprintf(f, "[%s]\n", whowhatwhere); //write to file [entity]
        while (temp != NULL) { // checks is pointer is null
            //write to file entity=response, line break
            fprintf(f, "%s=", temp->entity);
            fprintf(f, "%s\n", temp->response);
            temp = temp->next; //move pointer to next node
        }
    }

    fprintf(f, "%s", "\n"); //line break
}



void knowledge_write(FILE* f) {

    //call knowledgewrite_whowhatwhere
    knowledgewrite_whowhatwhere(f, whoheadpointer, "who");
    knowledgewrite_whowhatwhere(f, whatheadpointer, "what");
    knowledgewrite_whowhatwhere(f, whereheadpointer, "where");

}



