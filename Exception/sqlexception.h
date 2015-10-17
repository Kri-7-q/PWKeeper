#ifndef SQLEXCEPTION_H
#define SQLEXCEPTION_H

#include <QString>

class SqlException
{
public:
    SqlException();
    SqlException(const QString &error);
    SqlException(const QString& error, const QString& sqlStatement);
    SqlException(const QString &error, const QString &sqlStatement, const QString& databaseError);

    QString errorText() const                           { return m_errorText; }
    QString sqlStatement() const                        { return m_sqlStatement; }
    void setSqlStatement(const QString &statement)      { m_sqlStatement = statement; }
    void setDatabaseError(const QString &error)         { m_databaseError = error; }
    QString databaseError() const                       { return m_databaseError; }

private:
    QString m_errorText;
    QString m_sqlStatement;
    QString m_databaseError;
};

#endif // SQLEXCEPTION_H
