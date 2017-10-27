#ifndef RISK_H
#define RISK_H
#include <stdbool.h>


typedef struct players {
    const char *color;
    int troops;
    struct players *next;
    struct cards *deck; 
    struct territories *countries; 
}player;

typedef struct continents {
    const char *name;
    char *color;
    int num_countries;
    int bonus;
    struct continents *previous;
    struct continents *next;
    struct territories *head;
}continent;

typedef struct territories {
    const char *name;
    char *color;
    int num_troops;
    struct continents *home;
    struct territories *next;
    struct territories *previous; 
}territory;

typedef struct maps {
    continent *head;
}map;


typedef struct cards {
    const char *country;
    const char *value;
    bool wild;
}card;   


void talk_back(int client_fd, char **buf, const char *message, bool err);
int game_setup(int client_fd, player **competitor);
int country_selection(void);
int draw_card(void);
int build_card(card **deck, const char **array, int a, int c); 
int build_deck(card **deck);
int player_initiate(int num_players, player **competitor);
int player_teardown (player *competitor, int num_players);
int build_continents(continent **head);
int build_territories(territory *head, int country);
int free_graph(continent *cont_handle);



#endif
