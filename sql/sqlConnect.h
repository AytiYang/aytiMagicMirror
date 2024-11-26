#ifndef SQLCONNECT_H
#define SQLCONNECT_H

#endif // SQLCONNECT_H

#include <mariadb/mysql.h>
#include <iostream>
#include <string>

bool executeQuery(const std::string& server, const std::string& user, const std::string& password, const std::string& database, const std::string& query);

void test();
