#ifndef KQPOSTGRESQLRESULT_H
#define KQPOSTGRESQLRESULT_H

#include "kqpostgresqldriver.h"
#include <QSqlResult>


class KQPostgreSqlResult : public QSqlResult
{
    friend class KQPostgreSqlDriver;

public:
    explicit KQPostgreSqlResult(const KQPostgreSqlDriver* driver);
    ~KQPostgreSqlResult();

    QVariant handle() const override;
    
protected:
    // QSqlResult interface
    QVariant data(int i) override;
    bool isNull(int i) override;
    bool reset(const QString &sqlquery) override;
    bool fetch(int i) override;
    bool fetchFirst() override;
    bool fetchLast() override;
    int numRowsAffected() override;
    QSqlRecord record() const override;
    bool prepare(const QString &query) override;
    bool exec() override;
    int size() override;

private:
    // Concret class members
    static QVariant::Type variantTypeFromPostgreType(const Oid type);
    void clearResult();
    inline const KQPostgreSqlDriver* postgreDriver() const;
    QString replacePlaceholders(QString sqlStatement) const;
    char** cstringArrayOfSize(const int size) const;
    void freeCStringArray(char** array, const int size);
    char* stringCopy(const QString& origin) const;
    QString variantToString(const QVariant& value) const;
    
private:
    PGresult* m_result;
    int m_currentSize;
};

#endif // KQPOSTGRESQLRESULT_H
