//Cesar Ruiz & Sean Snyder
//COP 3223H
//Final Project

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MONSTERS_IN_AREA 3

/*void explorePossibilites(){

}*/

void encounterMonster(){
    //2D array of strings storing filenames of all possible monsters
    //player could encounter in the area they are in.
    FILE *monsterFile;
    FILE *monsterStats;
    char possibleMonsters[MAX_MONSTERS_IN_AREA][50];
    int i, testVar;

    monsterFile = fopen("possibleMonstersInForest.txt", "r");
    for(i = 0; i < MAX_MONSTERS_IN_AREA; i++){
        fscanf(monsterFile, "%s", possibleMonsters[i]);
        printf("%s\n", possibleMonsters[i]);
    }
    fclose(monsterFile);

    srand(time(0));

    monsterStats = fopen(possibleMonsters[ (rand() % MAX_MONSTERS_IN_AREA) ], "r");
    fscanf(monsterStats, "%d", &testVar);
    printf("%d", testVar);

    fclose(monsterStats);


}

//Character information struct is used to set the base attributes
//and information of various entities.
//NOTE: all variables for characterInformation may not be used for a
//given variable.
typedef struct{
    char name[30];
    char gender[6];
    char job[10];
    char race[20];
    int level;
    int experience;
    int expReward;
    int goldReward;
    int strength;
    int dexterity;
    int constitution;
    int intelligence;
    int willpower;
} charInformation;

int main(){
    //The protagonist variable is the character the plays controls.
    //Any information directly regarding the playable character
    //should edit only the protagonist variable.
    charInformation protagonist, currentEnemy;
    char possibleMonsters[100];
    strcpy(possibleMonsters, "goblinStats.txt");
    char keypress;

    printf("You find yourself in a thick forest, unsure of where you are. You can't seem to recall anything... besides who you are.\n");
    printf("What is your name?\n");
    scanf("%s", protagonist.name);

    //Repeats the choices until the user selects a valid one, then
    //sets protagonist as needed.
    printf("What is your gender? (Press the button that corresponds with your choice).\n");
    do {
        printf("1. Male    2. Female\n");
        scanf(" %c", &keypress);

        switch(keypress){
            case '1':
                strcpy(protagonist.gender, "male");
                break;
            case '2':
                strcpy(protagonist.gender, "female");
                break;
            default:
                printf("Say again?\n");
        }
    } while( !( (keypress == '1') || (keypress == '2') ) );

    //Since the variable keypress was reused multiple times, do-
    //while was used in order to prevent a leftover keypress value
    //from preventing a while loop from running
    printf("You are a %s named %s. You walk over to a nearby pond and look at your reflection. What do you see?\n", protagonist.gender, protagonist.name);
    do {
        printf("1. Human    2. Elf    3. Ork\n");
        scanf(" %c", &keypress);

        switch(keypress){
            case '1':
                strcpy(protagonist.race, "Human");
                printf("Humans are an all-around race. They tend to favor all play styles, though not as much as more restrictive races.\n");
                break;
            case '2':
                strcpy(protagonist.race, "Elf");
                printf("Eves are a race that excel in magic and feats of dexterity. They tend to favor mobile and magic orientated play styles.\n");
                break;
            case '3':
                strcpy(protagonist.race, "Ork");
                printf("Orks are brutal race. They prefer to use their brawn to solve most issues. They tend to favor melee and defensive play styles.\n");
                break;
            default:
                printf("Say again?\n");
        }
    } while( !( (keypress == '1') || ( (keypress == '2') || (keypress == '3') ) ) );

    printf("You are an %s. Before waking up here you recall that you were a...\n", protagonist.race);
    do {
        printf("1. Paladin    2. Mage   3. Archer    4. Berserker\n");
        scanf(" %c", &keypress);

        switch(keypress){
            case '1':
                strcpy(protagonist.job, "Paladin");
                break;
            case '2':
                strcpy(protagonist.job, "Mage");
                break;
            case '3':
                strcpy(protagonist.job, "Archer");
                break;
            case '4':
                strcpy(protagonist.job, "Berserker");
                break;
            default:
                printf("Say again?\n");
        }
    } while( !( ( (keypress == '1') ||  (keypress == '2') ) || ( (keypress == '3') || (keypress == '4') ) ) );

    do {
        printf("You are in a thick forest. What do you do?\n");
        printf("1. Explore    2. Check Stats\n");
        encounterMonster();
        scanf(" %c", &keypress);

        switch(keypress){
            case '1':

                break;
            case '2':

                break;
            default:
                printf("Say again?\n");
        }
    } while( !( (keypress == '1') || (keypress == '2') ) );

    return 0;
}
