

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFLEN 64

struct Traveler {
    char name[20];
    char status[12];
    int HP;
    int mana;
};


void assignCharacteristicsTraveler(struct Traveler* traveler){
    int point = 100;
    if (strncmp(traveler->status,"IMMUNITY",8)==0) {
           point = 10000;
    }
    traveler->HP = point;
    traveler->mana = point;
    strcpy(traveler->status, "READY\0");
}

struct Traveler* newTraveler(char* name){
    printf("You create a new traveler : %s\n\n", name);
    struct Traveler* traveler = malloc(sizeof(struct Traveler));
    strncpy(traveler->name, name, 20);
    assignCharacteristicsTraveler(traveler);
    return traveler;
}


void deleteTraveler(struct Traveler* traveler){
    strcpy(traveler->status,"DEAD\0");
    free(traveler);
}


struct Scientist {
    char name[32];
    int HP;
    int mana;
};


void assignCharacteristicsScientist(struct Scientist* scientist){
    scientist->HP = 10000;
    scientist->mana = 10000;
}


struct Scientist* newScientist(char* name){
    printf("You create a new scientist : %s\n\n", name);
    struct Scientist* scientist = malloc(sizeof(struct Scientist));
    strncpy(scientist->name, name, 32);
    assignCharacteristicsScientist(scientist);
    return scientist;
}


void deleteScientist(struct Scientist* scientist){
    free(scientist);
}


int travelInTime(char* name, int HP, int mana){
    printf("%s passes through the portal, the journey begins ...\n", name);
    if (HP < 2000 || mana < 5000) {
        return 0;
    }
    return 1;
}


void endingCredits(){
        printf("Congratulations, welcome in the past!\n");
        printf("End !\n");
        exit(0);
}


int cleanLine(char* line){
    for(;*line != '\n'; line++);
    *line = 0;
    return 0;
}
     

int main(){
    int end = 0;
    char order = -1;
    char nl = -1;
    char line[BUFLEN] = {0};
    struct Traveler* traveler = NULL;
    struct Scientist* scientist = NULL;

    
    while(!end){
        puts("1: Create a new traveler\n2: Travel in time\n3: Delete your traveler\n4: Create a scientist\n5: Make the scientist travel in time\n6: Remove the scientist\n7: Show traveler characteristics points\n8: Show scientist characteristics points\n0: Quit");
        order = getc(stdin);
        nl = getc(stdin);
        if(nl != '\n'){
            printf("Bad command, program will exit.\n");
            exit(0);
        }
        fseek(stdin,0,SEEK_END);
        switch(order){
        case '1':
            puts("Give a name to your traveler : ");
            fgets(line, BUFLEN, stdin);
            cleanLine(line);
            traveler = newTraveler(line);
            break;
        case '2':
            if(!traveler){
                puts("You do not have a traveler.\n\n");
                break;
            }
            if (travelInTime(traveler->name, traveler->HP, traveler->mana)) {
                endingCredits(); 
            } else {
                printf("Someting went wrong. %s was not strong enough and didn't survive the journey.\n\n", traveler->name);
                deleteTraveler(traveler);
                traveler = NULL;
            }
            break;
        case '3':
            if(!traveler){
                puts("You do not have a traveler.\n\n");
                break;
            }
            printf("You delete your traveler %s. Will you one day travel back in time ?\n\n", traveler->name);
            deleteTraveler(traveler);
            traveler= NULL;
            break;
        case '4':
            puts("Give a name to your scientist : ");
            fgets(line, BUFLEN, stdin);
            cleanLine(line);
            scientist = newScientist(line);
            break;
        case '5':
            if(!scientist){
                puts("You do not have a scientist.\n\n");
                break;
            }
            if (travelInTime(scientist->name, scientist->HP, scientist->mana)) {
                printf("The portal begins to light up. %s disapears maybe in the past or the future ...\n", scientist->name);
                sleep(1);
                scientist->HP -= 2000;
                scientist->mana -= 5000;
                printf("Suddenly, the scientist returns from the past (or future ?). He looks a bit injured and exhausted.\n\n");
            } else {
                printf("Someting went wrong. %s was not strong enough and didn't survive the journey.\n\n", scientist->name);
                deleteScientist(scientist);
                scientist = NULL;
            }
            break;
        case '6':
            if(!scientist){
                puts("You do not have a scientist.\n\n");
                break;
            }
            printf("The scientist %s suddently disapear. Maybe he is gone to another timeline ...\n\n", scientist->name);
            deleteScientist(scientist);
            scientist= NULL;
            break;
        case '7':
            if(!traveler){
                puts("You need to create a traveler to see its characteristics !\n\n");
            } else {
                printf("\nHere are the characteristics of your traveler\n");
                printf("HP: %d\n",traveler->HP);
                printf("Mana: %d\n\n",traveler->mana);
            }
            break;
        case '8':
            if(!scientist){
                puts("You need to create a scientist to see its characteristics !\n\n");
            } else {
                printf("\nHere are the characteristics of your scientist\n");
                printf("HP: %d\n",scientist->HP);
                printf("Mana: %d\n\n",scientist->mana);
            }
            break;
        case '0':
        default:
            end = 1;
        }
    }
    return 0;
}


