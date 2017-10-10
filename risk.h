#ifndef RISK_H
#define RISK_H

typedef struct players {
    char *color;
    int troops;
    struct players *next;
    //LL cards 
    //LL Territories 
}player;

typedef struct continents {
    char *name;
    char *color;
    int num_countries;
    int bonus;
    struct continents *previous;
    struct continents *next;
    struct territories *head;
}continent;

typedef struct territories {
    char *name;
    char *color;
    int num_troops;
    struct continents *home;
    struct territories *next;
    struct territories *previous; 
}territory;

typedef struct maps {
    continent *head;
}map;

typedef struct games {
    struct continents *map;
    struct players *competitors;
} game;
    


#endif
