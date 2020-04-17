#include "MySQL-handler.h"

using namespace std;

MySQLhandler::MySQLhandler()
{
    /* Create a connection */
    driver = get_driver_instance();
    con = driver->connect("localhost", "stu", "korona2020");

    /* Connect to the MySQL test database */
    con->setSchema("bakalarka");
}

int MySQLhandler::executeInsert1(double BER, double FIBER, string table)
{
    sql::Statement *stmt = NULL;
    try {
        string v = to_string(BER) + ", " + to_string(FIBER);
        string q = "INSERT INTO " + table + " (BER, FIBER) VALUES (" + v + ")";
        stmt = con->createStatement();
        stmt->execute(q);
        delete stmt;
        return 0;
    }
    catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        return 1;
    }
}

int MySQLhandler::executeInsert2(double SNR, uint32_t BW, string table)
{
    sql::Statement *stmt = NULL;
    try {
        string v = to_string(SNR) + ", " + to_string(BW);
        string q = "INSERT INTO " + table + " (SNR, bandwidth) VALUES (" + v + ")";
        stmt = con->createStatement();
        stmt->execute(q);
        delete stmt;
        return 0;
    }
    catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        return 1;
    }
}

MySQLhandler::~MySQLhandler()
{
    delete con;
}
