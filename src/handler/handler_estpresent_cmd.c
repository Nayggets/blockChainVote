#include "../../common/include/bd.h"
#include "../../common/include/messages.h"


//renvoie 0 si l'ID n'éxiste pas sinon renvoie 1
int estpresent(Commande* cmd,sqlite3 *db){

    if(getIdFromNumeroID(db,cmd->commande.estPresent.identifiant,  strlen(cmd->commande.estPresent.identifiant)+1 )== -1){
        return 0;
    }
    else{
        return 1;
    }


}