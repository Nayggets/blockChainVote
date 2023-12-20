#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

int main()
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *sql_create, *sql_insert;
    int rc;
    const char *data = "Ceci est un exemple de BLOB";

    // Ouvrir la base de données
    rc = sqlite3_open("exemple.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur d'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Créer une table
    sql_create = "CREATE TABLE IF NOT EXISTS MaTable (id INTEGER PRIMARY KEY, blobData BLOB);";
    rc = sqlite3_exec(db, sql_create, 0, 0, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur de création de table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Préparer une requête d'insertion
    sql_insert = "INSERT INTO MaTable (blobData) VALUES (?);";
    rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur de préparation de la requête: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Lier le BLOB à la requête
    sqlite3_bind_blob(stmt, 1, data, strlen(data), SQLITE_STATIC);

    // Exécuter la requête
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Erreur d'insertion: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Insertion réussie\n");
    }

    // Nettoyer
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
