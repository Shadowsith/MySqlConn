connmysql:
	g++ MySqlConn.cpp ./lib/hide_input.cpp -lmysqlcppconn -o connmysql

make clean:
	rm -f connmysql  
