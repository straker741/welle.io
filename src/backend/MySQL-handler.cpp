#include "MySQL-handler.h"

using namespace std;

MySQLhandler::MySQLhandler()
{
    /* Create a connection */
    driver = get_driver_instance();
    con = driver->connect("localhost", "root", "");

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

/**
 * SId      |   Slabel   |   ASCTy   |   bitrate
 * 0x5208   Radio Patria     DAB+        112
 * 0x5002   Radio Junior     DAB+        136
 * 0x5202   Radio_FM         DAB+        128
 * 0x521b   Radio Devin      DAB+        96
 * 0x5347   Radio Jemne      DAB+        104
 * 0x5346   ANTENA ROCK      DAB+        104
 * 0x5348   Radio VLNA       DAB+        104
 * 0x5005   Radio Litera     DAB+        96
 * 0x5343   Radio Lumen      DAB+        104
 * 0x5201   Radio Slovensko  DAB+        136
 */

/**
 * key  |   EId  |  Elabel      |   datetime    
 * 1        5001    TOWERCOM        YYYY-MM-DD hh:mm:ss
 * 2        5001    TOWERCOM    
 */