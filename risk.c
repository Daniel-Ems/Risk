#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "risk.h"

int player_initiate(int num_players, player *competitor);
int player_teardown (player *competitor, int num_players);
int build_continents(continent *head);
int build_territories(territory *head, int country);

int main(int argc, char *argv[])
{
    if(argc != 2) {
        fprintf(stderr, "%s <num players (2-4)> \n", argv[0]);
        return 1;
    }

    //turns string into number
    int num_players = strtol(argv[1], NULL, 10);
    
    //error checking for numbers
    if(!num_players){
        fprintf(stderr, "%s <num players (2-4)> \n", argv[0]);
        return 1;
    }
    
    //error checking for appropriate value 
    if(num_players > 4){
        fprintf(stderr, "%s <num players (2-4)> \n", argv[0]);
        return 1;
    }

    //error checking     
    printf("num_players = %d\n", num_players);

    //create initial player
    player *competitor = malloc(sizeof(*competitor)); 
   
    int success = player_initiate(num_players, competitor);

    //print player attributes
    int a = 1;
    
    //print player stats
    while(a < (num_players + 1)){
        printf("player %d:color %s:troops %d\n", a, competitor->color, competitor->troops);
        competitor = competitor->next;
        a++;
    }  
    
    //Create the head to the graph. 
    continent *globe = malloc(sizeof(continent));
    int b = build_continents(globe);

    for(int a = 0; a < 4; a++){
        printf("%s - %s\n", globe->name, globe->color);
        for(territory *b = globe->head; b != NULL; b = b->next){
            printf("territory %s\n", b->name);
        }
            
        globe = globe->next;
    }
    
    //destroy player function
    success = player_teardown(competitor, num_players);
    return 0;
    
}
////////END MAIN//////////

////////BEGINNING FUNCTIONS ////////// 

//Build out a circularly linked list of players 
int player_initiate(int num_players, player *competitor)
{
    //Colors assigned to players 
    char color_array[4][10] = {"Blue", "Red", "Green", "Yellow"};   
     
    //Determine the number of troops per player
    int num_troops;
    if(num_players == 2){
        num_troops = 40;
    }else if(num_players == 3){
        num_troops = 35;
    }else{
        num_troops = 30;
    }   
 
    //build out linked list based off the number of players passed to the program
    int a = 0;
    player *temp = competitor;
    for(a = 0; a < num_players; a++)
    {   
        competitor->color = color_array[a];
        competitor->troops = num_troops;
        if(a + 1 < num_players){
            competitor->next = malloc(sizeof(player));
            competitor = competitor->next;
        }
        else{
            competitor->next = temp;
            competitor = competitor->next;
        }
        
    }
    return 0;
}

//free all the malloced players
int player_teardown(player *competitor, int num_players)
{
    player *temp;
    int a = 0;
    //duplicate->free->move->repeat
    while(a < (num_players)){
        temp = competitor->next;
        free(competitor);
        competitor = temp;
        a++; 
    }
    return 0;
}

//Function to build out continents 
int build_continents(continent *cont_node){

    char continent_array[4][20] = {"Africa", "Europe", "North America", "Asia"};

    continent *temp = cont_node;
    for(int a = 0; a < 4; a++){
        //cont_node->name = malloc(strlen(continent_array[a]));
        cont_node->name = continent_array[a];
        cont_node->color = "White";
        
        cont_node->head = malloc(sizeof(territory));

        int ret = build_territories(cont_node->head, a);

        if((a + 1) < 4){
            cont_node->next = malloc(sizeof(*cont_node));
            cont_node = cont_node->next;
        }
        else{
            cont_node->next = temp;
            cont_node = cont_node->next;
        }
    } 
   
    //THE ERROR IS PRINTED HERE. HOWEVER, IS THIS THE SOURCE?????
    //THE ISSUE IS THE CONT_NODE->HEAD->NAME 
    
    return 0;
}

//Function to build out territories
int build_territories(territory *head, int country){

    char africa_array[5][20] = {"Morocco", "South Africa", "Algeria", "Nigeria", "Ethiopia"};
   
    char europe_array[4][20] = {"Germany", "France", "Italy", "United Kingdom"};

    char north_array[5][20] = {"USA", "Canada", "Mexixo", "Cuba", "Dominican Republic"};

    char asia_array[4][20] = {"China", "India", "Japan", "North Korea"};

    
    int count = 0;
    territory *temp = head;
    if(country == 0){
        
        count = 5;

        for(int a = 0; a < count; a++){
            //head->name = malloc(strlen(africa_array[a]));
            head->name = africa_array[a];
            head->color = "White";
            head->num_troops = 0;
            if(a + 1 < count){
                head->next = malloc(sizeof(*head));
                head->previous = NULL;
                head = head->next;
            }
            else{
                head->next = NULL;
            }
        }

    }

    else if(country == 1){

        
        count = 4;
        for(int a = 0; a < count; a++){
            //head->name = malloc(strlen(europe_array[a]));
            head->name = europe_array[a];
            head->color = "White";
            head->num_troops = 0;
            if(a + 1 < count){
                head->next = malloc(sizeof(*head));
                head->previous = NULL;
                head = head->next;
            }
            else{
                head->next = NULL;
            }
        }
    }
    else if(country == 2){
        
        count = 5;
        for(int a = 0; a < count; a++){
            //head->name = malloc(strlen(north_array[a]));
            head->name = north_array[a];
            head->color = "White";
            head->num_troops = 0;
            if(a + 1 < count){
                head->next = malloc(sizeof(*head));
                head->previous = NULL;
                head = head->next;
            }
            else{
                head->next = NULL;
            }
        }
    }
    else{
        count = 4;
        for(int a = 0; a < count; a++){
            //head->name = malloc(strlen(asia_array[a]));
            head->name = asia_array[a];
            head->color = "White";
            head->num_troops = 0;
            if(a + 1 < count){
                head->next = malloc(sizeof(*head));
                head->previous = NULL;
                head = head->next;
            }
            else{
                head->next = NULL;
            }
        }
    }
    

    return 0;
} 
 
  
    
/*
int randomize(long number);

int main(int argc, char *argv[])
{
    
    time_t t;
    srand((unsigned) time(&t));

    if (argc != 2) {
        fprintf(stderr, "%s <num players (2 - 4)> \n", argv[0]);
        return 1;
    }
    
    int lat_var = 0;
    int lon_var = 0;
    float half_lat = 0;
    float half_lon = 0;
    int country_x;
    int country_y;
    
    lat_var = randomize(lat_var, 40, 70);
    lon_var = randomize(lon_var, 40, 70);

    half_lat = floor(lat_var / 2);
    half_lon = floor(lon_var / 2);


    printf("lat_var = %d (%f)", lat_var, half_lat);
    printf("lon_var = %d (%f)\n", lon_var, half_lon);

    if(lon_var > lat_var){
        int temp = lat_var;
        lat_var = lon_var;
        lon_var = temp;
    }

    half_lat = floor(lat_var / 2);
    half_lon = floor(lon_var / 2);

    country_x = randomize(country_x, 0, lat_var/2);
    country_y = randomize(country_y, 0, lon_var/2);

   
    char array[lon_var][lat_var];

    int a;
    for(int a = 0; a < lat_var; a++){
        for(int b = 0; b < lon_var; b++){
            if(a == 0 || b == 0){
                array[b][a] = '#';
            }
            else if(b == lon_var - 1 || a == lat_var - 1){
               array[b][a] = '#';
            }
            else if(a == half_lat || b == half_lon){
                array[b][a] = ' ';
            }
            else{
                array[b][a] = '^';
            }
        }
    }
  
    for( int a = 0; a < lat_var; a++){
        puts("");
        for(int b = 0; b < lon_var; b++){
                printf("%c ", array[b][a]);
            }
        }
 
    puts(""); 
    

}

int randomize(int number, int min, int max){

    while(number < min){
        number = rand() % max;
    }

    return number;
}
*/
