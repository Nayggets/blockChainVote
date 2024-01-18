#ifndef MESSAGE_H
#define MESSAGE_H

#include "protocol.h"

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} AjoutElecteurCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} SupprimeElecteurCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} EstPresentCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
    char idElection[ENTITY_ID_SIZE];
    const void *ballot;
    size_t ballotSize;
} CastVoteCmd;

typedef enum
{
    NOP = 0,
    AJOUT_ELECTEUR,
    SUPPRIME_ELECTEUR,
    EST_PRESENT,
    CAST_VOTE
} CommandType;

//--
typedef struct
{
    CommandType type;
    char signature[256]; // la signature de la commande
    union
    {
        AjoutElecteurCmd ajoutElecteur;
        SupprimeElecteurCmd supprimeElecteur;
        EstPresentCmd estPresent;
        CastVoteCmd castVote;
    } commande;
} Commande;

#endif