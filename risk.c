<<<<<<< HEAD

=======
//<This is the branch for Risk>
>>>>>>> risk_temp.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "risk.h"
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_CONTINENTS      4
#define FULL_DECK           58
#define AF_SZ               5
#define EU_SZ               4
#define NO_SZ               5
#define AS_SZ               4
#define MAX_PLAYERS         4
#define MIN_PLAYERS         2
#define ERR                 -1
#define SUCCESS             0

const char *continent_array[] = {"Africa", "Europe", "North America", "Asia"};

const char *africa_array[AF_SZ] = {"Morroco", "South Africa", "Algeria", "Nigeria", "Ethiopia"};
const char *europe_array[EU_SZ] = {"Germany", "France", "Italy", "United Kingdom"};
const char *north_array[NO_SZ] = {"USA", "Canada", "Mexico", "Cuba", "Dominican Republic"};
const char *asia_array[AS_SZ] = {"China", "India", "Japan", "North Korea"};
const char **temp_array[NUM_CONTINENTS] = {africa_array, europe_array, north_array, asia_array};

const char *color_array[4] = {"Blue", "Red", "Green", "Yellow"};   
const char *card_values[4] = {"Artillery", "Infantry", "Cavalry"};

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int results; //function result for error checking

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints)); 
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    //Retrieve linked lists
    int status;
    if((status = getaddrinfo("localhost", "5555", &hints, &res)) == -1){
        fprintf(stderr, "getaddrinfo error %s \n", argv[1]);
        exit(1);
    }

    //Create socket
    int sfd;
    if((sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0){
        perror("Bad socket\n");
        exit(1);
    }

    //Allow replay on same port
    int yes = 1;
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1){
        perror("setsockopt error \n");
        exit(1);
    }
    

    //Bind socket
    int bind_err;
    if((bind_err = bind(sfd, res->ai_addr, res->ai_addrlen)) < 0){
        fprintf(stderr, "bind error %d \n", bind_err);
        exit(1);
    }

    //Once bound no longer need linked list
    freeaddrinfo(res);
 
    //listen on socket
    int lis_err;
    if((lis_err = listen(sfd, 10)) == -1){
        fprintf(stderr, "listen error %d \n", lis_err);
        exit(1);
    }
 
    char buf[100] ; 
    struct sockaddr_storage client = {0};
    socklen_t client_len = 0;

    int newsd = 0;
    newsd = accept(sfd, (struct sockaddr *)&client, &client_len);

    //create initial player                                                     GAME SETUP
    player *competitor = NULL;
    int num_players;          
    if((results = game_setup(newsd, &competitor)) == ERR){
            return(results);
    }   
    
    num_players = results;

    char *message = calloc(100, sizeof(char));
    for(int a = 1; a < num_players + 1; a++){
        sprintf(message,"output: player %d: %s\n", a, competitor->color);
        send(newsd, message, strlen(message),0);
        memset(message, 0, 100);
        if(a + 1 < num_players + 1){
            competitor = competitor->next;
            continue;
        }
        break;
    }
    
    free(message);

    message = "quit";
    send(newsd, message, strlen(message), 0);

    close(newsd);
    close(sfd);

/* 
    //Deck of cards                                                                 GAME SETUP
    card *deck = NULL; 
    //If deck creation fails, free necessary items and return
    if((results = build_deck(&deck)) == ERR){
        //player_teardown(competitor, results);
        return(results);
    }
  */ 
    //Head to graph                                                                 GAME SETUP
    //If they fail, free necessary items and return
    continent *globe = NULL;
    if((results = build_continents(&globe)) == ERR){ 
    //    free(deck);
        //player_teardown(competitor, results);
        return(results);
    }

    //find_territory(globe, "Mexico");
   
   // free(deck);
   
    player_teardown(competitor, num_players); 

    free_graph(globe);    
    //Program ran successfully to completion
    return SUCCESS;
    
}
////////END MAIN//////////

void talk_back(int client_fd, char **buf, const char *message, bool err)
{
    size_t received;
    int leave = 0;

    
    send(client_fd,  message, strlen(message), 0);

    /* 
     * For the time being, the client should always send something back.
     * However, This may change later on as the rest of the build out cont.
     * In the mean time, bool is unnecessary but will remain. 
     */

    recv(client_fd, *buf, 100, 0);
    
 
}

  
int game_setup(int client_fd, player **competitor)
{
    char *buf = calloc(100, sizeof(char));
    char *err;
    int move_on = 0;
    int result;
    int ret_val;
    bool is_err = false;
    

    const char *message = "How many people are playing <2-4>? ";
    const char *err_msg = "ERR: Please enter a number between <2-4> ";

    while(move_on == 0)
    {
        talk_back(client_fd, &buf, message, is_err);
        printf("%s \n", buf);                       //Error checking print statement
        result = strtol(buf, &err, 10);
        if(*err != '\0'){
            memset(buf, 0, 100);
            is_err = true;
            message = err_msg;
            continue;
        }
        if(result > MAX_PLAYERS || result < MIN_PLAYERS){
            memset(buf, 0, 100);
            is_err = true;
            message = err_msg;
            continue;
        }
        move_on = 1;
    }

    //If player creation fails, return
    if((ret_val = player_initiate(result, competitor)) == ERR){
        printf("player_initiate error\n");
        return(ret_val);
    }
  
    free(buf);

    return result;   // This needs to turn into an error checking return


}

/*
 * Everything below needs to be moved to a .c file possibly game.c?
 */

int find_territory(continent *country, const char *name)
{
    territory *search = country->head;
    int ret_val;
    
   while((ret_val=strncmp(name, search->name, strlen(name))) != 0) {
        if(search->next == NULL){
            country = country->next;
            search = country->head;
        }
        else{
            search = search->next;
        }
    }
    printf("%s vs: %s\n", name, search->name);
    return(0);
}

int country_selection(void)
{
    char buf[100];

    printf("Africa   |  Europe  |   North America   |   Asia\n");
    printf("-------------------------------------------------\n");
    printf("Morroco  |  Germany |   USA             |   China\n");
    printf("South Af.|  France  |   Canada          |   India\n");
    printf("Algeria  |  Italy   |   Mexico          |   Japan\n");
    printf("Nigeria  |  U.K.    |   Cuba            |   North Korea\n");
    printf("Ethiopia |          |   Dominican Rep.  | \n");

    /*
    printf("Type the country for which you would like to assign troops\n");
    fgets(buf, sizeof(buf), stdin);
    printf("%s \n", buf);    
    */
    return(0);
    
}
int draw_card(void)
{
    int card = 0;
    card = rand() % FULL_DECK;
    return(card);
}
    
int build_deck(card **deck)
{
    int c = 0;
    int a = 0;

    //malloc for a deck of cards
    if((*deck = malloc(sizeof(card) * FULL_DECK)) == NULL)
    {
        printf("malloc error inside build_deck");
        return(ERR);
    }

    //populate country names into cards
    //each country has three cards
    //each country has three values
    for(a = 0; a < NUM_CONTINENTS; a++) {
        c = build_card(deck, temp_array[a], a, c);
    }

    //finalize deck with four wild cards
    for(a = 0; a < NUM_CONTINENTS; a++){
        (*deck)[c].country = "Wild";
        (*deck)[c].value = "Wild";
        (*deck)[c].wild = true;
        c++;
    }
     
    return SUCCESS;
}

int build_card(card **deck, const char **array, int a, int c)
{
    
    int num_countries = 0;
    
    //variable number of iterations dependant on the number of countries in a continent
    switch(a)
    {
        case 0:
            num_countries = AF_SZ;
            break;
        case 1:
            num_countries = EU_SZ;
            break;
        case 2:
            num_countries = NO_SZ;
            break;
        case 3:
            num_countries = AS_SZ;
            break;
    }
            
    //populate the cards with country names, values, and set wild to false
    for(int b = 0; b < num_countries; b++){
        for(int e = 0; e < 3; e++){
            (*deck)[c].wild = false;
            (*deck)[c].country = array[b];
            (*deck)[c].value = card_values[e];
            c++;
        }
    }

    //use to keep track of palce in array
    return c;
}

   
//Build out a circularly linked list of players 
int player_initiate(int num_players, player **competitor)
{
    
    int num_troops = 0; 
    //Determine the number of troops per player
    switch(num_players)
    {
        case 2:
            num_troops = 40;
        case 3:
            num_troops = 35;
        case 4:
            num_troops = 30;
    }
    
 
    //build out linked list based off the number of players passed to the program
    int a = 0;
    
    if((*competitor = malloc(sizeof(player))) == NULL){
        printf("malloc error inside player_initiate\n");
        return(ERR);
    }

    //A player pointer, temporary
    player *temp;
    //assign temp to the competitor pointer
    temp = *competitor;
    
    for(a = 0; a < num_players; a++)
    {
        //assign the color in the array to the player   
        (*competitor)->color = color_array[a];
        //assign the player the appropriate number of troops
        (*competitor)->troops = num_troops;
        /*
        if(((*competitor)->deck = malloc(sizeof(card))) == NULL){
            printf("error in deck malloc");
            return(ERR);
        }

        if(((*competitor)->countries = malloc(sizeof(territory))) == NULL ) {
            printf("error in territory malloc");
            return(ERR);
        }
       */ 
        if(a + 1 < num_players){
            if(((*competitor)->next = malloc(sizeof(player))) == NULL){
                printf("error in player malloc");
                return(ERR);
            }
            *competitor = (*competitor)->next;
        }
        else{
            (*competitor)->next = temp;
            *competitor = (*competitor)->next;
        }
        
    }

    return SUCCESS;
}


//Function to build out continents 
int build_continents(continent **cont_node)
{

    int ret;

    if((*cont_node = malloc(sizeof(continent))) == NULL){
        printf("malloc error inside build_continents\n");
        return(ERR);
    }

    continent *temp = *cont_node;
    for(int a = 0; a < NUM_CONTINENTS; a++){
        //cont_node->name = malloc(strlen(continent_array[a]));
        (*cont_node)->name = continent_array[a];
        (*cont_node)->color = "White";
        
        (*cont_node)->head = malloc(sizeof(territory));

        ret = build_territories((*cont_node)->head, a);

        if((a + 1) < 4){
            (*cont_node)->next = malloc(sizeof(continent));
            *cont_node = (*cont_node)->next;
        }
        else{
            (*cont_node)->next = temp;
           *cont_node = (*cont_node)->next;
        }
    } 
   
    return SUCCESS;
}

//Function to build out territories
int build_territories(territory *head, int country)
{
    
    int count = 0;

    territory *temp = head;

    switch(country)
    {
        case 0:
            count = AF_SZ;
        case 1:
            count = EU_SZ;
        case 2:
            count = NO_SZ;
        case 3:
            count = AS_SZ;
    }
    
    for(int a = 0; a < count; a++){
            head->name = temp_array[country][a];
            head->color = "White";
            head->num_troops = 0;
            if(a + 1 < count){
                if((head->next = malloc(sizeof(territory))) == NULL){
                    printf("malloc error inside of build_territories \n");
                    return(ERR);
                }
                head->previous = NULL;
                head = head->next;
            }
            else{
                head->next = NULL;
            }
    }

    return SUCCESS;
} 


//free all the malloced players
int player_teardown(player *competitor, int num_players)
{
    player *temp;
    int a = 0;
    //duplicate->free->move->repeat
    for(a; a  < (num_players); a++){
        temp = competitor->next;
        //   free(competitor->countries);
        //   free(competitor->deck);
        free(competitor);
        competitor = temp;
       
    }
    
    return SUCCESS;
}

//Free the continents and the territories  
int free_graph(continent *cont_node)
{

    continent *cont_handle = cont_node;
    territory *ter_handle;
    territory *b;
    for(int a = 0; a < NUM_CONTINENTS; a++){
        while(cont_node->head != NULL){
            ter_handle = cont_node->head->next;
            free(cont_node->head);
            cont_node->head = ter_handle; 
        }
        if(a+1 < NUM_CONTINENTS){
            cont_handle = cont_node->next;
            free(cont_node);
            cont_node = cont_handle;
        }
        else{
            free(cont_node);
        }
    }
    return SUCCESS;
}
    

