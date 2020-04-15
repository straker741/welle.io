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

int MySQLhandler::executeInsert(double value, string table)
{
    try {
        string v = to_string(value);
        string q = "INSERT INTO " + table + " (value) VALUES (" + v + ")";
        stmt = con->createStatement();
        stmt->execute(q);
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
    delete res;
    delete stmt;
    delete con;
}
