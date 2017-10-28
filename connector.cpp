#include "connector.h"

Connector::Connector(){
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    retcode = SQLConnectA(hdbc, (SQLCHAR*) "mypg", SQL_NTS, (SQLCHAR*) "rpbd", SQL_NTS,  (SQLCHAR*) "1", SQL_NTS);
    buf_size = 50;
}

Connector::Connector(char* DSN, char* database, char* password) {
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    retcode = SQLConnectA(hdbc, (SQLCHAR*) DSN, SQL_NTS, (SQLCHAR*) database, SQL_NTS,  (SQLCHAR*) password, SQL_NTS);
    buf_size = 50;
}

void Connector::executeQuery(char *query) {
    retcode = SQLExecDirect(hstmt, (SQLWCHAR*)query, SQL_NTS);
    retcode = SQLFetch(hstmt);
}

char* Connector::ReadChar(int col){
    retcode = SQLGetData(hstmt, col, SQL_C_CHAR, buf, buf_size, &buf_len);
    return (char*)buf;
}

int Connector::ReadInt(int col){
    retcode = SQLGetData(hstmt, col, SQL_INTEGER, &bufInt, sizeof(SQL_INTEGER), &buf_len);
}

Connector::~Connector() {
    retcode = SQLDisconnect(hdbc);
    retcode = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    retcode = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    retcode = SQLFreeHandle(SQL_HANDLE_ENV, henv);
}
