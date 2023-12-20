#ifndef PROTOCOL_H
#define PROTOCOL_H

#define ENTITY_ID_SIZE 256

typedef struct
{
    char numeroID[ENTITY_ID_SIZE];
} Electeur;

typedef enum
{
    canceled,
    active,
    closed
} StatusElection;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
    char question[256];
    // La représentation de Date dépend de la façon dont vous souhaitez gérer les dates.
    // Ici, j'utilise une chaîne pour la simplicité, mais vous pouvez choisir une structure plus complexe.
    char dateDebut[256];
    char dateFin[256];
    StatusElection status;
} Election;

typedef struct
{
    char idVotant[ENTITY_ID_SIZE];
    char idElection[ENTITY_ID_SIZE];
    char timestamp[256]; // Ou une structure plus complexe pour les dates/heures
    char bulletin[256];
    char hashValidation[256];
} Vote;

#endif