#include "sqlexception.h"

SqlException::SqlException()
{

}

SqlException::SqlException(const QString &error) :
    m_errorText(error)
{

}

SqlException::SqlException(const QString &error, const QString &sqlStatement) :
    m_errorText(error),
    m_sqlStatement(sqlStatement)
{

}

SqlException::SqlException(const QString &error, const QString &sqlStatement, const QString &databaseError) :
    m_errorText(error),
    m_sqlStatement(sqlStatement),
    m_databaseError(databaseError)
{

}

