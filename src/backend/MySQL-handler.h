/**
 * https://dev.mysql.com/
 * Implemented:
 * jakub.svajka98@gmail.com
 */

#ifndef MYSQL_HANDLER_H
#define MYSQL_HANDLER_H

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <string>

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


class MySQLhandler
{
    public:
        MySQLhandler();
        ~MySQLhandler();
        int     executeInsert(double value, std::string table);
    private:
        sql::Driver *driver = NULL;
        sql::Connection *con = NULL;
};

#endif
