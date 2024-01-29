#ifndef MESSAGE_H
#define MESSAGE_H
#include <aio.h>
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

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
    char newId[ENTITY_ID_SIZE];
} UpdateElecteurCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} ReadElecteurCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
    char question[256];
    char dateDebut[256];
    char dateFin[256];
    const char *status[256];
} AjoutElectionCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} SupprimeElectionCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
    char question[256];
} UpdateElectionCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} ReadElectionCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} ProcessVotesCmd;


typedef enum
{
    NOP = 0,
    AJOUT_ELECTEUR,
    SUPPRIME_ELECTEUR,
    EST_PRESENT,
    CAST_VOTE,
    UPDATE_ELECTEUR,
    READ_ELECTEUR,
    AjoutElectionCmd,
    SupprimeElectionCmd,
    UpdateElectionCmd,
    ReadElecteurCmd,
    ProcessVotesCmd
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
        UpdateElecteurCmd updateElecteur;
        ReadElecteurCmd readElecteur;
        AjoutElectionCmd ajoutElection;
        SupprimeElectionCmd supprimeElection;
        UpdateElectionCmd updateElection;
        ReadElectionCmd readElection;
        ProcessElectionCmd processVotes;
    } commande;
} Commande;

#endif