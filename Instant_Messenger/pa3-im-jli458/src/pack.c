#include <stdlib.h>
#include <string.h>

#include "serialize.h"

/* Pack the user input provided in input into the appropriate message
 * type in the space provided by packed.  You can assume that input is a
 * NUL-terminated string, and that packed is a buffer of size
 * PACKET_SIZE.
 *
 * Returns the packet type for valid input, or -1 for invalid input.
 */
int pack(void *packed, char *input) {
    //only checks that its valid

    char s[4]="/me ";
    char stat[7]="/stats ";
    int type=-1;

    int sacc=0;
    int stacc=0;
    size_t space=0;
    size_t space1=0;
    size_t len=0;

    if(input[1]==s[1]){
        while(sacc<4){
            if(input[sacc]==s[sacc]){
                sacc+=1;
            }
            else if(input[sacc]!=s[sacc]){
                break;
            }
        }
        if(sacc==4){
            int i;
            for(i=3;input[i]!='\0';i++){
                if(input[i]==' '){
                    space+=1;
                }
                else if(input[i]!=' '){
                    break;
                }
            }
            if(input[i]=='\0'){
                return -1;
            }
            type=STATUS;
        }
    }
    else if(input[0]=='@'){
        for(size_t i=1;input[i]!=' ';i++){
            len+=1;
        }
        if(len>0 && len<=16){
             size_t i;
             for(i=len+1;input[i]==' ';i++){
                 space1+=1;
             }
             if(input[i]=='\0'){
                 return -1;
             }
             else if(input[space1+len]!='\0'){
                 type=LABELED;
             }
        }
        else if(len<=0 || len>16){
            return -1;
        }
    }
    else if(input[1]==stat[1]){
        while(stacc<6){
            if(input[stacc]==stat[stacc]){
                stacc+=1;
            }
            else if(input[stacc]!=stat[stacc]){
                break;
            }
        }
        if(strlen(input)==6){
            type=STATISTICS;
        }else if(strlen(input)!=6){
            return -1;
        }
    }
    else{
        int i=0;
        for(i=0;input[i]!='\0';i++){
            if(input[i]!=' '){
                type=MESSAGE;
                break;
            }
        }
        if(input[i]=='\0'){
            return -1;
        }
    }

    if(type==STATUS){
        *(int*)packed=STATUS;
        packed+=sizeof(int);
        char ubitstr[6] = "jli458";
        for(int i=0; i<=5 ;i++){
            *(char*)packed=ubitstr[i];
            packed+=sizeof(char);
        }
        //len = 7
        for(int i=1; i<=10;i++){
            *(char*)packed='\0';
            packed+=sizeof(char);
        }
        //len = 17
        *(size_t*)packed=strlen(input)-(3+space);
        //how to make strlen the statuslen
        packed+=sizeof(size_t);
        *(size_t*)packed=0;
        packed+=sizeof(size_t);
        for(int i=3+space;input[i]!='\0';i++){
            *(char*)packed=input[i];
            packed+=sizeof(char);
        }
        return STATUS;
    }
    else if(type==MESSAGE){
        *(int*)packed=MESSAGE;
        packed+=sizeof(int);
        char ubitstr[6] = "jli458";
        for(int i=0; i<=5 ;i++){
            *(char*)packed=ubitstr[i];
            packed+=sizeof(char);
        }
        //len = 7
        for(int i=1; i<=10;i++){
            *(char*)packed='\0';
            packed+=sizeof(char);
        }
        //len = 17
        *(size_t*)packed=strlen(input);
        packed+=sizeof(size_t);
        *(size_t*)packed=0;
        packed+=sizeof(size_t);
        for(int i=0;input[i]!='\0';i++){
            *(char*)packed=input[i];
            packed+=sizeof(char);
        }
        return MESSAGE;
    }
     else if(type==LABELED){
        *(int*)packed=LABELED;
        packed+=sizeof(int);

        char ubitstr[6] = "jli458";
        for(int i=0; i<=5 ;i++){
            *(char*)packed=ubitstr[i];
            packed+=sizeof(char);
        }
        //len = 7
        for(int i=1; i<=10;i++){
            *(char*)packed='\0';
            packed+=sizeof(char);
        }

        size_t len=0;
        for(size_t i=1;input[i]!=' ';i++){
            len++;
        }

        size_t space=2;
        for(size_t i=len;input[i]==' ';i++){
            space++;
        }

        size_t mess=0;
        for(size_t i=(len+space);input[i]!='\0';i++){
            mess+=1;
        }


        *(size_t*)packed=mess;
        packed+=sizeof(size_t);
        *(size_t*)packed=len;
        packed+=sizeof(size_t);
        *(size_t*)packed=0;
        packed+=sizeof(size_t);

        mess=mess+'0';
        for(size_t i=(space+len);input[i]!='\0';i++){
            *(char*)packed=input[i];
            packed+=sizeof(char);
        }

        for(size_t l=1; input[l]!=' ';l++){
            *(char*)packed=input[l];
            packed+=sizeof(char);
        }
        return LABELED;
    }
    else if(type==STATISTICS){
        *(int*)packed=STATISTICS;
        packed+=sizeof(int);
        char ubitstr[6] = "jli458";
        for(int i=0; i<=5 ;i++){
            *(char*)packed=ubitstr[i];
            packed+=sizeof(char);
        }
        //len = 7
        for(int i=1; i<=10;i++){
            *(char*)packed='\0';
            packed+=sizeof(char);
        }
        //len = 17
        return STATISTICS;
    }
    return -1;
}





/* Create a refresh packet for the given message ID.  You can assume
 * that packed is a buffer of size PACKET_SIZE.
 *
 * You should start by implementing this method!
 *
 * Returns the packet type.
 */

int pack_refresh(void *packed, int message_id) {

    *(int*)packed=REFRESH;
    packed+=sizeof(int);
    char ubitstr[6] = "jli458";

    for(int i=0; i<=5 ;i++){
        *(char*)packed=ubitstr[i];
        packed+=sizeof(char);
    }
    //len = 7
    for(int i=1; i<=10;i++){
        *(char*)packed='\0';
        packed+=sizeof(char);
    }
    //len=17
    *(int*)packed=message_id;
    packed+=sizeof(int);
    //len = 18
    return REFRESH;
}
