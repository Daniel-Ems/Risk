#ifndef RISK_H
#define RISK_H


const char *continent_array[] = {"Africa", "Europe", "North America", "Asia"};

const char *africa_array[5] = {"Morroco", "South Africa", "Algeria", "Nigeria", "Ethiopia"};
const char *europe_array[4] = {"Germany", "France", "Italy", "United Kingdom"};
const char *north_array[5] = {"USA", "Canada", "Mexixo", "Cuba", "Dominican Republic"};
const char *asia_array[4] = {"China", "India", "Japan", "North Korea"};

const char **temp_array[4] = {africa_array, europe_array, north_array, asia_array};

typedef struct players {
    char *color;
    int troops;
    struct players *next;
    //LL cards 
    //LL Territories 
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

typedef struct games {
    struct continents *map;
    struct players *competitors;
} game;
    


#endif
