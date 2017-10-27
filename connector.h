#include <windows.h>
#include <odbcinst.h>
#include <sqlext.h>

class Connector{
public:
    void executeQuery(char *query);
    void executeScalar(char *query);
    ~Connector();
private:
    SQLHENV     henv;     	// Дескриптор окружения
    SQLHDBC     hdbc; 		// Дескриптор соединения
    SQLHSTMT    hstmt; 	// Дескриптор оператора
    SQLRETURN   retcode; 	// Код возврата
    SQLCHAR     buf[50];	//
    SQLINTEGER buf_len;
    Connector *instance;
    Connector();
    Connector(char* DSN, char* database, char* password);
};

