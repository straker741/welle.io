

FIND_PATH(MYSQL_INCLUDE_DIR driver.h
          PATHS /usr/include/cppconn
          )

FIND_LIBRARY(MYSQL_LIBRARIES NAMES mysqlcppconn
             PATHS /usr/lib/arm-linux-gnueabihf
             )

MARK_AS_ADVANCED(MYSQL_LIBRARIES MYSQL_INCLUDE_DIR) 