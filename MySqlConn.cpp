/* MIT License Copyright (c) 2018 Philip Mayer */

#include "MySqlConn.h"
#include "lib/hide_input.h"

MySqlConn::MySqlConn(){
    std::cout << "\nConnecting to Database" << std::endl;
    std::cout << "Enter MySQL server address:";
    std::cin >> this->server;
    std::cout << "Enter username:";
    std::cin >> this->username;
    std::cout << "Enter password ";
    this->password = hcin::hide(); //see hide_input.cpp

    this->driver = sql::mysql::get_mysql_driver_instance();
    this->con = this->driver->connect(this->server,this->username,this->password);
    // not sure if init stmt here could make trouble in some situations
    // if it makes trouble, I must add this two any single method, which it needs
    this->stmt = this->con->createStatement(); 
    this->res = NULL;
}

MySqlConn::MySqlConn(std::string server){
    std::cout << "\nConnecting to Database at: " << server << std::endl;
    this->server = server;
    std::cout << "Enter username:";
    std::cin >> this->username;
    std::cout << "Enter password ";
    this->password = hcin::hide();

    this->driver = sql::mysql::get_mysql_driver_instance();
    this->con = this->driver->connect(this->server,this->username,this->password);
    this->stmt = this->con->createStatement();
    this->res = NULL;
}

MySqlConn::MySqlConn(std::string server, std::string username){
    std::cout << "\nConnecting to Database at: " << server << std::endl;
    this->server = server;
    this->username = username;
    std::cout << "Hello " << username << " enter password ";
    this->password = hcin::hide();

    this->driver = sql::mysql::get_mysql_driver_instance();
    this->con = this->driver->connect(this->server,this->username,this->password);
    this->stmt = this->con->createStatement();
    this->res = NULL;
}

// use this carfully, password could be readed from object dump
MySqlConn::MySqlConn(std::string server, std::string username, std::string password){
    this->server = server;
    this->username = username;
    this->password = password;

    this->driver = sql::mysql::get_mysql_driver_instance();
    this->con = this->driver->connect(this->server,this->username,this->password);
    this->stmt = this->con->createStatement();
}

/*
// use this for pointer|dynamic allocated objects
MySqlConn::~MySqlConn(){
    delete this->con;
    delete this->stmt;
    delete this->res;
}*/

void MySqlConn::createSchema(std::string schema){
    this->stmt->execute("CREATE SCHEMA IF NOT EXISTS " + schema);
}

void MySqlConn::dropSchema(std::string schema){
    this->stmt->execute("DROP SCHEMA IF EXISTS " + schema);
}

// only alias name for createSchema
void MySqlConn::createDatabase(std::string database){
    MySqlConn::createSchema(database); 
}

void MySqlConn::dropDatabase(std::string database){
    MySqlConn::dropSchema(database);
}

void MySqlConn::setSchema(std::string schema){
    this->schema = schema;
    /*
    older solution, but it gives a native one 
    this->stmt = this->con->createStatement();
    this->stmt->execute("USE " + this->schema);
    */
    this->con->setSchema(this->schema);
}

void MySqlConn::dropTable(std::string table){
    this->stmt->execute("DROP TABLE IF EXISTS " + table);
}

void MySqlConn::dropRecreateTable(std::string table, std::string columns){
    this->stmt->execute("DROP TABLE IF EXISTS " + table);
    this->stmt->execute("CREATE TABLE " + table + "(" + columns + ")");
}

void MySqlConn::createTable(std::string querry){
    this->stmt->execute(querry);
}

void MySqlConn::createTable(std::string table, std::string columns){
    this->stmt->execute("CREATE TABLE IF NOT EXISTS " + table + "(" + columns + ")");
}

void MySqlConn::insert(std::string querry){
    this->stmt->execute(querry);
}

void MySqlConn::insert(std::string table, std::string columns, std::string values){
    this->stmt->execute("INSERT INTO " + table + "(" + columns + ")" + " VALUES (" + values + ")");
}

void MySqlConn::update(std::string querry){
    this->stmt->executeUpdate(querry);
}

void MySqlConn::update(std::string table, std::string set_, std::string where){
    this->stmt->executeUpdate("UPDATE " + table + " SET " + set_ + " WHERE " + where);
}

void MySqlConn::select(std::string querry){
    //this->stmt = this->con->createStatement();
    this->res = this->stmt->executeQuery(querry);
    this->res_meta = this->res->getMetaData();
    while(this->res->next()){
        for(int i = 1; i <= this->res_meta->getColumnCount(); i++){
            this->selector.push_back(this->res->getString(i));
        }
    }
}

void MySqlConn::select(std::string columns, std::string from){
    //this->stmt = this->con->createStatement();
    this->res = this->stmt->executeQuery("SELECT " + columns + " FROM " + from);
    this->res_meta = this->res->getMetaData();
    while(this->res->next()){
        for(int i = 1; i <= this->res_meta->getColumnCount(); i++){
            this->selector.push_back(this->res->getString(i));
        }
    }
}

void MySqlConn::select(std::string columns, std::string from, std::string where){
    //this->stmt = this->con->createStatement();
    this->res = this->stmt->executeQuery("SELECT " + columns + " FROM " + from + " WHERE " + where);
    this->res_meta = this->res->getMetaData();
    while(this->res->next()){
        for(int i = 1; i <= this->res_meta->getColumnCount(); i++){
            this->selector.push_back(this->res->getString(i));
        }
    }
}

std::vector<std::string> MySqlConn::resSelect(std::string querry){
    std::vector<std::string> vec;
    this->res = this->stmt->executeQuery(querry);
    this->res_meta = this->res->getMetaData();
    while(this->res->next()){
        for(int i = 1; i <= this->res_meta->getColumnCount(); i++){
            vec.push_back(this->res->getString(i));
        }
    }
    return vec;
}


std::vector<std::string> MySqlConn::resSelect(std::string columns, std::string from){
    std::vector<std::string> vec;
    this->res = this->stmt->executeQuery("SELECT " + columns + " FROM " + from);
    this->res_meta = this->res->getMetaData();
    while(this->res->next()){
        for(int i = 1; i <= this->res_meta->getColumnCount(); i++){
            vec.push_back(this->res->getString(i));
        }
    }
    return vec;
}

std::vector<std::string> MySqlConn::resSelect(std::string columns, std::string from, std::string where){
    std::vector<std::string> vec;
    this->res = this->stmt->executeQuery("SELECT " + columns + " FROM " + from + " WHERE " + where);
    this->res_meta = this->res->getMetaData();
    while(this->res->next()){
        for(int i = 1; i <= this->res_meta->getColumnCount(); i++){
            vec.push_back(this->res->getString(i));
        }
    }
    return vec;
}

//use this for non pointer objects
void MySqlConn::deleteConnetor(){
    delete this->con;
    delete this->stmt;
    delete this->res;
}



int main(void){

    MySqlConn connect("localhost:3306","shadowsith");
    connect.setSchema("hello");
    connect.dropTable("neon");
    connect.createTable("neon","id INT AUTO_INCREMENT, state VARCHAR(40), city VARCHAR(40), PRIMARY KEY(id)");
    connect.insert("neon","state, city","'Deutschland', 'Munich'");
    connect.insert("neon","state, city","'Deutschland', 'Berlin'");
    connect.insert("neon","state, city","'Deutschland', 'Hamburg'");
    connect.insert("neon","state, city","'United Kingdom', 'London'");
    connect.insert("neon","state, city","'United Kingdom', 'Bristol'");
    connect.insert("neon","state, city","'USA', 'New York City'");
    connect.insert("neon","state, city","'USA', 'Chicago'");
    connect.insert("neon","state, city","'USA', 'Phoenix'");
    //connect.select("id, state, city","neon","state LIKE 'Deutschland'");
    connect.select("*","neon");
    int i = 0;
    while(!(i >= connect.selector.size())){
        std::cout << "Id: " << connect.selector[i] << "\t" << "State: " << connect.selector[i+1] << "\t" << connect.selector[i+2] << std::endl; 
        i = i+3;
        //if( i >= (connect.selector.size()))
            //break;
    }
    connect.deleteConnetor();
    return 0;
}

