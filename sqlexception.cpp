#include "sqlexception.h"

SqlException::SqlException()
{

}

SqlException::SqlException(const QString &error) :
    m_errorText(error)
{

}

