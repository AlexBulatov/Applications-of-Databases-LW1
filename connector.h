#pragma once
#include <windows.h>
#include <odbcinst.h>
#include <sqlext.h>
#include <string>
#include <iostream>
#include <ctime>
enum class FETCH_RESULT {
    SUCCESS,
    FAIL,
    END
};

class Query {
public:
    Query(SQLHSTMT stmt, const char *query)
        : stmt(stmt), query(query) {}

    ~Query() {
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    }

    bool exec() {
        auto retcode = SQLExecDirectA(stmt, (SQLCHAR *) query, SQL_NTS);
        return (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO);
    }

    FETCH_RESULT fetchOne() {
        int retcode = SQLFetch(stmt);
        if (retcode == SQL_ERROR) {
            return FETCH_RESULT::FAIL;
        }
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            return FETCH_RESULT::SUCCESS;
        }
        return FETCH_RESULT::END;
    }

    SQLHSTMT getHandler() {
        return stmt;
    }

private:
    SQLHSTMT stmt;
    const char *query;
};

class Connector {
public:
    Connector() : Connector("mypg", "postgres", "1") {}

    Connector(const char* DSN, const char* database, const char* password) {
        retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
        retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
        retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
        retcode = SQLConnectA(hdbc, (SQLCHAR*) DSN, SQL_NTS, (SQLCHAR*) database, SQL_NTS,  (SQLCHAR*) password, SQL_NTS);
    }

    ~Connector() {
        retcode = SQLDisconnect(hdbc);
        retcode = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
        retcode = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        retcode = SQLFreeHandle(SQL_HANDLE_ENV, henv);
    }

    void executeQuery(char *query) {
        retcode = SQLExecDirect(hstmt, (SQLWCHAR*)query, SQL_NTS);
        retcode = SQLFetch(hstmt);
    }

    char* readChar(int col) {
        retcode = SQLGetData(hstmt, col, SQL_C_CHAR, buf, buf_size, &buf_len);
        return (char*)buf;
    }

    char readSymbol(int col){
        retcode = SQLGetData(hstmt, col, SQL_C_CHAR, symbol, 2, &buf_len);
        return (char)symbol[0];
    }

    int readInt(int col) {
        retcode = SQLGetData(hstmt, col, SQL_INTEGER, &bufInt, 0, &buf_len);
        return (int)bufInt;
    }

    TIMESTAMP_STRUCT readTimestamp(int col) {
        retcode = SQLGetData(hstmt, col, SQL_TIMESTAMP, &dateTime, 0, &buf_len);
        return dateTime;
    }

    Query createQuery(const char *queryText) {
        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
        return Query(hstmt, queryText);
    }

private:
    SQLHENV     henv;
    SQLHDBC     hdbc;
    SQLHSTMT    hstmt;
    SQLRETURN   retcode;
    SQLCHAR     buf[50];
    SQLCHAR     symbol[2];
    SQLINTEGER buf_len = 50;
    SQLINTEGER buf_size = 50;
    SQLINTEGER bufInt;
    TIMESTAMP_STRUCT dateTime;
};

