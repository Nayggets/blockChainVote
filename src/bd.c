#include "../common/include/bd.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

const char *electeur_create = "CREATE TABLE IF NOT EXISTS Electeur(id INTEGER PRIMARY KEY, numeroID BLOB);";

const char *election_create = "CREATE TABLE IF NOT EXISTS Election(\
    id INTEGER PRIMARY KEY, \
    identifiant BLOB ,\
    question TEXT CHECK(length(question) <= 256),\
    dateDebut TEXT, \
    dateFin TEXT, \
    status TEXT CHECK(status IN('canceled', 'active', 'closed')));\
                              ";

const char *vote_create = "CREATE TABLE IF NOT EXISTS Vote(id INTEGER PRIMARY KEY,\
    idVotant INTEGER,\
    idElection INTEGER,\
    timestamp TEXT,\
    ballot BLOB ,\
    hashValidation TEXT CHECK(length(hashValidation) <= 256) );";

int database_exists(const char *path)
{
    struct stat buffer;
    return (stat(path, &buffer));
}

sqlite3 *database_open(const char *path)
{
    sqlite3 *db = NULL;
    int rc = sqlite3_open(path, &db);
    if (rc != SQLITE_OK)
    {
        // Gérer l'erreur
        sqlite3_close(db);
        return NULL;
    }
    return db;
};

int database_close(sqlite3 *db)
{
    return (sqlite3_close(db) == SQLITE_OK) ? 0 : 1;
};

int database_init(sqlite3 *db)
{
    printf("database_init\n");

    const char *requests[3] = {electeur_create, election_create, vote_create};
    int i;
    for (i = 0; i < 3; i++) // TODO
    {
        const char *sql = requests[i];
        char *errMsg = 0;
        int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
        if (rc != SQLITE_OK)
        {
            // Gérer l'erreur
            if (errMsg)
            {
                sqlite3_free(errMsg);
            }
            sqlite3_close(db);
            return 1;
        }
    }
    return 0;
};

void createElecteur(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Electeur (numeroID) VALUES (?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        const char *data = numeroID;
        // int size = strlen(data) + 1; // +1 pour le caractère nul de fin

        sqlite3_bind_blob(stmt, 1, data, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            printf("Erreur lors de l'insertion: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("Electeur ajouté avec succès\n");
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

void readElecteur(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM Electeur WHERE numeroID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            // Supposons que numeroID est la première colonne
            const char *id = sqlite3_column_blob(stmt, 0);
            printf("Electeur: %s\n", id);
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

int electeurExists(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM Electeur WHERE numeroID = ?;";
    int result = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result = sqlite3_column_int(stmt, 0) > 0;
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }

    return result;
}

void updateElecteur(sqlite3 *db, const char *oldNumeroID, int size1, const char *newNumeroID, int size2)
{
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE Electeur SET numeroID = ? WHERE numeroID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, newNumeroID, size1, SQLITE_STATIC);
        sqlite3_bind_blob(stmt, 2, oldNumeroID, size2, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            printf("Erreur lors de la mise à jour: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("Electeur mis à jour avec succès\n");
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

void deleteElecteur(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM Electeur WHERE numeroID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            printf("Erreur lors de la suppression: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("Electeur supprimé avec succès\n");
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

int getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM Electeur WHERE numeroID = ?;";
    int id = -1; // Valeur par défaut si l'ID n'est pas trouvé

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            id = sqlite3_column_int(stmt, 0); // Récupère l'ID de la première colonne
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }

    return id;
}

void createElection(sqlite3 *db, const char *identifiant, int sizeId, const char *question, const char *dateDebut, const char *dateFin, const char *status)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Election (identifiant, question, dateDebut, dateFin, status) VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, identifiant, sizeId, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, question, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, dateDebut, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, dateFin, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, status, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de l'insertion: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

int Election_getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM Election WHERE identifiant = ?;";
    int id = -1; // Valeur par défaut si l'ID n'est pas trouvé

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            id = sqlite3_column_int(stmt, 0); // Récupère l'ID de la première colonne
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }

    return id;
}

// TODO
void readElection(sqlite3 *db, int id)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM Election WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            // Traiter les résultats ici
            // Exemple : printf("%s\n", sqlite3_column_text(stmt, 2)); // Pour la colonne 'question'
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

void updateElection(sqlite3 *db, int id, const char *question)
{
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE Election SET question = ? WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, question, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de la mise à jour: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

void deleteElection(sqlite3 *db, int id)
{
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM Election WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de la suppression: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

// usecases election

void Election_castVote(sqlite3 *db, int idVotant, int idElection, const void *ballot, int ballotSize, const char *hashValidation)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Vote (idVotant, idElection, timestamp, ballot, hashValidation) VALUES (?, ?, datetime('now'), ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, idVotant);
        sqlite3_bind_int(stmt, 2, idElection);
        sqlite3_bind_blob(stmt, 3, ballot, ballotSize, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, hashValidation, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de l'insertion: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("Vote ajouté avec succès\n");
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

//
void Election_processVotes(sqlite3 *db, int electionId, int *p_option0, int *p_option1, int *p_totalvotes)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM Vote WHERE idElection = ?;";
    *p_totalvotes = 0;
    *p_option0 = 0;
    *p_option1 = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, electionId);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            // Récupérer les données de chaque vote
            int voteId = sqlite3_column_int(stmt, 0); // id
            // Autres colonnes peuvent être récupérées ici
            const void *ballotBlob = sqlite3_column_blob(stmt, 4);
            int blobSize = sqlite3_column_bytes(stmt, 4);
            // Traiter les données du vote
            // printf("Traitement du vote ID: %d\n", voteId);
            // Ajoutez ici le code pour traiter chaque vote
            *p_totalvotes = *p_totalvotes + 1;
            int v = *((char *)ballotBlob);
            if (v)
            {
                *p_option1 = *p_option1 + 1;
            }
            else
            {
                *p_option0 = *p_option0 + 1;
            }
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}
