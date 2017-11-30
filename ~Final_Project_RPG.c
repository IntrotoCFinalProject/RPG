//Cesar Ruiz & Sean Snyder
//COP 3223H
//Final Project

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define  MAX_MONSTERS_IN_AREA 5
#define  MAX_NUM_MONSTER_STATS 5
#define  MAX_NUM_PROTAG_STATS_FROM_FILE 5

typedef struct{
    char name[30];          //This will save the user's declared name
    char gender[7];         //This saves their gender
    char race[20];          //This saves their race which affects their base stats
    char job[10];           //Their chosen job further affects their stats
    int  statIndex;         //Offset (based on selected job) for how far along to move in the jobStatsPerLevel.txt for the desired character stat mods
    int  level;             //This will keep track of their level which will increase their stats the higher it is
    int  currentExperience; //Current experience will be used to keep track of their progress to leveling up
    int  neededExperience;  //Needed experience will track the amount of experience needed for level up
    int  gold;              //The gold stat will be used to track the players score based on the enemies they've killed
    int  physicalPower;     //This stat will keep track of the damage done through melee
    int  magicalPower;      //This stat keeps track of magical damage
    int  speed;             //This stat tracks their speed to see who begins the fight
    int  evasionChance;
    int  maxHP;             //This stat tracks the maximum health that the player can have
    int  currentHP;         //This stat checks the current health which will differ from max whenever the player is hurt
    int  maxMana;           //This stat controls the maximum mana the player can have
    int  currentMana;       //This stat tracks the player's current mana which changes after using magic
    int  area;              //The area stat will track which explore function should be used
} charInformation;

//This function will be used to increase the stats of the player each time they gain a level
void protagLevelUp(charInformation *protag);

//This function randomizes which monster a player will encounter, depending on the area
void encounterMonster(charInformation *protag);

//This function will be used whenever the player checks their stats
void checkStats(charInformation stats);

//These functions are called whenever the player chooses the option to explore
void exploreForest(charInformation *protag);
void exploreCave(charInformation *protag);
void exploreTown(charInformation *protag);
void exploreGraveyard(charInformation *protag);
void exploreMountain(charInformation *protag);
void exploreMarsh(charInformation *protag);
void exploreDesert(charInformation *protag);
void exploreVolcano(charInformation *protag);

//This function gets called every time the player encounters a monster and chooses to fight or fails to flee
void battleEncounter(charInformation *protag, int monsterStats[], char monsterName[]);

//These function determine the damage inflicted or received, respectively
void playerAttacksMonster(charInformation *protag, int monsterStats[], char monsterName[]);
void monsterAttacksPlayer(charInformation *protag, int monsterStats[], char monsterName[]);

//Character information struct is used to set the base attributes
//and information of various entities.
//NOTE: all variables for characterInformation may not be used for a given variable.

int main(){
    //The protag variable is the character the plays controls. Any information directly regarding the playable character should edit only the protag variable.
    charInformation protag;
    protag.level = 0; protag.speed = 0; protag.currentExperience = 0; protag.neededExperience = 0; protag.area = 0; protag.physicalPower = 0; protag.magicalPower = 0;
    protag.evasionChance = 10;
    char keypress;

    //Here we set up the randomizer for later in the program
    srand(time(0));

    printf("You find yourself in a thick forest, unsure of where you are. You can't seem to recall anything... besides who you are.\n");
    printf("What is your name?\n");
    scanf("%s", protag.name);

    //Repeats the choices until the user selects a valid one, then
    //sets protag as needed.
    printf("What is your gender? (Press the button that corresponds with your choice).\n");
    do {
        printf("1. Male    2. Female\n");
        scanf(" %c", &keypress);

        switch(keypress){
            case '1':
                strcpy(protag.gender, "male");
                break;
            case '2':
                strcpy(protag.gender, "female");
                break;
            default:
                printf("Say again?\n");
        }
    } while( !( (keypress >= '1') && (keypress <= '2') ) );

    //Since the variable keypress was reused multiple times, do-
    //while was used in order to prevent a leftover keypress value
    //from preventing a while loop from running
    printf("You are a %s named %s. You walk over to a nearby pond and look at your reflection. What do you see?\n", protag.gender, protag.name);
    do {
        printf("1. Human    2. Elf    3. Ork\n");
        scanf(" %c", &keypress);//The max length for the string is 80 characters plus a space for the /0

        //Player race and starting hp and mana are set
        switch(keypress){
            case '1':
                strcpy(protag.race, "human");
                printf("Humans are an all-around race. They tend to favor all play styles, though not as much as more restrictive races.\n");
                protag.maxHP = 5;
                protag.maxMana = 4;
                break;
            case '2':
                strcpy(protag.race, "elf");
                printf("Eves are a race that excel in magic and speed. They tend to favor mobile and magic orientated play styles.\n");
                protag.maxHP = 4;
                protag.maxMana = 5;
                break;
            case '3':
                strcpy(protag.race, "ork");
                printf("Orks are brutal race. They prefer to use their brawn to solve most issues. They tend to favor melee and defensive play styles.\n");
                protag.maxHP = 7;
                protag.maxMana = 2;
                break;
            default:
                printf("Say again?\n");
        }
    } while( !( (keypress >= '1') && (keypress <= '3')  ) );

    //Player selects their job/class
    printf("You are %s. Before waking up here you recall that you were a...\n", protag.race);
    do {
        printf("1. Paladin    2. Berserker   3. Mage    4. Cleric\n");
        scanf(" %c", &keypress);

        switch(keypress){
            case '1':
                strcpy(protag.job, "paladin");
                protag.statIndex = 1;
                break;
            case '2':
                strcpy(protag.job, "berserker");
                protag.statIndex = 2;
                break;
            case '3':
                strcpy(protag.job, "mage");
                protag.statIndex = 3;
                break;
            case '4':
                strcpy(protag.job, "cleric");
                protag.statIndex = 4;
                break;
            default:
                printf("Say again?\n");
        }
    } while( !( ( (keypress >= '1') && (keypress <= '4') ) ) );

    //Once Player has selected their class, they level up from 0 to 1, setting their initial stats.
    protagLevelUp(&protag);

    //Main gameplay loop, where player either encounters monsters to fight or explores the world.
    do {
        while(protag.area == 0){
            printf("\nYou are in a thick forest. What do you do?\n");
            printf("1. Explore    2. Check Stats\n");

            scanf(" %c", &keypress);

            switch(keypress){
                case '1':
                    if((rand() % 100) >= 70){
                        encounterMonster(&protag);
                    }
                    else{
                        exploreForest(&protag);
                    }
                    break;
                case '2':
                    checkStats(protag);
                    break;
                default:
                    printf("Say again?\n");
            }
        }
    } while( !( (keypress >= '1') && (keypress <= '2') ) );

    do{
        while(protag.area == 1){
            printf("\nYou are surrounded by an endless cave system. What do you do?\n");
            printf("1. Explore    2. Check Stats\n");

            scanf(" %c", &keypress);

            switch(keypress){
                case '1':
                    if((rand() % 100) >= 70){
                        encounterMonster(&protag);
                    }
                    else{
                        exploreCave(&protag);
                    }
                    break;
                case '2':
                    checkStats(protag);
                    break;
                default:
                    printf("Say again?\n");
            }
        }
    } while( !( (keypress >= '1') && (keypress <= '2') ) );

    do{
        while(protag.area == 2){
            printf("\nYou find yourself in a deserted village. What do you do?\n");
            printf("1. Explore    2. Check Stats\n");

            scanf(" %c", &keypress);

            switch(keypress){
                case '1':
                    if((rand() % 100) >= 70){
                        encounterMonster(&protag);
                    }
                    else{
                        exploreTown(&protag);
                    }
                    break;
                case '2':
                    checkStats(protag);
                    break;
                default:
                    printf("Say again?\n");
            }
        }
    } while( !( (keypress >= '1') && (keypress <= '2') ) );

    do{
        while(protag.area == 3){
            printf("\nYou are lost in the billowing mist of the graveyard. What do you do?\n");
            printf("1. Explore    2. Check Stats\n");

            scanf(" %c", &keypress);

            switch(keypress){
                case '1':
                    if((rand() % 100) >= 70){
                        encounterMonster(&protag);
                    }
                    else{
                        exploreGraveyard(&protag);
                    }
                    break;
                case '2':
                    checkStats(protag);
                    break;
                default:
                    printf("Say again?\n");
            }
        }
    } while( !( (keypress >= '1') && (keypress <= '2') ) );

    do{
        while(protag.area == 4){
            printf("\nYou look around to find you are in a sweeping mountain range. What do you do?\n");
            printf("1. Explore    2. Check Stats\n");

            scanf(" %c", &keypress);

            switch(keypress){
                case '1':
                    if((rand() % 100) >= 70){
                        encounterMonster(&protag);
                    }
                    else{
                        exploreMountain(&protag);
                    }
                    break;
                case '2':
                    checkStats(protag);
                    break;
                default:
                    printf("Say again?\n");
            }
        }
    } while( !( (keypress >= '1') && (keypress <= '2') ) );

    do{
        while(protag.area == 5){
            printf("\nYou search your surroundings for a way out of the marsh. What do you do?\n");
            printf("1. Explore    2. Check Stats\n");

            scanf(" %c", &keypress);

            switch(keypress){
                case '1':
                    if((rand() % 100) >= 70){
                        encounterMonster(&protag);
                    }
                    else{
                        exploreMarsh(&protag);
                    }
                    break;
                case '2':
                    checkStats(protag);
                    break;
                default:
                    printf("Say again?\n");
            }
        }
    } while( !( (keypress >= '1') && (keypress <= '2') ) );

    do{
        while(protag.area == 6){
            printf("\nThe blazing sun beats down on you as you move. What do you do?\n");
            printf("1. Explore    2. Check Stats\n");

            scanf(" %c", &keypress);

            switch(keypress){
                case '1':
                    if((rand() % 100) >= 70){
                        encounterMonster(&protag);
                    }
                    else{
                        exploreDesert(&protag);
                    }
                    break;
                case '2':
                    checkStats(protag);
                    break;
                default:
                    printf("Say again?\n");
            }
        }
    } while( !( (keypress >= '1') && (keypress <= '2') ) );

    do{
        while(protag.area == 7){
            printf("\nYou have arrived at the foot of the volcano, preparing for the final battle. What do you do?\n");
            printf("1. Explore    2. Check Stats\n");

            scanf(" %c", &keypress);

            switch(keypress){
                case '1':
                    if((rand() % 100) >= 70){
                        encounterMonster(&protag);
                    }
                    else{
                        exploreVolcano(&protag);
                    }
                    break;
                case '2':
                    checkStats(protag);
                    break;
                default:
                    printf("Say again?\n");
            }
        }
    } while( !( (keypress >= '1') && (keypress <= '2') ) );

    return 0;
}

void checkStats(charInformation stats){
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("You are a %s %s named %s, and you are a %s.\n", stats.gender, stats.race, stats.name, stats.job);
    printf("Your current health is: %3d/%d\n", stats.currentHP, stats.maxHP);
    printf("Your current mana is:   %3d/%d\n", stats.currentMana, stats.maxMana);
    printf("Your physical power is:%3d\n", stats.physicalPower);
    printf("Your magical power is: %3d\n", stats.magicalPower);
    printf("You can move at a speed of %d.\n", stats.speed);
    printf("You are currently level %d, %d out of %d experience from leveling up.\n", stats.level, stats.currentExperience, stats.neededExperience);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void protagLevelUp(charInformation *protag){
    int   mod[MAX_NUM_PROTAG_STATS_FROM_FILE];
    int   i;
    FILE *statsFile;

    //Opens text file depending on player's class
    if( strcmp(protag->job, "paladin") == 0 )
    {
        statsFile = fopen("ZpaladinStatsPerLevel.txt", "r");
    }
    else if( strcmp(protag->job, "berserker") == 0 )
    {
        statsFile = fopen("ZberserkerStatsPerLevel.txt", "r");
    }
    else if( strcmp(protag->job, "mage") == 0)
    {
        statsFile = fopen("ZmageStatsPerLevel.txt", "r");
    }
    else if( strcmp(protag->job, "cleric") == 0 )
    {
        statsFile = fopen("ZclericStatsPerLevel.txt", "r");
    }

    //prints out a message alerting the player they have leveled up, only if they aren't leveling up from level 0 to 1
    if( protag->level != 0 ){
        printf("Your level has increased from %d to %d!\n", protag->level, (protag->level + 1) );
    }

    //increments the player's level, subtracting the experience needed to level up from their current amount
    //then increases the amount of exp needed for the next level up
    protag->level++;
    protag->currentExperience -= protag->neededExperience;
    protag->neededExperience = protag->level * 50;

    for(i = 0; i < MAX_NUM_PROTAG_STATS_FROM_FILE; i++){
        fscanf(statsFile, "%d", &mod[i]);
    }

    //increment stats by desired amount
    protag->physicalPower = mod[0] + protag->physicalPower;
    protag->magicalPower  = mod[1] + protag->magicalPower;
    protag->speed         = mod[2] + protag->speed;
    protag->maxHP         = mod[3] + protag->maxHP;
    protag->maxMana       = mod[4] + protag->maxMana;

    //Heals the protag up to their max resource amounts
    protag->currentHP   = protag->maxHP;
    protag->currentMana = protag->maxMana;

    fclose(statsFile);
}

void exploreForest(charInformation *protag){
    int random;
    char insideKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 66){
        printf("You found some loot!\n");
    }
    //If the number is between 33 and 65, the player advances
    else if (random >= 33){
        printf("You found a hidden path deep into the forest.\nWhat will you do?\n");
        do{
            printf("1. Follow The Path    2. Turn Back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You venture further down the path and find yourself lost in a branching cave system.\n");
                protag->area = 1;
                break;
            case '2':
                printf("You turn around and retrace your steps.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }
    else if(random >= 10){
        printf("You see a village off in the distance.\nWhat will you do?\n");
        do{
            printf("1. Head Towards The Village    2. Turn Back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You head towards the village only to find it deserted.\n");
                protag->area = 2;
                break;
            case '2':
                printf("You continue to wander around the forest.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    //If the number is between 0 and 32, nothing happens
    else{
        printf("You found nothing and keep moving.\n");
    }

}

void exploreCave(charInformation *protag){
    int random;
    char insideKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 66){
        printf("You found some loot!\n");
    }
    //If the number is between 33 and 65, the player advances
    else if (random >= 33){
        printf("You find a caved in mineshaft.\nWhat will you do?\n");
        do{
            printf("1. Move The Rubble    2. Turn Back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You follow the mineshaft to the surface and are surrounded by mountains on every side.\n");
                protag->area = 4;
                break;
            case '2':
                printf("You turn around continue to explore the cave system.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if(random >= 10){
        printf("You find a way out of the caves but cannot see past some mist.\nWhat will you do?\n");
        do{
            printf("1. Dash Into The Mist    2. Turn Back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You run haphazardly into the mist.\n");
                protag->area = 3;
                break;
            case '2':
                printf("You cautiously make your way back into the caves.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }
    //If the number is between 0 and 32, nothing happens
    else{
        printf("You found nothing and keep moving.\n");
    }

}

void exploreTown(charInformation *protag){
    int random;
    char insideKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 66){
        printf("You found some loot!\n");
    }
    //If the number is between 33 and 65, the player advances
    else if (random >= 33){
        printf("You find a path that leads into the mountain range.\nWhat will you do?\n");
        do{
            printf("1. Approach The Mountains    2. Turn Back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You follow the path into the valley.\n");
                protag->area = 4;
                break;
            case '2':
                printf("You turn around and return to the village.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if(random >= 10){
        printf("You see mist billowing in the distance.\nWhat will you do?\n");
        do{
            printf("1. Approach The Mist    2. Turn Back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You enter the mist near the edge of the town.\n");
                protag->area = 3;
                break;
            case '2':
                printf("You continue to search the village.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }
    //If the number is between 0 and 32, nothing happens
    else{
        printf("You found nothing and keep moving.\n");
    }

}

void exploreGraveyard(charInformation *protag){
    int random;
    char insideKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 66){
        printf("You found some loot!\n");
    }
    //If the number is between 33 and 65, the player advances
    else if (random >= 33){
        printf("You spot a river leading out of the mist.\nWhat will you do?\n");
        do{
            printf("1. Follow The River    2. Turn Back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You follow the river, lose your footing, and get swept away by the current.\n");
                protag->area = 5;
                break;
            case '2':
                printf("You walk away from the river and lose yourself in the mist.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if(random >= 10){
        printf("As you move forward, you find the mist begins to slowly clear as the air dries.\nWhat will you do?\n");
        do{
            printf("1. Keep Moving Forward    2. Turn Back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You keep moving until sand is the only thing you can see for miles.\n");
                protag->area = 6;
                break;
            case '2':
                printf("You turn around and return to the endless mist.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }
    //If the number is between 0 and 32, nothing happens
    else{
        printf("You found nothing and keep moving.\n");
    }

}

void exploreMountain(charInformation *protag){
    int random;
    char insideKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 66){
        printf("You found some loot!\n");
    }
    //If the number is between 33 and 65, the player advances
    else if (random >= 33){
        printf("You find a river that leads out of the valley.\nWhat will you do?\n");
        do{
            printf("1. Follow The River    2. Turn Back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You follow the river until you are surrounded by marsh.\n");
                protag->area = 5;
                break;
            case '2':
                printf("You walk past the river and explore the mountains.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if(random >= 10){
        printf("You slide down the mountain and find yourself at the foot of a desert.\nWhat will you do?\n");
        do{
            printf("1. Venture Into The Desert    2. Turn Back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You move forward towards the desert, hoping to find an oasis within.\n");
                protag->area = 6;
                break;
            case '2':
                printf("You climb back up the mountain.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }
    //If the number is between 0 and 32, nothing happens
    else{
        printf("You found nothing and keep moving.\n");
    }

}

void exploreMarsh(charInformation *protag){
    int random;
    char insideKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 66){
        printf("You found some loot!\n");
    }
    //If the number is between 33 and 65, the player advances
    else if (random >= 33){
        printf("As you travel the marsh, you find scorched earth beneath your feet. You look forward and see a looming volcano.\nYou sense this is what you have been preparing for. What will you do?\n");
        do{
            printf("1. Approach The Volcano    2. Turn Back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You walk towards the volcano, careful to not burn yourself from the spewing craters.\n");
                protag->area = 7;
                break;
            case '2':
                printf("You are not ready to discover your fate and return to the marsh.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }
    //If the number is between 0 and 32, nothing happens
    else{
        printf("You found nothing and keep moving.\n");
    }

}

void exploreDesert(charInformation *protag){
    int random;
    char insideKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 66){
        printf("You found some loot!\n");
    }
    //If the number is between 33 and 65, the player advances
    else if (random >= 33){
        printf("As you explore the desert, you find it to be growing hotter. You notice a volcano spewing lava nearby.\nWill you approach the volcano and prepare for the end?\n");
        do{
            printf("1. Approach The Volcano    2. Turn Back\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You walk towards the volcano, careful to not burn yourself from the spewing craters.\n");
                protag->area = 7;
                break;
            case '2':
                printf("You are not ready to discover your fate and return to the desert.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }
    //If the number is between 0 and 32, nothing happens
    else{
        printf("You found nothing and keep moving.\n");
    }

}

void exploreVolcano(charInformation *protag){
    int random;
    char insideKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 66){
        printf("You found some loot!\n");
    }
    //If the number is between 33 and 65, the player advances
    else if (random >= 33){
        printf("This is the end.\n");
    }
    //If the number is between 0 and 32, nothing happens
    else{
        printf("You found nothing and keep moving.\n");
    }

}

void encounterMonster(charInformation *protag){
    //2D array of strings storing filenames of all possible monsters
    //player could encounter in the area they are in.
    FILE *monsterFile;
    FILE *monsterInfo;
    char possibleMonsters[MAX_MONSTERS_IN_AREA][50];
    //monsterStats info at each index-
    //0: HP Current; 1: Attack Power; 2: Speed; 3: EXP Reward ; 4: Gold Reward
    int monsterStats[MAX_NUM_MONSTER_STATS], i;
    char monsterName[30];
    char insideKeypress;

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
    monsterInfo = fopen(possibleMonsters[ (rand() % MAX_MONSTERS_IN_AREA) ], "r");

    //Here we access the monster's name from the appropriate file
    fscanf(monsterInfo, "%s", monsterName);
    printf("\nYou have encountered a %s!\n", monsterName);

    //Here we we retrieve the Monster's stats
    for(i = 0; i < MAX_NUM_MONSTER_STATS; i++){
        fscanf(monsterInfo, "%d", &monsterStats[i]);
    }

    //When the player first encounters the monster, they have the chance to flee
    printf("Will you fight or try to run away?\n");

    do{
        printf("1. Fight    2. Run\n");
        scanf(" %c", &insideKeypress);

        //User decides whether to engage the monster or to flee. Fleeing only succeeds if protag is faster
        switch(insideKeypress){
            case '1':
                printf("You move in to attack.\n\n");
                battleEncounter(protag, monsterStats, monsterName);
                break;
            case '2':
                if(protag->speed >= monsterStats[2])
                {
                    printf("You got away!\n");
                    break;
                }
                else
                {
                    printf("You can't get away!\n");
                    battleEncounter(protag, monsterStats, monsterName);
                }
                break;
            default:
                printf("Say again?\n");
            }
    }while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );


    //Again, we are done with the file and finish by closing it
    fclose(monsterInfo);
}

void battleEncounter(charInformation *protag, int monsterStats[], char monsterName[]){
    char insideKeypress;
    int  damageBuff = 0, overHP, healedHP, randMod = 0;

    do{
        printf("You are fighting a %s, which has %d HP.\n", monsterName, monsterStats[0]);
        printf("Your current HP is:  %3d/%d\n", protag->currentHP, protag->maxHP);
        printf("Your current Mana is:%3d/%d\n", protag->currentMana, protag->maxMana);

        //Basic actions, available to all jobs
        do{
            printf("You approach the monster, and as a %s choose to:\n\n", protag->job);

            printf("1. Attack    2. Dodge    3. Run    ");

            //Any further actions are locked to specific jobs
            if( strcmp(protag->job, "paladin") == 0 )
                printf("4. Moderate Heal\n");
            else if( strcmp(protag->job, "berserker") == 0 )
                printf("4. Damage Buff\n");
            else if( strcmp(protag->job, "mage") == 0 )
                printf("4. Minor Heal\n");
            else if( strcmp(protag->job, "cleric") == 0 )
                printf("4. Major Heal\n");

            scanf(" %c", &insideKeypress);
            printf("\n");

            switch(insideKeypress){
                case('1'):
                    printf("You attempt to attack the %s!\n\n", monsterName);

                    //Whoever is faster attacks first (i.e. potentially deals damage first)
                    if( protag->speed >= monsterStats[2] ){
                        playerAttacksMonster(protag, monsterStats, monsterName);
                        monsterAttacksPlayer(protag, monsterStats, monsterName);
                        printf("\n");
                    }
                    else{
                        monsterAttacksPlayer(protag, monsterStats, monsterName);
                        playerAttacksMonster(protag, monsterStats, monsterName);
                        printf("\n");
                    }
                    break;
                case('2'):
                    printf("You attempt to dodge against the monster's attack!\n");

                    //Player attempts to defend against the enemy, reducing the probability to be hit
                    protag->evasionChance = 40;
                    monsterAttacksPlayer(protag, monsterStats, monsterName);
                    protag->evasionChance = 10;
                    break;
                case('3'):
                    //50 percent chance to flee the battle, if it fails the monster still atacks
                    if( (rand() % 100) <= 50 ){
                        printf("You got away!\n");
                        return;
                    }
                    else{
                        printf("You can't get away!\n\n");
                        monsterAttacksPlayer(protag, monsterStats, monsterName);
                    }
                    break;
                case('4'):
                    //50-50 chance to to either heal/damage a little more or a little less than average
                    if( protag->level < 1){
                        if( (rand() % 100) >= 50 ){
                            randMod = -1;
                        }
                        else{
                            randMod = 1;
                        }
                    }

                    if( protag->level >= 4){
                        if( (rand() % 100) >= 75 ){
                            randMod = -2;
                        }
                        else if( (rand() % 100) >= 50 ){
                            randMod = -2;
                        }
                        else if( (rand() % 100) >= 25 ){
                            randMod = -2;
                        }
                        else{
                            randMod = 2;
                        }
                    }

                    //If the player is a paladin, they have access to a heal that refills half of their max health
                    if( strcmp(protag->job, "paladin") == 0 ){
                        //healedHP tracks the amount of health healed to inform the player
                        healedHP = (protag->maxHP * .5) + randMod;
                        protag->currentHP += (protag->maxHP * .5) + randMod;

                        //If the heal goes over their max, their currentHP is set to maxHP and that difference is subtracted from healedHP
                        if(protag->currentHP > protag->maxHP){
                            overHP = protag->currentHP - protag->maxHP;
                            protag->currentHP = protag->maxHP;
                            healedHP -= overHP;
                        }
                        //Here the player learns how much health they gained
                        printf("You healed %d HP!\n\n", healedHP);
                    }
                    else if( strcmp(protag->job, "berserker") == 0 ){
                        //The berserker cannot heal, but can gain a temporary damage buff that lasts until the end of the fight
                        damageBuff = protag->physicalPower + randMod;
                        protag->physicalPower = (protag->physicalPower * 1.5) + .5 + randMod;
                        damageBuff -= protag->physicalPower;

                        printf("You now do %d damage with a %d damage buff!\n\n", protag->physicalPower, abs(damageBuff));
                    }
                    else if( strcmp(protag->job, "mage") == 0 ){
                        //This process is the same as the paladin's heal, just slightly weaker
                        healedHP = protag->maxHP * .33;
                        protag->currentHP += (protag->maxHP * .33);

                        if(protag->currentHP > protag->maxHP){
                            overHP = protag->currentHP - protag->maxHP;
                            protag->currentHP = protag->maxHP;
                            healedHP -= overHP;
                        }
                        printf("You healed %d HP!\n\n", healedHP);
                    }
                    else if( strcmp(protag->job, "cleric") == 0 ){
                        //This process is the same as the paladin's heal, just slightly stronger
                        healedHP = protag->maxHP * .66;
                        protag->currentHP += (protag->maxHP * .66);

                        if(protag->currentHP > protag->maxHP){
                            overHP = protag->currentHP - protag->maxHP;
                            protag->currentHP = protag->maxHP;
                            healedHP -= overHP;
                        }
                        printf("You healed %d HP!\n\n", healedHP);
                    }
                    monsterAttacksPlayer(protag, monsterStats, monsterName);
                    break;
                default: printf("Say again?\n");;
            }
        } while( !( (insideKeypress >= '1') && (insideKeypress <= '4') ) );
    } while( (protag->currentHP > 0) && (monsterStats[0] > 0) );

    /*while(monster == ALIVE && player == ALIVE)
    {
        printf("You approach the monster, andprotagLevelUp(protag); as a %s choose to:\n\n", protag->job);
        if( strcmp(protag->job, "paladin") == 0 )
        {
            printf("1. Swing Your Longsword    2. Shield    3. Magical Spark    4. Moderate Heal\n");
        }
        else if( strcmp(protag->job, "berserker") == 0 )
        {
            printf("1. Swing Your Axes    2. Block    3. Conjure Throwing Knives    4. Damage Buff\n");
        }
        else if( strcmp(protag->job, "mage") == 0 )
        {
            printf("1. Swing Your Dagger    2. Summon A Shield    3. Summon Elemental Shards    4. Minor Heal\n");
        }
        else if( strcmp(protag->job, "cleric") == 0 )
        {
            printf("1. Swing Your Staff   2. Conjure A Barrier    3. Fire Magic Missiles    4. Major Heal\n");
        }
    }*/

    if( monsterStats[0] <= 0 )
    {
        printf("You defeated the %s!\n", monsterName);

        //Here the temporary buff is removed from the player, if they are not a berserker, 0 is added
        protag->physicalPower += damageBuff;

        //Rewards player with appropriate amount of EXP and gold, then checks if there was a level up
        printf("The %s had %d gold on it, which you quickly pocket. You gain %d experience from the battle!\n", monsterName, monsterStats[4], monsterStats[3]);
        protag->currentExperience += monsterStats[3];
        protag->gold += monsterStats[4];

        if( protag->currentExperience >= protag->neededExperience ){
            protagLevelUp(protag);
        }
    }
    else if( protag->currentHP <= 0 )
    {
        printf("You have been slain in battle.\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GAME OVER~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

        exit(0);
    }
}

void playerAttacksMonster(charInformation *protag, int monsterStats[], char monsterName[]){
    int randDamageMod = 0, damageDealt;

    //checks if player is alive. If not, returns early and no damage is calculated
    if( protag->currentHP <= 0 ){
        return;
    }

    //players have a flat 90 percent chance to hit monsters
    if( (rand() % 100) >= 10){
        //50-50 chance to to either inflict a little more or little less damage than average
        if( protag->level < 4){
            if( (rand() % 100) >= 50 ){
                randDamageMod = -1;
            }
            else{
                randDamageMod = 1;
            }
        }

        //If the player reaches a high enough level, damage dealt has a slightly larger range
        if( protag->level >= 4){
            if( (rand() % 100) >= 75 ){
                randDamageMod = -2;
            }
            else if( (rand() % 100) >= 50 ){
                randDamageMod = -2;
            }
            else if( (rand() % 100) >= 25 ){
                randDamageMod = -2;
            }
            else{
                randDamageMod = 2;
                }
            }


        //uses physical power or magical power depending on the protag's job
        if( (strcmp(protag->job, "paladin") == 0) || (strcmp(protag->job, "berserker") == 0) ){
            damageDealt = protag->physicalPower + randDamageMod * ( ( protag->physicalPower * (rand() % 20) ) / 100 ) ;
            monsterStats[0] -= damageDealt;
            printf("You hurt the %s for %d damage!\n", monsterName, damageDealt);
        }
        else if( (strcmp(protag->job, "cleric") == 0) || (strcmp(protag->job, "mage") == 0) ){
            damageDealt = protag->magicalPower + randDamageMod * ( ( protag->magicalPower * (rand() % 20) ) / 100 ) ;
            monsterStats[0] -= damageDealt;
            printf("You hurt the %s for %d damage!\n", monsterName, damageDealt);
        }
    }
    else{
        printf("You missed the %s!\n", monsterName);
    }
}

void monsterAttacksPlayer(charInformation *protag, int monsterStats[], char monsterName[]){
    int randDamageMod = 0, damageDealt;

    //checks if monster is alive. If not, returns early and no damage is calculated
    if( monsterStats[0] <= 0 ){
        return;
    }

    //evasionChance is the the probability for a monster to miss an attack
    if( (rand() % 100) >= protag->evasionChance ){
        //50-50 chance to to either inflict a little more or little less damage than average
        if( protag->level < 4)
            if( (rand() % 100) >= 50 ){
                randDamageMod = -1;
            }
            else{
                randDamageMod = 1;
            }

        //If the player reaches a high enough level, damage dealt has a slightly larger range
        if( protag->level >= 4){
            if( (rand() % 100) >= 75 ){
                randDamageMod = -2;
            }
            else if( (rand() % 100) >= 50 ){
                randDamageMod = -2;
            }
            else if( (rand() % 100) >= 25 ){
                randDamageMod = -2;
            }
            else{
                randDamageMod = 2;
                }
            }

        damageDealt = monsterStats[1] + randDamageMod * ( ( monsterStats[1] * (rand() % 20) ) / 100 ) ;
        protag->currentHP -= damageDealt;
        printf("The %s hurt you for %d damage!\n", monsterName, damageDealt);
    }
    else{
        printf("The %s missed you!\n", monsterName);
    }
}
