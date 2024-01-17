#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "../src/handler/handler_ajoutelecteur.c"

int main(int argc, char *argv[]) {
    
    sqlite3 *db;
   
    char *zErrMsg = 0;
    int rc;
    char *sql;
    rc = sqlite3_open("test.db", &db);
    //delete table Electeur
    sql = "DROP TABLE IF EXISTS Electeur;";
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    //create table Electeur
    
    sql = "CREATE TABLE Electeur("  \
        "numeroID TEXT PRIMARY KEY NOT NULL);";

    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

   
    Commande *cmd = malloc(sizeof(Commande));
    cmd->type = AJOUT_ELECTEUR;
 
    char *id = "id";
    strcpy(cmd->commande.ajoutElecteur.identifiant, id);
    handlerajoutelecteur(db, cmd);
    //if good , print "Electeur added"
    if (electeurExists(db, id, strlen(id) + 1) == 0) {
        printf("Electeur added\n");
    }

    char *id2 = "id2";
    strcpy(cmd->commande.ajoutElecteur.identifiant, id2);
    handlerajoutelecteur(db, cmd);
    

    //printf database
    sql = "SELECT * from Electeur";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    int ctotal = sqlite3_column_count(stmt);
    int res = 0;
    
    while (1) {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW) {
            for (int i = 0; i < ctotal; i++) {
                printf("%s = %s\n", sqlite3_column_name(stmt, i), sqlite3_column_text(stmt, i));
            }
        }
        if (res == SQLITE_DONE) {
            printf("done\n");
            break;
        }
        if (res != SQLITE_ROW && res != SQLITE_DONE) {
            printf("error\n");
            break;
        }
    }
    sqlite3_finalize(stmt);

    free(cmd);

  
    sqlite3_close(db);
    return 0;
}