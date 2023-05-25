#include <stdlib.h>
#include <string.h>

#include "serialize.h"

/* Unpack the given packet into the buffer unpacked.  You can assume
 * that packed points to a packet buffer large enough to hold the
 * packet described therein, but you cannot assume that the packet is
 * otherwise valid.  You can assume that unpacked points to a character
 * buffer large enough to store the unpacked packet, if it is a valid
 * packet.
 *
 * Returns the packet type that was unpacked, or -1 if it is invalid.
 */
int unpack(char *unpacked, void *packed) {
    int cool=*(int*)packed;
    if(cool==STATUS){
        packed+=sizeof(int);

        int len1=0;
        for(int i = 0;i < NAME_SIZE; i++){
            char cool1=*(char*)packed;
            if(cool1!='\0'){
            unpacked[len1]=*(char*)packed;
            len1++;
            }
            packed+=sizeof(char);
        }
        unpacked[len1]=' ';
        len1++;

        size_t len = *(size_t*)packed;
        packed+=sizeof(size_t);
        packed+=sizeof(size_t);
        for(size_t i=0;i<len;i++,len1++){
            unpacked[len1]=*(char*)packed;
            packed+=sizeof(char);
        }
        unpacked[len1]='\0';
        return STATUS;
    }
    else if(cool==LABELED){
         packed+=sizeof(int);

        int len1=0;
        for(int i = 0;i < NAME_SIZE; i++){
            char cool1=*(char*)packed;
            if(cool1!='\0'){
            unpacked[len1]=*(char*)packed;
            len1++;
            }
            packed+=sizeof(char);
        }
        unpacked[len1]=':';
        len1++;
        unpacked[len1]=' ';
        len1++;
        unpacked[len1]='@';
        len1++;

        size_t num=*(size_t*)packed;
        packed+=sizeof(size_t);
        size_t num2=*(size_t*)packed;
        packed+=sizeof(size_t);
        packed+=sizeof(size_t);
        for(size_t i=0;i<num;i++){
            packed+=sizeof(char);
        }
        for(size_t i=0;i<num2;i++,len1++){
            unpacked[len1]=*(char*)packed;
            packed+=sizeof(char);
        }
        for(size_t i=0;i<num2;i++){
            packed-=sizeof(char);
        }
        unpacked[len1]=' ';
        len1++;
        for(size_t i=0;i<num;i++){
            packed-=sizeof(char);
        }

        for(size_t i=0;i<num;i++,len1++){
            unpacked[len1]=*(char*)packed;
            packed+=sizeof(char);
        }

        unpacked[len1]='\0';

        return LABELED;
    }
    else if(cool==MESSAGE){
        packed+=sizeof(int);

        int len1=0;
        for(int i = 0;i < NAME_SIZE; i++){
            char cool1=*(char*)packed;
            if(cool1!='\0'){
            unpacked[len1]=*(char*)packed;
            len1++;
            }

            packed+=sizeof(char);
        }
        /*for(int i=len1;16>len1;i++){
            packed+=sizeof(char);
            }*/
        unpacked[len1]=':';
        len1++;
        unpacked[len1]=' ';
        len1++;
        size_t len=*(size_t*)packed;
        packed+=sizeof(size_t);
        packed+=sizeof(size_t);
        // size_t sum = len + (size_t)len1;
        for(size_t i=0;len>i;i++, len1++){
            unpacked[len1]=*(char*)packed;;
            packed+=sizeof(char);
        }
        unpacked[len1]='\0';
        return MESSAGE;
    }
    else{
        return -1;
    }
}

/* Unpack the given packed packet into the given statistics structure.
 * You can assume that packed points to a packet buffer large enough to
 * hold the statistics packet, but you cannot assume that it is
 * otherwise valid.  You can assume that statistics points to a
 * statistics structure.
 *
 * Returns the packet type that was unpacked, or -1 if it is invalid.
 */
int unpack_statistics(struct statistics *statistics, void *packed) {
    if(*(int*)packed==STATISTICS){

    packed+=sizeof(int);
    int len1=0;
    for(int i = 0;i < NAME_SIZE; i++){
         char cool1=*(char*)packed;
         if(cool1!='\0'){
                statistics->sender[i]=*(char*)packed;
                len1++;
        }
        packed+=sizeof(char);
    }
       statistics->sender[len1]='\0';
    // packed+=sizeof(char);
    //  packed+=sizeof(int);

    len1=0;
    for(int i=0;i<NAME_SIZE;i++){
        char cool1=*(char*)packed;
           if(cool1!='\0'){
                statistics->most_active[i]=*(char*)packed;
                len1++;
           }
     packed+=sizeof(char);
    }

    statistics->most_active[len1]='\0';
    // packed+=sizeof(char);

     statistics->most_active_count=*(int*)packed;
     packed+=sizeof(int);
     statistics->invalid_count=*(long*)packed;
     packed+=sizeof(long);
     statistics->refresh_count=*(long*)packed;
     packed+=sizeof(long);


     //  packed-=sizeof(long);
     // packed-=sizeof(long);
     // packed-=sizeof(int);
    statistics->messages_count=*(int*)packed;

     return STATISTICS;
    }
    else{
        return -1;
    }
}
