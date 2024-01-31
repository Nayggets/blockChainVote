#ifndef MESSAGE_H
#define MESSAGE_H
#include <aio.h>
#include "protocol.h"
#include <openssl/ssl.h>
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
    char nouveauId[ENTITY_ID_SIZE];
} MiseAJourElecteurCmd;

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
} MiseAJourElectionCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} ReadElectionCmd;

typedef struct
{
    char identifiantElection[ENTITY_ID_SIZE];
} ProcederAuxResultatsVotesCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} MiseAJourStatusCmd;

typedef enum
{
    NOP = 0,
    AJOUT_ELECTEUR,
    SUPPRIME_ELECTEUR,
    EST_PRESENT,
    CAST_VOTE,
    MISE_A_JOUR_ELECTEUR,
    READ_ELECTEUR,
    AJOUT_ELECTION,
    SUPPRIME_ELECTION,
    MISE_A_JOUR_ELECTION,
    READ_ELECTION,
    PROCEDER_AUX_RESULTATS_VOTE,
    MISE_A_JOUR_STATUS
} CommandeType;


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
    CommandeType type;
    char signature[256]; // la signature de la commande
    union
    {
        AjoutElecteurCmd ajoutElecteur;
        SupprimeElecteurCmd supprimeElecteur;
        EstPresentCmd estPresent;
        CastVoteCmd castVote;
        MiseAJourElecteurCmd updateElecteur;
        ReadElecteurCmd readElecteur;
        AjoutElectionCmd ajoutElection;
        SupprimeElectionCmd supprimeElection;
        MiseAJourElectionCmd updateElection;
        ReadElectionCmd readElection;
        ProcederAuxResultatsVotesCmd procederAuxResultatsVotes;
        MiseAJourStatusCmd miseAJourStatus;
    } commande;
} Commande;


typedef struct ClientFd
{
    int fd;
    SSL* ssl;
} ClientFd;

typedef struct 
{
    CodeErreur codeErreur;
    CommandeType command;
    char message[256];
}Paquets;

typedef struct 
{
    Commande* commande;
    ClientFd* client;
} TacheCommande;
#endif