# MySqlConn -- simplify mysql-connector-c++

This class is a wrapper to simplify mysql-connections and statements

## Third party requirements:

* Boost C++ library
* MySQL Connector C++ library

## Documentation:

### member variables

### constructors

The constructor handles also the connection to your mysql server:
The constructor has maximum three parameter, but only the first param must be used.

1. param: "server-adress"
1. param: "username"
1. param: "password" -- but use this carefully

If the parameter two and three aren't used, the consturctor ask you for username and password.
The password input is hided by a simple lib class in /lib.






