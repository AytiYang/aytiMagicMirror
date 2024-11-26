#include "sqlConnect.h"
bool executeQuery(const std::string& server, const std::string& user, const std::string& password, const std::string& database, const std::string& query) {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    conn = mysql_init(nullptr);

    // 连接到数据库
    if (!mysql_real_connect(conn, server.c_str(), user.c_str(), password.c_str(), database.c_str(), 0, nullptr, 0)) {
        std::cerr << "MySQL连接失败: " << mysql_error(conn) << std::endl;
        return false;
    }

    // 执行SQL查询
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "查询失败: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    res = mysql_store_result(conn);

    // 输出查询结果
    while ((row = mysql_fetch_row(res)) != nullptr) {
        std::cout << row[0] << std::endl;
    }

    // 释放结果集
    mysql_free_result(res);

    // 关闭连接
    mysql_close(conn);

    return true;
}


void test() {
    std::string server = "47.115.56.144";
    std::string user = "root";
    std::string password = "yangzihao2002"; // 请替换为你的实际密码
    std::string database = "testdb";
    std::string query = "SELECT * FROM user";

    if (executeQuery(server, user, password, database, query)) {
        std::cout << "查询成功" << std::endl;
    } else {
        std::cerr << "查询失败" << std::endl;
    }
}
