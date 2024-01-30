#ifndef MESSAGE_H
#define MESSAGE_H
#include <aio.h>
#include "protocol.h"
#include <gmp.h>

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
    int ballot;
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
    char status[256];
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
    char identifiantElection[ENTITY_ID_SIZE];
    char option0[256];
    char option1[256];
    int* totalVotes;
    mpz_t g;
    mpz_t lambda;
    mpz_t mu;
    mpz_t n;
} ProcessVotesCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} UpdateStatusCmd;

typedef enum
{
    NOP = 0,
    AJOUT_ELECTEUR,
    SUPPRIME_ELECTEUR,
    EST_PRESENT,
    CAST_VOTE,
    UPDATE_ELECTEUR,
    READ_ELECTEUR,
    AJOUT_ELECTION,
    SUPPRIME_ELECTION,
    UPDATE_ELECTION,
    READ_ELECTION,
    PROCESS_VOTES,
    UPDATE_STATUS
} CommandType;


typedef enum
{
   REUSSITE = 0,
   ELECTEUR_PAS_PRESENT = 1,
   ElECTION_EXISTE_DEJA = 2,
   ElECTION_PAS_PRESENTE = 3,
} CodeErreur;

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
        ProcessVotesCmd processVotes;
        UpdateStatusCmd updateStatus;
    } commande;
} Commande;

#endif