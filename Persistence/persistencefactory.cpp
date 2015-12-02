#include "persistencefactory.h"
#include "Persistence/postgresql.h"


/**
 * Creates a Persistence object.
 * @param object
 * @return
 */
Persistence *PersistenceFactory::createPersistenceObj()
{
    return new PostgreSql();
}
