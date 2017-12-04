//Cesar Ruiz & Sean Snyder, COP 3223H, Section 202
//Final Project, December 4, 2017

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define  MAX_MONSTERS_IN_AREA 5
#define  MAX_NUM_MONSTER_STATS 5
#define  MAX_NUM_PROTAG_STATS_FROM_FILE 5

#define  USED_MANA_FOR_HEAL 3

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
    int  evasionChance;     //This stat controls the chance that the monster has to miss their attack
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

void isAlive(charInformation *protag);

//Character information struct is used to set the base attributes
//and information of various entities.
//NOTE: all variables for characterInformation may not be used for a given variable.

int main(){
    //The protag variable is the character the plays controls. Any information directly regarding the playable character should edit only the protag variable.
    charInformation protag;
    protag.level = 0; protag.currentExperience = 0; protag.neededExperience = 0; protag.area = 0; protag.physicalPower = 0; protag.magicalPower = 0;
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
                protag.maxHP = 7;
                protag.maxMana = 7;
                protag.speed = 2;
                break;
            case '2':
                strcpy(protag.race, "elf");
                printf("Eves are a race that excel in magic and speed. They tend to favor mobile and magic orientated play styles.\n");
                protag.maxHP = 5;
                protag.maxMana = 8;
                protag.speed = 3;
                break;
            case '3':
                strcpy(protag.race, "ork");
                printf("Orks are brutal race. They prefer to use their brawn to solve most issues. They tend to favor melee and defensive play styles.\n");
                protag.maxHP = 12;
                protag.maxMana = 5;
                protag.speed = 1;
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
                    if( protag.currentMana < protag.maxMana)
                        protag.currentMana++;

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
                    if( protag.currentMana < protag.maxMana)
                        protag.currentMana++;

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
                    if( protag.currentMana < protag.maxMana)
                        protag.currentMana++;

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
                    if( protag.currentMana < protag.maxMana)
                        protag.currentMana++;

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
                    if( protag.currentMana < protag.maxMana)
                        protag.currentMana++;

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
                    if( protag.currentMana < protag.maxMana)
                        protag.currentMana++;

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
                    if( protag.currentMana < protag.maxMana)
                        protag.currentMana++;

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
                    if( protag.currentMana < protag.maxMana)
                        protag.currentMana++;

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
    printf("Your current stats are:\n");
    printf(" %14d/%2d HP\n", stats.currentHP, stats.maxHP);
    printf(" %14d/%2d Mana\n", stats.currentMana, stats.maxMana);
    printf(" %17d Physical Power\n", stats.physicalPower);
    printf(" %17d Magical Power\n", stats.magicalPower);
    printf(" %17d Speed\n", stats.speed);
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
    int random, damageTaken, mushroom = 0;
    char insideKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 80){
        printf("You found some loot!\n");
    }

    else if (random >= 65){
        printf("While you are walking through the forest, you notice an increasing amount of leaves and branches shaking with every step you take.\n");
        printf("You stop for a moment... and are ambushed by monsters!\n");

        if(protag->level < 4){
            encounterMonster(protag);
            encounterMonster(protag);
        }
        else{
            encounterMonster(protag);
            encounterMonster(protag);
            encounterMonster(protag);
        }
    }


    else if (random >= 45){
        printf("There is a fallen log blocking your path forcing you to backtrack and to find a new path.\n");
    }


    else if (random >= 35){
        printf("Whilst walking through the forest you notice the tripwire just as you are about to trip it. How will you react?\n");
        do{
            printf("1. Try To Dodge    2. Block Some Damage\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                if(protag->speed > 8){
                        if((rand() % 100) >= 40)
                            printf("You get away from the trap in time and emerge unscathed!\n");
                        else{
                            damageTaken = (protag->maxHP * .2);
                            protag->currentHP -= damageTaken;
                            printf("You manage to mostly dodge the trap only losing %d HP.\n", damageTaken);
                            isAlive(protag);
                        }
                }
                else if(protag->speed >= 5){
                        if((rand() % 100) >= 70)
                            printf("You get away from the trap in time and emerge unscathed!\n");
                        else{
                            damageTaken = (protag->maxHP * .2);
                            protag->currentHP -= damageTaken;
                            printf("You manage to mostly dodge the trap only losing %d HP.\n", damageTaken);
                            isAlive(protag);
                        }
                }
                else{
                    damageTaken = (protag->maxHP * .5);
                    protag->currentHP -= damageTaken;
                    printf("You fail to get away from the trap in time and lost %d HP.\n", damageTaken);
                    isAlive(protag);
                }
                break;
            case '2':
                damageTaken = (protag->maxHP * .3);
                protag->currentHP -= damageTaken;
                printf("You protect against most of the brunt damage from the trap and only lose %d HP.\n", damageTaken);
                isAlive(protag);
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 15){
        printf("You come across a magical spring. What will you do?\n");
        do{
            printf("1. Drink    2. Leave\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                if((rand() % 100) >= 25){
                    printf("You drink from the spring, and it heals you to full health and full mana!\n");
                    protag->currentHP = protag->maxHP;
                    protag->currentMana = protag->maxMana;
                }
                else{
                    printf("The spring tastes sickly sweet.. You get a stomachache and lose 1 HP.\n");
                    protag->currentHP--;
                    isAlive(protag);
                }
                break;
            case '2':
                printf("You step away from the spring and delve back into the forest.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }
    //If the number is between 5 and 15, the player can advance either to the caves or the town
    else if (random >= 10){
        printf("You found a hidden path deep into the forest. What will you do?\n");
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
    else if(random >= 5){
        printf("You see a village off in the distance. What will you do?\n");
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
        printf("You find a strange blue mushroom on the ground. What will you do with it?\n");
        do{
            printf("1. Eat It    2. Leave It Be\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                if(mushroom == 0){
                    printf("You eat the odd mushroom and suddenly.. You feel more powerful than ever!\n");
                    protag->maxHP++;
                    protag->currentHP++;
                    protag->currentMana++;
                    protag->maxMana++;
                    protag->speed++;
                    protag->physicalPower++;
                    protag->magicalPower++;
                    protag->currentExperience += 20;
                    printf("You have gained +1 to every stat, but you won't always be so lucky...\n");
                    mushroom++;
                }
                else{
                    printf("You eat the odd mushroom and suddenly.. You feel extremely nauseous and nothing happened.\n");
                }
                break;
            case '2':
                printf("You leave the mushroom alone and continue further into the forest.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

}

void exploreCave(charInformation *protag){
    int random, goldGained, damageTaken, rock = 0;
    char insideKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 85){
        printf("You found some loot!\n");
    }

    else if (random >= 70){
        printf("You are walking through the cave and feel yourself sink into the ground. You have just triggered a pressure plate!\nHow will you react?\n");
        do{
            printf("1. Roll Away    2. Rip The Pressure Plate Out Of The Ground\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                if(protag->speed > 15){
                        if((rand() % 100) >= 40)
                            printf("You roll away from the trap in time and emerge unscathed!\n");
                        else{
                            damageTaken = (protag->maxHP * .2);
                            protag->currentHP -= damageTaken;
                            printf("You manage to mostly dodge the trap only losing %d HP.\n", damageTaken);
                            isAlive(protag);
                        }
                }
                else if(protag->speed >= 11){
                    if((rand() % 100) >= 70){
                        printf("You roll away from the trap in time and emerge unscathed!\n");
                    }
                    else{
                        damageTaken = (protag->maxHP * .2);
                        protag->currentHP -= damageTaken;
                        printf("You manage to mostly dodge the trap only losing %d HP.\n", damageTaken);
                        isAlive(protag);
                    }
                }
                else{
                    damageTaken = (protag->maxHP * .35);
                    protag->currentHP -= damageTaken;
                    printf("You fail to get away from the trap in time and lost %d HP.\n", damageTaken);
                    isAlive(protag);
                }
                break;
            case '2':
                if (protag->physicalPower > 15){
                    printf("You rip out the pressure plate, completely disabling the trap!\n");
                }
                else{
                    damageTaken = (protag->maxHP * .35);
                    protag->currentHP -= damageTaken;
                    printf("You are not strong enough to pull out the pressure plate and lose %d HP.\n", damageTaken);
                    isAlive(protag);
                }
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 55){
        printf("As you wander through the caves, you notice a shadowy corridor. What do you do?\n");
        do{
            printf("1. Walk Through The Corridor   2. Keep Moving\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
                case '1':
                    printf("As you are walking down the corridor you ");
                    if (rock == 1){
                        printf("look down as the rock begin to glow.\nAs you keep walking, it leads you to a chasm filled with gold!\n");
                        rock++;
                        printf("After finding 100 gold, you feel more inspired for greatness. +1 to All Stats!\n");
                        protag->gold += 100;
                        protag->maxHP++;
                        protag->maxMana++;
                        protag->currentHP++;
                        protag->currentMana++;
                        protag->speed++;
                        protag->physicalPower++;
                        protag->magicalPower++;
                    }
                    else if((rand() % 100) >= 50){
                        printf("find yourself back where you began but feel invigorated. You have healed back to full health and mana!\n");
                        protag->currentHP = protag->maxHP;
                        protag->currentMana = protag->maxMana;
                    }
                    else {
                        printf("get lost and find yourself where you started.\n");
                    }
                    break;
                case '2':
                    printf("You ignore the corridor and keep walking.\n");
                    break;
                default:
                    printf("Say again?\n");
            }
        }while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 35){
        printf("As you wander through the caves, you notice a shadowy corridor. What do you do?\n");
        do{
            printf("1. Walk Through The Corridor   2. Keep Moving\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
                case '1':
                    printf("As you are walking down the corridor you ");
                    if((rand() % 100) >= 50){
                        printf("find yourself back where you began but feel invigorated. You have healed back to full health and mana!\n");
                        protag->currentHP = protag->maxHP;
                        protag->currentMana = protag->maxMana;
                    }
                    else {
                        printf("get lost and find yourself where you started.\n");
                    }
                    break;
                case '2':
                    printf("You ignore the corridor and keep walking.\n");
                    break;
                default:
                    printf("Say again?\n");
            }
        }while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 15){
        printf("You find a pickaxe leaning against a wall of ore. What will you do?\n");
        do{
            printf("1. Mine The Ore    2. Leave It Alone\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                if(protag->physicalPower > 15){
                    printf("You pick up the pickaxe and start swinging.\n");
                    if((rand() % 100) >= 66){
                        goldGained = (rand() % 30) + 1;
                        printf("After mining for about an hour, you finish breaking up the pocket of ore and gain %d gold!\n", goldGained);
                        protag->gold += goldGained;
                    }
                    else if((rand() % 100) >= 33){
                        printf("After mining for about an hour, you finish breaking up the pocket of ore.. only to discover it's fools gold!\n");
                    }
                    else{
                        printf("While mining the ore, you are attacked by monsters, losing your pickaxe in the process!\n");
                        if (protag->level < 7){
                            encounterMonster(protag);
                            encounterMonster(protag);
                        }
                        else{
                            encounterMonster(protag);
                            encounterMonster(protag);
                            encounterMonster(protag);
                        }
                    }
                }
                else{
                    printf("You grab hold of the pickaxe's handle and.. find yourself too weak to even lift it.\n");
                }
                break;
            case '2':
                printf("You walk past the pickaxe and continue exploring.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    //If the number is between 33 and 65, the player advances
    else if (random >= 10){
        printf("You find a caved in mineshaft. What will you do?\n");
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

    else if(random >= 5){
        printf("You find a way out of the caves but cannot see past some mist. What will you do?\n");
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
        if(rock == 0){
            printf("You find a shiny yellow rock and slip it into your pocket.\n");
            rock++;
        }
        else
            printf("You see a normal rock on the ground and kick it as you walk.\n");
    }

}

void exploreTown(charInformation *protag){
    int random, damageTaken;
    char insideKeypress, insiderKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 80){
        printf("You found some loot!\n");
    }

    else if (random >= 60){
        printf("You are walking through the town and spot a cellar. You enter the cellar and set off arrow traps! How will you react?\n");
        do{
            printf("1. Dodge The Arrows    2. Magically Destroy The Arrows\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                if(protag->speed > 15){
                        if((rand() % 100) >= 40)
                            printf("You dodge the arrows in time and emerge unscathed!\n");
                        else{
                            damageTaken = (protag->maxHP * .2);
                            protag->currentHP -= damageTaken;
                            printf("You manage to mostly dodge the arrows only losing %d HP.\n", damageTaken);
                            isAlive(protag);
                        }
                }
                else if(protag->speed >= 11){
                        if((rand() % 100) >= 70){
                            printf("You dodge the arrows in time and emerge unscathed!\n");
                        }
                        else{
                            damageTaken = (protag->maxHP * .2);
                            protag->currentHP -= damageTaken;
                            printf("You manage to mostly dodge the arrows only losing %d HP.\n", damageTaken);
                            isAlive(protag);
                        }
                }
                else{
                    damageTaken = (protag->maxHP * .35);
                    protag->currentHP -= damageTaken;
                    printf("You fail to dodge the arrows and lose %d HP.\n", damageTaken);
                    isAlive(protag);
                }
                break;
            case '2':
                if (protag->magicalPower > 15){
                    printf("You shoot down the arrows with magic leaving you untouched!\n");
                }
                else{
                    damageTaken = (protag->maxHP * .35);
                    protag->currentHP -= damageTaken;
                    printf("Your magical abilities are lacking, costing you %d HP.\n", damageTaken);
                    isAlive(protag);
                }
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 40){
        printf("You find two fruits lying on the ground. Which one do you eat?\n");
        do{
            printf("1. The Red Fruit    2. The Blue Fruit\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You eat the red fruit, and ");
                if ((rand() % 100) >= 50){
                    printf("you gain 2 Physical Power!\n");
                    protag->physicalPower = protag->physicalPower + 2;
                }
                else{
                    printf("you lose 2 Physical Power!\n");
                    protag->physicalPower = protag->physicalPower - 2;
                }
                break;
            case '2':
                printf("You eat the blue fruit, and ");
                if ((rand() % 100) >= 50){
                    printf("you gain 2 Magical Power!\n");
                    protag->magicalPower = protag->magicalPower + 2;
                }
                else{
                    printf("you lose 2 Magical Power!\n");
                    protag->magicalPower = protag->magicalPower - 2;
                }
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }
    else if (random >= 15){
        printf("You see that one of the doors to a house is slightly ajar. What will you do?\n");
        do{
            printf("1. Enter The House    2. Walk Past The House\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You slowly enter the house and look around. Where do you go?\n");
                do{
                    printf("1. The Bedroom    2. The Basement    3. The Kitchen    4. The Library\n");
                    scanf(" %c", &insiderKeypress);

                    switch(insiderKeypress){
                    case '1':
                        printf("You enter the bedroom and decide to take a nap in the empty bed.\n");
                        if ((rand() % 100) >= 50){
                            printf("You awake feeling refreshed and leave the house.");
                            protag->currentHP = protag->maxHP;
                            protag->currentMana = protag->maxMana;
                        }
                        else{
                            printf("You wake up to monsters standing over you, poised to attack!\n");
                            encounterMonster(protag);
                            encounterMonster(protag);
                            printf("You make a break for it and escape the house.\n");
                        }
                        break;
                    case '2':
                        printf("You enter the basement.. only to be assaulted by monsters!\n");
                        encounterMonster(protag);
                        encounterMonster(protag);
                        encounterMonster(protag);
                        printf("You make a break for it and escape the house.\n");
                        break;
                    case '3':
                        printf("You enter the kitchen, find some ingredients, and cook up a meal.\n");
                        if ( protag->magicalPower >= 12){
                            printf("You make an astounding meal and regain full health and mana!\n");
                            protag->currentHP = protag->maxHP;
                            protag->currentMana = protag->maxMana;
                        }
                        else {
                            printf("You create a mediocre meal that only bores you. You leave the house with slumped shoulders.\n");
                        }
                        break;
                    case '4':
                        printf("You enter the library and decide to flip through some books.\n");
                        if ((rand() % 100) >= 50){
                            printf("You learn a little more about magic and gained +1 Magical Power!\n");
                            protag->magicalPower++;
                        }
                        else{
                            printf("You learn new methods to beat slimes with sticks and gained +1 Physical Power!\n");
                            protag->physicalPower++;
                        }
                        break;
                    default:
                        printf("Say again?\n");
                    }
                } while( !( (insideKeypress >= '1') && (insideKeypress <= '4') ) );
                break;
            case '2':
                printf("You ignore the house and continue to investigate the village.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }
    //If the number is between 33 and 65, the player advances
    else if (random >= 10){
        printf("You find a path that leads into the mountain range. What will you do?\n");
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

    else if(random >= 5){
        printf("You see mist billowing in the distance. What will you do?\n");
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
        printf("You find a sack of gold on the ground, earning you 10 gold!\n");
        protag->gold = protag->gold + 10;
    }

}

void exploreGraveyard(charInformation *protag){
    int random, damageTaken, grave = 0;
    char insideKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 80){
        printf("You found some loot!\n");
    }

    else if (random >= 65){
        printf("While walking through the mist, you accidentally trip and fall.. into an open grave! You notice creatures approaching from the mist. What will you do?\n");
        do{
            printf("1. Face The Monsters    2. Play Dead\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                if (protag->physicalPower > 19){
                    printf("Your pure brawn scares the monsters away!\n");
                }
                else if (protag->physicalPower >= 15){
                    printf("Your fearlessness frightens some of the monsters away, leaving only a few stragglers.\n");
                    encounterMonster(protag);
                    encounterMonster(protag);
                }
                else {
                    printf("Your form is.. less than menacing. The monsters forward in an attempt to claim another life.\n");
                    encounterMonster(protag);
                    encounterMonster(protag);
                    encounterMonster(protag);
                    encounterMonster(protag);
                }
                break;
            case '2':
                if (protag->magicalPower > 16){
                    printf("You cast an illusion making it appear as though you are a rotting corpse.. and it works!\n");
                }
                else{
                    damageTaken = (protag->maxHP * .35);
                    protag->currentHP -= damageTaken;
                    printf("Playing dead works.. until a couple of enemies fall on top of you. You climb out and run as far as you can, losing %d HP in the process.\n", damageTaken);
                    isAlive(protag);
                }
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 50){
        printf("You find a shallow pool of water in the middle of the mist. You notice the stillness in the air and decide to take advantage of it. What do you do?\n");
        do{
            printf("1. Meditate    2. Drink The Water\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You sit next to the water and begin to meditate.\n");
                if ((rand() % 100) >= 25){
                    printf("You finish meditating feeling completely whole once again!\n");
                    protag->currentHP = protag->maxHP;
                    protag->currentMana = protag->maxMana;
                }
                else {
                    printf("You try to meditate, but cannot stay focused due to trickling water nearby.\n");
                }
                break;
            case '2':
                printf("You drink the water and.. nothing happened. It's just a regular pool of water.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }


    //If the number is between 33 and 65, the player advances
    else if (random >= 35){
        printf("Through the mist, you see the outline of a structure. As you move closer, you realize it is a mausoleum. What will you do?\n");
        do{
            printf("1. Enter The Mausoleum    2. Pass It\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You enter the mausoleum only to find monsters contained within! Who could have guessed?\n");
                if ((rand() % 100) >= 30){
                    encounterMonster(protag);
                    encounterMonster(protag);
                }
                else if ((rand() % 100) >= 5){
                    encounterMonster(protag);
                    encounterMonster(protag);
                    encounterMonster(protag);
                }
                else {
                    printf("Turns out, you interrupted a very important meeting. You have quite the ordeal ahead.\n");
                    encounterMonster(protag);
                    encounterMonster(protag);
                    encounterMonster(protag);
                    encounterMonster(protag);
                    encounterMonster(protag);
                }
                break;
            case '2':
                printf("You choose to pass the mausoleum, fearing potential monsters within.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 15){
        printf("Whilst wondering the mist, you find a shallow grave. What will you do?\n");
        do{
            printf("1. Dig Up The Grave    2. Keep Walking\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You decide to dig up the grave and at the bottom you find \n");
                if ((rand() % 100) >= 66){
                    printf("monsters lurking in wait!\n");
                    encounterMonster(protag);
                    encounterMonster(protag);
                    encounterMonster(protag);
                }
                else if ((rand() % 100) >= 33){
                    printf("nothing! You decide to keep moving.");
                }
                else {
                    printf("gold! You find 20 gold and swiftly add it to the rest.\n");
                    protag->gold = protag->gold;
                }
                break;
            case '2':
                printf("You walk past the grave and move further into the mist.\n");
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 10){
        printf("You spot a river leading out of the mist. What will you do?\n");
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

    else if(random >= 5){
        printf("As you move forward, you find the mist begins to slowly clear as the air dries. What will you do?\n");
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
        if (grave == 0){
            printf("You find a grave with the name %s. Disturbed by this, you destroy the headstone and keep moving.\n", protag->name);
            grave++;
        }
        else{
            printf("You see the remains of the headstone you smashed and keep moving.\n");
        }
    }

}

void exploreMountain(charInformation *protag){
    int random, damageTaken;
    char insideKeypress, insiderKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 80){
        printf("You found some loot!\n");
    }

    else if (random >= 65){
        printf("You find a crevice in the mountain with varying sizes of rocks and decide to build up some strength. What kind of strength will you work out?\n");
        do{
            printf("1. Physical Power    2. Magical Power\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You choose to build up your physical power. What size rock do you train with?\n");
                do{
                    printf("1. Small    2. Medium    3. Large\n");
                    scanf(" %c", &insiderKeypress);

                    switch(insiderKeypress){
                    case '1':
                        printf("You train using a small rock and gain +1 physical power.\n");
                        protag->physicalPower = protag->physicalPower + 1;
                        break;
                    case '2':
                        if (protag->physicalPower >= 15){
                            printf("You train using a small rock and gain +2 physical power.\n");
                            protag->physicalPower = protag->physicalPower + 2;
                        }
                        else{
                            damageTaken = (protag->maxHP * .1);
                            protag->currentHP -= damageTaken;
                            printf("You are not strong enough to train with this rock and hurt yourself for %d HP.\n", damageTaken);
                        }
                        break;
                    case '3':
                        if (protag->physicalPower >= 18){
                            printf("You train using a small rock and gain +3 physical power.\n");
                            protag->physicalPower = protag->physicalPower + 3;
                        }
                        else{
                            damageTaken = (protag->maxHP * .25);
                            protag->currentHP -= damageTaken;
                            printf("You are not strong enough to train with this rock and hurt yourself for %d HP.\n", damageTaken);
                        }
                        break;
                    default:
                        printf("Say again?\n");
                    }

                } while( !( (insideKeypress >= '1') && (insideKeypress <= '3') ) );
                break;
            case '2':
                printf("You choose to build up your magical power. What size rock do you train with?\n");
                do{
                    printf("1. Small    2. Medium    3. Large\n");
                    scanf(" %c", &insiderKeypress);

                    switch(insiderKeypress){
                    case '1':
                        printf("You train using a small rock and gain +1 magical power.\n");
                        protag->magicalPower = protag->magicalPower + 1;
                        break;
                    case '2':
                        if (protag->magicalPower >= 15){
                            printf("You train using a small rock and gain +2 magical power.\n");
                            protag->magicalPower = protag->magicalPower + 2;
                        }
                        else{
                            damageTaken = (protag->maxHP * .1);
                            protag->currentHP -= damageTaken;
                            printf("You are not strong enough to train with this rock and hurt yourself for %d HP.\n", damageTaken);
                        }
                        break;
                    case '3':
                        if (protag->magicalPower >= 18){
                            printf("You train using a small rock and gain +3 physical power.\n");
                            protag->magicalPower = protag->magicalPower + 3;
                        }
                        else{
                            damageTaken = (protag->maxHP * .25);
                            protag->currentHP -= damageTaken;
                            printf("You are not strong enough to train with this rock and hurt yourself for %d HP.\n", damageTaken);
                        }
                        break;
                    default:
                        printf("Say again?\n");
                    }

                } while( !( (insideKeypress >= '1') && (insideKeypress <= '3') ) );
                break;
            default:
                printf("Say again?\n");
            }
        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 50){
        printf("As you are walking you begin to feel tremors. You turn around and see a gigantic boulder rolling your way! How will you react?\n");
        do{
            printf("1. Duck Into A Nearby Cave    2. Charge At The Boulder\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                if (protag->speed >= 15){
                    printf("You duck into a nearby cave avoiding the boulder.\n");
                    encounterMonster(protag);
                    encounterMonster(protag);
                    encounterMonster(protag);
                }
                else{
                    damageTaken = (protag->maxHP * .35);
                    protag->currentHP -= damageTaken;
                    printf("You fail to make it to the cave, getting knocked down the mountain by the boulder in the process at the cost of %d HP.\n", damageTaken);
                    isAlive(protag);
                }
                break;
            case '2':
                printf("You rely on overwhelming strength and charge at the boulder.\n");
                if(protag->physicalPower >= 18){
                    printf("You completely demolish the boulder! Feeling pretty good about yourself, you enter the cave and take a refreshing nap, giving you full HP and Mana.\n");
                    protag->currentHP = protag->maxHP;
                    protag->currentMana = protag->maxMana;
                }
                else if(protag->physicalPower >= 15){
                    damageTaken = (protag->maxHP * .2);
                    protag->currentHP -= damageTaken;
                    printf("You manage to send the boulder off course only losing %d HP in the process.\n", damageTaken);
                    isAlive(protag);
                }
                else{
                    damageTaken = (protag->maxHP * .45);
                    protag->currentHP -= damageTaken;
                    printf("As you are about as strong as a noodle, you are comically rolled over by the boulder, costing you %d HP.\n", damageTaken);
                    isAlive(protag);
                }
                break;
            default:
                printf("Say again?\n");
            }
        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 35){
        printf("You reach the peak of the mountain, providing you with a majestic view. As you are looking down, you fail to notice monsters approaching from behind.\n How will you react?\n");
        do{
            printf("1. Face The Monsters    2. Slide Down The Mountain\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You turn around and prepare to fight!\n");
                encounterMonster(protag);
                encounterMonster(protag);
                encounterMonster(protag);
                break;
            case '2':
                printf("Without another thought, you face the side of the mountain and jump.. sliding for what seems like hours.\n");
                if((rand() % 100) >= 80){
                    printf("You slowly come to a stop without taking so much as a scratch!\n");
                }
                else if((rand() % 100) >= 40){
                    damageTaken = (protag->maxHP * .2);
                    protag->currentHP -= damageTaken;
                    printf("You land after getting a little scraped up on the way costing you %d HP\n", damageTaken);
                    isAlive(protag);
                }
                else{
                    damageTaken = (protag->maxHP * .2);
                    protag->currentHP -= damageTaken;
                    printf("You took quite the beating from that trip and have lost %d HP\n", damageTaken);
                    isAlive(protag);
                }
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 15){
        printf("As you climb up the mountain, you are greeted with a heavy snowstorm. What will you do?\n");
        do{
            printf("1. Hide In A Cave    2. Bear The Cold\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You choose to hide in a cave until the snowstorm blows over. What will you do to pass the time?\n");
                do{
                    printf("1. Build A Fire    2. Huddle In The Corner\n");
                    scanf(" %c", &insiderKeypress);

                    switch(insiderKeypress){
                    case '1':
                        printf("You warm yourself by a fire you built, giving you a chance to rest and to refill your health and mana.\n");
                        protag->currentHP = protag->maxHP;
                        protag->currentMana = protag->maxMana;
                        if ((rand() % 100) >= 60){
                            printf("After a short time, the snowstorm peters out and you venture back out into the mountains.\n");
                        }
                        else{
                            printf("After a short time, the snowstorm peters out and you venture back outside.. only to be met by monsters!\n");
                            encounterMonster(protag);
                            encounterMonster(protag);
                            encounterMonster(protag);
                        }
                        break;
                    case '2':
                        printf("Not risking the attention, you stay huddled in the corner until the storm passes.\n");
                        damageTaken = (protag->maxHP * .2);
                        protag->currentHP -= damageTaken;
                        printf("You lose %d HP from the chilling temperatures and continue your journey.\n", damageTaken);
                        isAlive(protag);
                        break;
                    default:
                        printf("Say again?\n");
                    }

                } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
                break;
            case '2':
                damageTaken = (protag->maxHP * .4);
                protag->currentHP -= damageTaken;
                printf("You foolishly keep moving in the freezing snowstorm losing %d HP in the process.\n", damageTaken);
                isAlive(protag);
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }
    //If the number is between 33 and 65, the player advances
    else if (random >= 10){
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

    else if(random >= 5){
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
        printf("You ponder the danger of wandering around mountains.. as you trip and fall down the valley.\nFortunately, you fell into deep water that refills your health and mana. What luck!\n");
        protag->currentHP = protag->maxHP;
        protag->currentMana = protag->maxHP;
    }

}

void exploreMarsh(charInformation *protag){
    int random, damageTaken;
    char insideKeypress, insiderKeypress;

    //We use a random number to determine the outcome of each exploration
    random = (rand() % 100);

    //If the number rolls 66 or above, the player finds loot
    if (random >= 80){
        printf("You found some loot!\n");
    }
    //If the number is between 33 and 65, the player advances
    else if (random >= 65){
        printf("You are walking through the marsh and notice a snare under your feet, almost a second too late. How will you react?\n");
        do{
            printf("1. Run Away    2. Jump Away\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                if (protag->speed > 22){
                    printf("Your speed enables you to hightail it out of the trap, just in the nick of time!\n");
                }
                else if (protag->physicalPower >= 17){
                    damageTaken = (protag->maxHP * .2);
                    protag->currentHP -= damageTaken;
                    printf("You begin to fall into the pit that has opened up, just barely grabbing the edge caused %d HP to be lost.\n", damageTaken);
                    isAlive(protag);
                }
                else {
                    damageTaken = (protag->maxHP * .35);
                    protag->currentHP -= damageTaken;
                    printf("You fall into the trap losing %d HP.\n", damageTaken);
                    isAlive(protag);
                }
                break;
            case '2':
                if (protag->physicalPower > 22){
                    printf("You summon all of your power into your legs and escape the trap!\n");
                }
                else if (protag->physicalPower >= 17){
                    damageTaken = (protag->maxHP * .2);
                    protag->currentHP -= damageTaken;
                    printf("You are just able to jump to the side of the pit, slamming your legs into the wall causing you to lose %d HP.\n", damageTaken);
                    isAlive(protag);
                }
                else {
                    damageTaken = (protag->maxHP * .35);
                    protag->currentHP -= damageTaken;
                    printf("You are lacking in physical power and lose %d HP.\n", damageTaken);
                    isAlive(protag);
                }
                break;
            default:
                printf("Say again?\n");
            }

        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 50){
        printf("You see smoke in the distance. What will you do?\n");
        do{
            printf("1. Move Towards The Smoke    2. Walk The Opposite Direction\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
                case '1':
                    printf("You approach the smoke and find monsters sitting around a fire, cooking up a meal. What do you do?\n");
                    do{
                        printf("1. Sneak Up On The Monsters    2. Charge At The Monsters\n");
                        scanf(" %c", &insideKeypress);

                        switch(insideKeypress){
                        case '1':
                            if (protag->speed >= 17){
                                printf("You silently take out the monsters in quick succession, earning 75 XP and allowing you to eat their meal and regain full health and mana.");
                                protag->currentExperience = protag->currentExperience + 75;
                                protag->currentHP = protag->maxHP;
                                protag->currentMana = protag->maxMana;
                            }
                            else{
                                printf("You stumble and the monsters grab their food and ready their attack.\n");
                                encounterMonster(protag);
                                encounterMonster(protag);
                                encounterMonster(protag);
                                encounterMonster(protag);
                            }
                            break;
                        case '2':
                            if (protag->speed >= 18){
                                printf("You charge the monsters scaring most of the monsters awayand regain full health and mana.\n");
                                encounterMonster(protag);
                                encounterMonster(protag);
                                printf("With the stragglers taken care of, you eat the meal and regain full health and mana.\n");
                                protag->currentHP = protag->maxHP;
                                protag->currentMana = protag->maxMana;
                            }
                            else{
                                printf("The monster munch on their meal and charge at you.\n");
                                encounterMonster(protag);
                                encounterMonster(protag);
                                encounterMonster(protag);
                            }
                            break;
                        default:
                            printf("Say again?\n");
                            break;
                        }
                    } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
            case '2':
                printf("You turn around and move forward in blissful ignorance.\n");
                break;
            default:
                printf("Say again?\n");
            }
        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 30){
        printf("While walking through the marsh, you notice you are surrounded by brush on all sides.. and get ambushed!\n");
        if (protag->level < 6){
            encounterMonster(protag);
            encounterMonster(protag);
            encounterMonster(protag);
        }
        else{
            encounterMonster(protag);
            encounterMonster(protag);
            encounterMonster(protag);
            encounterMonster(protag);
        }
    }

    else if (random >= 10){
        printf("You notice a small shack in the middle of the woods. What will you do?\n");
        do{
            printf("1. Approach The Shack    2. Pass It By\n");
            scanf(" %c", &insideKeypress);

            switch(insideKeypress){
            case '1':
                printf("You approach the shack and notice the faint smell of rot. Do you continue moving forward?\n");
                do{
                    printf("1. Continue Towards The Shack    2. Turn Back\n");
                    scanf(" %c", &insiderKeypress);

                    switch(insiderKeypress){
                    case '1':
                        printf("You enter the shack to find ");
                        if ((rand() % 100) >= 66){
                            printf("a dead rat, rotting in an empty room. You leave grossed out and disappointed.\n");
                        }
                        else if ((rand() % 100) >= 33){
                            printf("a foul smelling potion which refills your health and mana!\n");
                            protag->currentHP = protag->maxHP;
                            protag->currentMana = protag->maxMana;
                        }
                        else{
                            printf("a collective of monsters, all ready and waiting to pounce!\n");
                            encounterMonster(protag);
                            encounterMonster(protag);
                            encounterMonster(protag);
                            encounterMonster(protag);
                        }
                        break;
                    case '2':
                        printf("You decide against continuing further, only to be ambushed by monsters!\n");
                        encounterMonster(protag);
                        encounterMonster(protag);
                        break;
                    default:
                        printf("Say again?\n");
                    }
                } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
                break;
            case '2':
                printf("You are not ready to discover your fate and return to the marsh.\n");
                break;
            default:
                printf("Say again?\n");
            }
        } while( !( (insideKeypress >= '1') && (insideKeypress <= '2') ) );
    }

    else if (random >= 5){
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
        printf("You find a strange wooden outhouse near the edge of the marsh. How odd.\n");
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
    else if (random >= 5){
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
                    if( protag->currentMana >= USED_MANA_FOR_HEAL ){
                        protag->currentMana -= USED_MANA_FOR_HEAL;

                        //50-50 chance to to either heal/damage a little more or a little less than average
                        if( protag->level < 1){
                            if( (rand() % 100) >= 50 ){
                                randMod = -1;
                            }
                            else{
                                randMod = 1;
                            }
                        }
                        else if( protag->level >= 4){
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
                            printf("For the cost of %d Mana, you heal for %d HP!\n", USED_MANA_FOR_HEAL, healedHP);
                        }
                        else if( strcmp(protag->job, "berserker") == 0 ){
                            //The berserker cannot heal, but can gain a temporary damage buff that lasts until the end of the fight
                            damageBuff = protag->physicalPower + randMod;
                            protag->physicalPower = (protag->physicalPower * 1.5) + .5 + randMod;
                            damageBuff -= protag->physicalPower;

                            printf("For the cost of %d Mana, you now do %d damage with a %d damage buff!\n\n", USED_MANA_FOR_HEAL, protag->physicalPower, abs(damageBuff));
                        }
                        else if( strcmp(protag->job, "mage") == 0 ){
                            //This process is the same as the paladin's heal, just slightly weaker
                            healedHP = protag->maxHP * .33 + randMod;
                            protag->currentHP += (protag->maxHP * .33);

                            if(protag->currentHP > protag->maxHP){
                                overHP = protag->currentHP - protag->maxHP;
                                protag->currentHP = protag->maxHP;
                                healedHP -= overHP;
                            }
                            printf("For the cost of %d Mana, you heal for %d HP!\n", USED_MANA_FOR_HEAL, healedHP);
                        }
                        else if( strcmp(protag->job, "cleric") == 0 ){
                            //This process is the same as the paladin's heal, just slightly stronger
                            healedHP = protag->maxHP * .66 + randMod;
                            protag->currentHP += (protag->maxHP * .66);

                            if(protag->currentHP > protag->maxHP){
                                overHP = protag->currentHP - protag->maxHP;
                                protag->currentHP = protag->maxHP;
                                healedHP -= overHP;
                            }
                            printf("For the cost of %d Mana, you heal for %d HP!\n", USED_MANA_FOR_HEAL, healedHP);
                        }
                        monsterAttacksPlayer(protag, monsterStats, monsterName);
                    }
                    else{
                        printf("You do not have enough Mana to do that.\n");
                    }
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
    else
        isAlive(protag);
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
        if( protag->level <= 4)
            if( (rand() % 100) >= 50 ){
                randDamageMod = -1;
            }
            else{
                randDamageMod = 1;
            }

        //If the player reaches a high enough level, damage dealt has a slightly larger range
        else if( protag->level >= 5){
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

void isAlive(charInformation *protag){
    if( protag->currentHP <= 0 )
    {
        printf("You have been slain.\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GAME OVER~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        exit(0);
    }
}
