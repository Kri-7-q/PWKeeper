#include "kqpostgresqlresult.h"
#include <QDateTime>
#include <QSqlField>
#include <QSqlRecord>
#include <QString>

Q_DECLARE_OPAQUE_POINTER(PGresult*)
Q_DECLARE_METATYPE(PGresult*)


/**
 * Constructor
 * Takes a pointer to a KQPostgreSqlDriver object.
 * @param driver    Pointer to the database driver.
 */
KQPostgreSqlResult::KQPostgreSqlResult(const KQPostgreSqlDriver *driver) :
    QSqlResult(driver),
    m_result(NULL),
    m_currentSize(-1)
{
    
}

/**
 * Destructor
 * Free used memory.
 */
KQPostgreSqlResult::~KQPostgreSqlResult()
{
    if (m_result) {
        PQclear(m_result);
    }
}

/**
 * A handle wrapped in a QVariant to the PGresult object.
 * @return
 */
QVariant KQPostgreSqlResult::handle() const
{
    return QVariant::fromValue(m_result);
}

/**
 * Override
 * Get data of a field as QVariant.
 * @param i     Field number.
 * @return      The value as QVariant or QVariant().
 */
QVariant KQPostgreSqlResult::data(int i)
{
    if (m_result == NULL || i > PQnfields(m_result)) {
        qWarning("Field number is out of range. Or has no result.");
        return QVariant();
    }
    Oid typeOid = PQftype(m_result, i);
    QVariant::Type dataType = variantTypeFromPostgreType(typeOid);
    if (PQgetisnull(m_result, at(), i)) {
        return QVariant(dataType);
    }
    const char* value = PQgetvalue(m_result, at(), i);
    switch (dataType) {
    case QVariant::Bool:
        return QVariant((bool)(value[0] == 't'));
        break;
    case QVariant::String:
        return QVariant(QString(value));
        break;
    case QVariant::LongLong:
        return QVariant(QString(value).toLongLong());
        break;
    case QVariant::Int:
        return QVariant(QString(value).toInt());
        break;
    case QVariant::Double:
        if (typeOid == 1700) {
            if (numericalPrecisionPolicy() != QSql::HighPrecision) {
                QVariant retval;
                bool convert;
                double dbl=QString(value).toDouble(&convert);
                if (numericalPrecisionPolicy() == QSql::LowPrecisionInt64)
                    retval = (qlonglong)dbl;
                else if (numericalPrecisionPolicy() == QSql::LowPrecisionInt32)
                    retval = (int)dbl;
                else if (numericalPrecisionPolicy() == QSql::LowPrecisionDouble)
                    retval = dbl;
                if (!convert)
                    return QVariant();
                return retval;
            }
            return QVariant(QString(value));
        }
        return QVariant(QString(value).toDouble());
        break;
    case QVariant::Date:
        return QVariant(QDate::fromString(QString(value), Qt::ISODate));
        break;
    case QVariant::Time:
        return QVariant(QTime::fromString(QString(value), Qt::ISODate));
        break;
    case QVariant::DateTime:
        return QVariant(QDateTime::fromString(QString(value), Qt::ISODate));
        break;
    case QVariant::ByteArray: {
        size_t length = 0;
        unsigned char* buffer = PQunescapeBytea((unsigned char*)value, &length);
        QByteArray array((char*)buffer, length);
        PQfreemem(buffer);
        return QVariant(array);
        break;
    }
    default:
        qWarning("Unknown data type !");
        break;
    }

    return QVariant();
}

/**
 * Override
 * Lookup if a value in result is NULL.
 * @param i     The field index of result value.
 * @return      True if value is NULL.
 */
bool KQPostgreSqlResult::isNull(int i)
{
    return PQgetisnull(m_result, at(), i);
}

/**
 * Override
 * Reset result to a given prepared statement.
 * @param sqlquery      The name of prepared statement.
 * @return              True if result is present.
 */
bool KQPostgreSqlResult::reset(const QString &sqlquery)
{
    clearResult();
    if (!driver()) {
        return false;
    }
    if (!driver()->isOpen() || driver()->isOpenError()) {
        return false;
    }
    m_result = postgreDriver()->executePrepared(sqlquery);
    ExecStatusType status = PQresultStatus(m_result);
    if (status == PGRES_COMMAND_OK) {
        setActive(true);
        setSelect(false);
        return true;
    }
    if (status == PGRES_TUPLES_OK) {
        setActive(true);
        setSelect(true);
        setAt(QSql::BeforeFirstRow);
        return true;
    }

    return false;
}

/**
 * Override
 * Sets the result to the given row.
 * @param i     The number of where result should be set to.
 * @return      True if done.
 */
bool KQPostgreSqlResult::fetch(int i)
{
    if (! isActive()) {
        return false;
    }
    if (i < 0 || i >= PQntuples(m_result)) {
        return false;
    }
    if (i == at()) {
        return false;
    }
    setAt(i);

    return true;
}

/**
 * Override
 * Sets the result to the first row.
 * @return  True if done.
 */
bool KQPostgreSqlResult::fetchFirst()
{
    return fetch(0);
}

/**
 * Override
 * Sets the result to the last row.
 * @return  True if done.
 */
bool KQPostgreSqlResult::fetchLast()
{
    return fetch(PQntuples(m_result) - 1);
}

/**
 * Override
 * Get the number of rows which was affected by the las statement.
 * @return          The number of affected rows.
 */
int KQPostgreSqlResult::numRowsAffected()
{
    char* value = PQcmdTuples(m_result);
    QString numRowsString(value);
    bool OK = false;
    int numRows = numRowsString.toInt(&OK);
    if (!OK) {
        qWarning("Could not convert char* to int.");
    }

    return numRows;
}

/**
 * Override
 * Get a info record about the result.
 * @return      A record with result information.
 */
QSqlRecord KQPostgreSqlResult::record() const
{
    QSqlRecord info;
    if (!isActive() || !isSelect()) {
        return info;
    }
    int columns = PQnfields(m_result);
    for (int index=0; index<columns; ++index) {
        char* value = PQfname(m_result, index);
        QString name(value);
        int length = PQfsize(m_result, index);
        int precision = PQfmod(m_result, index);
        Oid postgreType = PQftype(m_result, index);
        QVariant::Type dataType = variantTypeFromPostgreType(postgreType);
        QSqlField field(name, dataType);
        field.setLength(length);
        field.setPrecision(precision);

        info.append(field);
    }

    return info;
}

/**
 * Override
 * Prepare a query for execution.
 * Let the DBMS parse the query with place holders and
 * execute them after values are set.
 * @param query     The SQL query string to prepare.
 * @return          True if successfully prepared.
 */
bool KQPostgreSqlResult::prepare(const QString &query)
{
    QString stmt = replacePlaceholders(query);
    PGresult* result = postgreDriver()->prepareStatement(stmt, boundValueCount());

    return PQresultStatus(result) == PGRES_COMMAND_OK;
}

/**
 * Execute a previously prepared statement.
 * @return      True if done.
 */
bool KQPostgreSqlResult::exec()
{
    QVector<QVariant> paramVector = boundValues();
    // Allocate memory.
    char** values = cstringArrayOfSize(paramVector.size());
    int* valueLength = new int[paramVector.size()];
    for (int index=0; index<paramVector.size(); ++index) {
        QString value = variantToString(paramVector.value(index));
        values[index] = stringCopy(value);
        valueLength[index] = value.length();
    }
    clearResult();
    m_result = postgreDriver()->executePrepared(paramVector.size(), values, valueLength);
    // Free allocated memory.
    freeCStringArray(values, paramVector.size());
    delete valueLength;
    ExecStatusType status = PQresultStatus(m_result);
    if (status == PGRES_TUPLES_OK) {
        setActive(true);
        setSelect(true);
        m_currentSize = PQntuples(m_result);
        return true;
    }
    if (status == PGRES_COMMAND_OK) {
        setActive(true);
        setSelect(false);
        m_currentSize = -1;
        return true;
    }
    QSqlError error(QString("Could not execute query !"), QString(PQresultErrorMessage(m_result)), QSqlError::StatementError,
                    QString(PQresStatus(status)));
    setLastError(error);
    clearResult();

    return false;
}

/**
 * Get the size of result.
 * @return      The number of rows in result.
 */
int KQPostgreSqlResult::size()
{
    return m_currentSize;
}

/**
 * Private
 * Get a QVariant type from a PostgreSql data type.
 * @param type      A PostgreSql Oid with data type information.
 * @return          A QVariant::Type similar to the Postgre type.
 */
QVariant::Type KQPostgreSqlResult::variantTypeFromPostgreType(const Oid type)
{
    QVariant::Type variantType = QVariant::Invalid;
    switch (type) {
    case 16:        // bool
        variantType = QVariant::Bool;
        break;
    case 20:        // int8
        variantType = QVariant::LongLong;
        break;
    case 21:        // int2
    case 23:        // int4
    case 2278:      // oid
    case 24:        // regproc
    case 28:        // xid
    case 29:        // cid
        variantType = QVariant::Int;
        break;
    case 1700:      // numeric
    case 700:       // float4
    case 701:       // float8
        variantType = QVariant::Double;
        break;
    case 702:       // abstime
    case 703:       // reltime
    case 1082:      // date
        variantType = QVariant::Date;
        break;
    case 1083:      // time
    case 1266:      // timetz
        variantType = QVariant::Time;
        break;
    case 1114:      // timestamp
    case 1184:      // timestamptz
        variantType = QVariant::DateTime;
        break;
    case 17:        // bytea
        variantType = QVariant::ByteArray;
        break;
    default:
        variantType = QVariant::String;
        break;
    }
    
    return variantType;
}

/**
 * Protected
 * Delete result from memory.
 */
void KQPostgreSqlResult::clearResult()
{
    if (m_result) {
        PQclear(m_result);
        m_result = NULL;
        setAt(QSql::BeforeFirstRow);
        setActive(false);
        m_currentSize = -1;
    }
}

/**
 * Protected
 * Get the Postgre driver pointer.
 * @return      A pointer to the Postgre driver.
 */
const KQPostgreSqlDriver *KQPostgreSqlResult::postgreDriver() const
{
    return reinterpret_cast<const KQPostgreSqlDriver*>(driver());
}

/**
 * Searches for placeholders and replace them with PostgreSql placeholders.
 * Relaced are either '?' and ':identifier'. They are searched by regular
 * expressions. Found placeholders are replaced with: '$1','$2','$3'...
 * @param sqlStatement      A statement with placeholders.
 * @return sqlStatement     With PostgreSql placeholders.
 */
QString KQPostgreSqlResult::replacePlaceholders(QString sqlStatement) const
{
    int numPlaceholder = 1, position = 0;
    QChar currentPlaceholder[2] = { QChar('$'), QChar('0') };
    while (true) {
        currentPlaceholder[1] = QChar('0' + numPlaceholder);
        position = sqlStatement.indexOf(QChar('?'), position);
        if (position < 0) {
            break;
        }
        sqlStatement.replace(position, 1, currentPlaceholder, 2);
        ++numPlaceholder;
    }

    return sqlStatement;
}

/**
 * Creates an array of char pointer with given size.
 * Array content is set to NULL.
 * @param size
 * @return
 */
char **KQPostgreSqlResult::cstringArrayOfSize(const int size) const
{
    char** array = new char*[size];
    for (int index=0; index<size; ++index) {
        array[index] = NULL;
    }

    return array;
}

/**
 * Private
 * Free memory of an allocated char pointer array.
 * Delete the arrays content and the array it self.
 * @param array     The array to delete.
 * @param size      The size of the array.
 */
void KQPostgreSqlResult::freeCStringArray(char **array, const int size)
{
    for (int index=0; index<size; ++index) {
        if (array[index] != NULL) {
            delete array[index];
        }
    }
    delete array;
}

/**
 * Copy QString to cstring.
 * Creates a string in heap and return a pointer to it.
 * @param origin
 * @return string       A null terminated cstring pointer.
 */
char *KQPostgreSqlResult::stringCopy(const QString &origin) const
{
    QByteArray source = origin.toLocal8Bit();
    int length = source.length();
    char* string = new char[length + 1];
    for (int index=0; index<source.size(); ++index) {
        string[index] = source.at(index);
    }
    string[length] = 0;

    return string;
}

/**
 * Converts a QVariant value to a QString which can be inserted in
 * the parameter array of a prepared statment.
 * @param value     The value which is to convert.
 * @return string   The converted value. Or empty string.
 */
QString KQPostgreSqlResult::variantToString(const QVariant &value) const
{
    QVariant::Type dataType = value.type();
    switch (dataType) {
    case QVariant::Bool:
        if (value.toBool()) {
            return QString("t");
        } else {
            return QString("f");
        }
        break;
    case QVariant::LongLong:
    case QVariant::Int:
    case QVariant::Double:
    case QVariant::String:
        return value.toString();
        break;
    case QVariant::Date:
        return value.toDate().toString(Qt::ISODate);
        break;
    case QVariant::Time:
        return value.toTime().toString(Qt::ISODate);
        break;
    case QVariant::DateTime:
        return value.toDateTime().toString(Qt::ISODate);
        break;
    case QVariant::ByteArray: {
        return QString(value.toByteArray());
        break;
    }
    default:
        qWarning("Unknown data type !");
        break;
    }

    return QString();
}
