#ifndef PERSISTENCEFACTORY_H
#define PERSISTENCEFACTORY_H

#include "Persistence/persistence.h"

class PersistenceFactory
{
public:
    static Persistence* createPersistenceObj();
};

#endif // PERSISTENCEFACTORY_H
