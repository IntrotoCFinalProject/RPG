//Cesar Ruiz & Sean Snyder
//COP 3223H
//Final Project

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MONSTERS_IN_AREA 5

/*void explorePossibilities(){

}*/

void encounterMonster(){
    //2D array of strings storing filenames of all possible monsters
    //player could encounter in the area they are in.
    FILE *monsterFile;
    FILE *monsterStats;
    char possibleMonsters[MAX_MONSTERS_IN_AREA][50];
    int i, testVar;

    //This opens up a text document that contains the names of the text files of all possible monsters in a given area
    monsterFile = fopen("possibleMonstersInForest.txt", "r");

    //This runs a loop that records the name of each text files inside of the array
    for(i = 0; i < MAX_MONSTERS_IN_AREA; i++){
        fscanf(monsterFile, "%s", possibleMonsters[i]);
    }
    //Now we are done with this particular file so we must close it
    fclose(monsterFile);

    //Now that each array is filled with the names of each monster's text file,
    //we use a randomizer to pick a monster out of the possible files
    monsterStats = fopen(possibleMonsters[ (rand() % MAX_MONSTERS_IN_AREA) ], "r");

    //Here we print out the stats of the monster which is currently used for testing purposes
    fscanf(monsterStats, "%d", &testVar);
    printf("%d", testVar);

    //Again, we are done with the file and finish by closing it
    fclose(monsterStats);
}

void exploreForest(){
    int random;
    char insideKeypress;

    random = (rand() % 100);
    if (random >= 75){
        printf("You found some loot!\n");
    }
    else if (random >= 50){
        printf("You found a hidden path deep into the forest\nWhat will you do?\n");
        do{
            printf("1. Follow the path    2. Turn back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You venture further down the path and find yourself lost in a strange land.\n");
                break;
            case '2':
                printf("You turn around and retrace your steps.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }
    else if (random >= 25){
        encounterMonster();
    }
    else{
        printf("You found nothing and keep moving.\n");
    }

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
    int  random;

    //Here we set up the randomizer for later in the program
    srand(time(0));

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
    } while( !( (keypress >= '1') && (keypress <= '2') ) );

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
    } while( !( (keypress >= '1') && (keypress <= '3')  ) );

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
    } while( !( ( (keypress >= '1') && (keypress <= '4') ) ) );

    do {
        while(1){
            printf("You are in a thick forest. What do you do?\n");
            printf("1. Explore    2. Check Stats\n");

            scanf(" %c", &keypress);

            switch(keypress){
                case '1':
                    exploreForest();
                    break;
                case '2':

                    break;
                default:
                    printf("Say again?\n");
            }
        }
    } while( !( (keypress >= '1') && (keypress <= '2') ) );

    return 0;
}
