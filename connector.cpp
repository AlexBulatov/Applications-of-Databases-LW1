#include "connector.h"

Connector::Connector(){
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    retcode = SQLConnectA(hdbc, (SQLCHAR*) "mypg", SQL_NTS, (SQLCHAR*) "rpbd", SQL_NTS,  (SQLCHAR*) "1", SQL_NTS);
}

Connector::Connector(char* DSN, char* database, char* password) {
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    retcode = SQLConnectA(hdbc, (SQLCHAR*) DSN, SQL_NTS, (SQLCHAR*) database, SQL_NTS,  (SQLCHAR*) password, SQL_NTS);
}

void Connector::executeQuery(char *query) {
    retcode = SQLExecDirect(hstmt, (SQLWCHAR*)query, SQL_NTS);
    retcode = SQLFetch(hstmt);
    SQLGetData(hstmt, 1, SQL_C_CHAR, buf, 50, &buf_len);
}

Connector::~Connector() {
    retcode = SQLDisconnect(hdbc);
    retcode = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    retcode = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    retcode = SQLFreeHandle(SQL_HANDLE_ENV, henv);
}
