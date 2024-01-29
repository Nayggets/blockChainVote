#include "../common/include/handler.h"

//renvoie 0 si l'ID n'éxiste pas sinon renvoie 1
int handlerestpresent(sqlite3 *db,Commande* cmd){

    if(getIdFromNumeroID(db,cmd->commande.estPresent.identifiant,  strlen(cmd->commande.estPresent.identifiant)+1 )== -1){
        return -1;
    }
    else{
        printf("Electeur creer");

        return 0;
    }


}