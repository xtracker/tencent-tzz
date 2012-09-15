#include <iostream>

#include <mysql/mysql.h>

#include <string.h>

using namespace std;

int main()
{
	MYSQL mysql;
	mysql_init(&mysql);
	MYSQL *i = mysql_real_connect(&mysql, "localhost", "root", "", "tencent", 0, NULL, 0);
	if (i == NULL)
		cout<<	mysql_error(&mysql);
	return 0;

}
