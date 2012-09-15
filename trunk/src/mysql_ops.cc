#include <iostream>

#include <mysql/mysql.h>

#include <string.h>

int main()
{
	MYSQL mysql;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "root", "root", "tencent", 3306, NULL, 0);
	return 0;

}
