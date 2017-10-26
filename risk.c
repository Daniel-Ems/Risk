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

#define NUM_CONTINENTS      4
#define FULL_DECK           58
#define AF_SZ               5
#define EU_SZ               4
#define NO_SZ               5
#define AS_SZ               4
#define MAX_PLAYERS         4
#define ERR                 -1
#define SUCCESS             0

const char *continent_array[] = {"Africa", "Europe", "North America", "Asia"};

const char *africa_array[AF_SZ] = {"Morroco", "South Africa", "Algeria", "Nigeria", "Ethiopia"};
const char *europe_array[EU_SZ] = {"Germany", "France", "Italy", "United Kingdom"};
const char *north_array[NO_SZ] = {"USA", "Canada", "Mexixo", "Cuba", "Dominican Republic"};
const char *asia_array[AS_SZ] = {"China", "India", "Japan", "North Korea"};
const char **temp_array[NUM_CONTINENTS] = {africa_array, europe_array, north_array, asia_array};

const char *color_array[4] = {"Blue", "Red", "Green", "Yellow"};   
const char *card_values[4] = {"Artillery", "Infantry", "Cavalry"};

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int results; //function result for error checking
    char *err = "<num players (2-4)>";
    if(argc != 2) {
        fprintf(stderr, "%s %s \n",argv[0], err);
        return 1;
    }

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

    //Bind socket
    bind(sfd, res->ai_addr, res->ai_addrlen);

    //Once bound no longer need linked list
    freeaddrinfo(res);
 
    //listen on socket
    int lis_err;
    if((lis_err = listen(sfd, 10)) == -1){
        fprintf(stderr, "listen error %d \n", lis_err);
        exit(1);
    }
    
    //Allow replay on same port
    int yes = 1;
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1){
        perror("setsockopt error \n");
        exit(1);
    }
    
    
    char buf[100] ; 
    struct sockaddr_storage client = {0};
    socklen_t client_len = 0;

    int newsd = 0;
    newsd = accept(sfd, (struct sockaddr *)&client, &client_len);

    game_setup(newsd);   
  
    close(newsd);
    close(sfd);

    /* 
    //turns string into number  -                                                   GAME SETUP
    int num_players = strtol(argv[1], NULL, 10);
    
    
    //error checking for number of arguments                                        GAME SETUP
    if(!num_players){
        fprintf(stderr, "%s %s \n",argv[0], err);
        return 1;
    }
    
    //error checking for appropriate number of players                              GAME SETUP
    if(num_players > MAX_PLAYERS){
        fprintf(stderr, "%s %s \n",argv[0], err);
        return 1;
    }

    //error checking                                                                GAME SETUP
    printf("num_players = %d\n", num_players);

    //create initial player                                                         GAME SETUP
    player *competitor = NULL;          
    //If player creation fails, return
    if((results = player_initiate(num_players, &competitor)) == ERR){
        return(results);
    }
 
    //Deck of cards                                                                 GAME SETUP
    card *deck = NULL; 
    //If deck creation fails, free necessary items and return
    if((results = build_deck(&deck)) == ERR){
        player_teardown(competitor, num_players);
        return(results);
    }
    
    //Head to graph                                                                 GAME SETUP
    continent *globe = NULL;
    //Build out continents and territories
    //If they fail, free necessary items and return
    if((results = build_continents(&globe)) == ERR){
        player_teardown(competitor, num_players);
        free(deck);
        return(results);
    }

    //TODO: FINISH                                                                  GAME SETUP  
    country_selection(); 
   
                                                         
    //FREES
    //TODO -> Check success
    free_graph(globe);    
    free(deck);
    player_teardown(competitor, num_players);
    */ 
    //Program ran successfully to completion
    return SUCCESS;
    
}
////////END MAIN//////////
char *talk_back(int client_fd, const char *message)
{
    char *buf = calloc(100, sizeof(char));
    size_t received;
    int leave = 0;

    send(client_fd,  message, strlen(message), 0);
    recv(client_fd, buf, 100, 0);
 
    
    return(buf);
}

  
int game_setup(int client_fd)
{
    char *buf;
    char *err = NULL;
    int move_on = 0;
    int result;

    const char *message = "How many people are playing <2-4>? ";
    const char *err_msg = "Please enter a number between <2-4> ";
    while(move_on == 0){
        buf = talk_back(client_fd, message);
        printf("%s \n", buf);
        result = strtol(buf, &err, 10);
        printf("%d \n", result);
        /*
        if(err != NULL){
            memset(&buf, 0, sizeof(buf));
            err = NULL;
            buf = talk_back(client_fd, err_msg);
            continue;
        }
        */
        move_on = 1;
    }
           
    printf("Thank you\n%s are playing \n", buf);
   
    message = "quit";
    send(client_fd, message, strlen(message), 0);
    free(buf);
    return 0;
    //Number of Players 

    //Country selection 

}

////////BEGINNING FUNCTIONS ////////// 
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
    
    if((*competitor = malloc(sizeof(**competitor))) == NULL){
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
        if(((*competitor)->deck = malloc(sizeof(card) * 18)) == NULL){
            printf("error in deck malloc");
            return(ERR);
        }

        if(((*competitor)->countries = malloc(sizeof(territory) * 18)) == NULL ) {
            printf("error in territory malloc");
            return(ERR);
        }

        if(a + 1 < num_players){
            (*competitor)->next = malloc(sizeof(player));
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

    if((*cont_node = malloc(sizeof(**cont_node))) == NULL){
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
            (*cont_node)->next = malloc(sizeof(territory));
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
        free(competitor->countries);
        free(competitor->deck);
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

    for(int a = 0; a < NUM_CONTINENTS; a++){
        for(territory *b = cont_node->head; b; b = ter_handle){
            ter_handle = b->next;
            free(b);
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
    

