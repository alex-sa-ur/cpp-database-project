#include <iostream>
#include <stdlib.h>

#include "mysql_driver.h"
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


using namespace std;



int main() {
    
    int choice;
    string empID, empName, ssn, address, phoneNum, salary, unionNum, position,expertise,plane;
    
    sql :: Driver *driver;
    sql :: Connection *connection;
    sql :: Statement *statement;
    sql :: ResultSet *result;
    
    driver = get_driver_instance();
    connection = driver -> connect(/*tcp://address*/,/*name*/,/*password*/);
    connection -> setSchema("final_project");
    
    statement = connection -> createStatement();
    
    cout << "Welcome to the Assignment 3 Database...\n"
         << "---------------------------------------\n"
         << "What would you like to display/do?\n"
         << "\t1.  Insert a new technician into the database\n"
         << "\t2.  Delete an existing airplane from the database\n"
         << "\t3.  Update the expertise of an existing Technician\n"
         << "\t4.  List Technician details for those whose salary is greater than the average\n"
         << "\t5.  List Models that have experts, along with weight and capacity details\n"
         << "\t6.  List total number of Technicians per Model\n"
         << "\t7.  List the details of Tests for a specific plane, ordered by maximum score\n"
         << "\t8.  List most recent Exam dates and Union Number for each Traffic Controller\n"
         << "\t9.  List the number of tests done on a plane, ordered by Technicians\n"
         << "\t10. List information for tests done between SEP05 and DEC05, ordered by Test Number\n";
    
    cin >> choice;
    
    switch (choice){
        case 1:{
            cout << "Please insert information as requested\n";
            cout << "ID: \n";
            cin >> empID;
            cout << "Name: \n";
            cin >> empName;
            cout << "ssn: \n";
            cin >> ssn;
            cout << "address: \n";
            cin >> address;
            cout << "phoneNum: \n";
            cin >> phoneNum;
            cout << "salary: \n";
            cin >> salary;
            cout << "unionNum: \n";
            cin >> unionNum;
            cout << "expertise: \n";
            cin >> expertise;
            
            statement -> execute(
                                 "INSERT INTO `final_project`.`Employee` (`empID`, `empName`, `ssn`, `address`, `phoneNum`, `salary`, `unionNum`, `position`) VALUES ('"+empID+"', '"+empName+"', '"+ssn+"', '"+address+"', '"+phoneNum+"', '"+salary+"', '"+unionNum+"', 'Technician');"
                                 );
            
            statement -> execute(
                                 "INSERT INTO `final_project`.`Technician` (`techID`, `techModNum`) VALUES ('"+empID+"', '"+expertise+"');"
                                 );

            
            cout << "Technician added!...\n";
            result = statement -> executeQuery(
                                               "SELECT empID, empName, ssn, address, phoneNum, salary, unionNum, techModNum FROM `Employee`, `Technician` WHERE empID = techID;"
                                               );
            
            while (result -> next()){
                cout << "| empID = " << result -> getString("empID")
                     << "| empName = " << result -> getString("empName")
                     << "| ssn = " << result -> getString("ssn")
                     << "| address = " << result -> getString("address")
                     << "| phoneNum = " << result -> getString("phoneNum")
                     << "| salary = " << result -> getString("salary")
                     << "| unionNum = " << result -> getString("unionNum")
                     << "| techModNum = " << result -> getString("techModNum") << endl
                     << "------------------------------------------------" << endl;
            }
        }
            break;
        case 2:{
            cout << "Which airplane would you like to delete?\n";
            
            result = statement -> executeQuery(
                                               "SELECT regNum FROM `Airplane`;"
                                               );
            
            while (result -> next()){
                cout << "regNo: " <<result -> getString("regNum") << endl;
            }
            
            cin >> plane;
            
            statement -> execute(
                                 "DELETE FROM `final_project`.`PlaneTest` WHERE `testRegNum`='"+plane+"';"
                                 );
            
            statement -> execute(
                                 "DELETE FROM `final_project`.`Airplane` WHERE `regNum`='"+plane+"';"
                                 );
            cout << "Airplane deleted!...\n";
        }
            break;
        case 3:{
            cout << "Who would you like to update?\n";
            cin >> empID;
            cout << "What would you like to update their expertise to?\n";
            cin >> expertise;
            
            statement -> execute(
                                    "UPDATE `final_project`.`Technician` SET `techModNum`='"+expertise+"' WHERE `techID`='"+empID+"';"
                                );
            
            result = statement -> executeQuery(
                                               "SELECT * FROM `Technician` WHERE "+empID+" = techID;"
                                               );
            
            while (result -> next()){
                cout << "| techID     = " << result -> getString("techID")  << endl
                     << "| techModNum = " << result -> getString("techModNum") << endl;
            }
        }
            break;
        case 4:{
            
            result = statement -> executeQuery(
                                               "SELECT empID, empName, ssn, address, phoneNum, salary, unionNum, techModNum FROM `Employee`, `Technician` WHERE salary > (SELECT AVG(salary) FROM `Employee`) AND empID = techID;"
                                               );
            
            while (result -> next()){
                cout << "| empID = " << result -> getString("empID")
                     << "| empName = " << result -> getString("empName")
                     << "| ssn = " << result -> getString("ssn")
                     << "| address = " << result -> getString("address")
                     << "| phoneNum = " << result -> getString("phoneNum")
                     << "| salary = " << result -> getString("salary")
                     << "| unionNum = " << result -> getString("unionNum")
                     << "| techModNum = " << result -> getString("techModNum") << endl
                     << "------------------------------------------------" << endl;
            }
        }
            break;
        case 5:{
            result = statement -> executeQuery(
                                               "SELECT modelNum, modelCap, modelWeight FROM `Model`, `Technician` WHERE modelNum = techModNum;"
                                               );
            
            while (result -> next()){
                cout << "| modelNum    = " << result -> getString("modelNum")
                     << "| modelCap    = " << result -> getString("modelCap")
                     << "| modelWeight = " << result -> getString("modelWeight") << endl
                     << "------------------------------------------------" << endl;
            }
        }
            break;
        case 6:{
            
            result = statement -> executeQuery(
                                               "SELECT modelNum, COUNT(techID) FROM `Model`, `Technician` WHERE modelNum = techModNum GROUP BY modelNum;"
                                               );
            
            while (result -> next()){
                cout << "| modelNum = " << result -> getString("modelNum")
                     << "| count = " << result -> getString("COUNT(techID)")<< endl
                     << "------------------------------------------------" << endl;
            }
            
        }
            break;
        case 7:{
            cout << "What plane would you like to list tests for?\n";
            cin >> plane;
            
            result = statement -> executeQuery(
                                               "SELECT testRegNum, testNum, testTechID, date, hours, score, testName, maxScore FROM `PlaneTest`, `Test Type` WHERE testNum = testTestNum AND testRegNum = "+plane+" ORDER BY maxScore;"
                                               );
            
            while (result -> next()){
                cout << "| testRegNum = " << result -> getString("testRegNum")
                     << "| testNum = " << result -> getString("testNum")
                     << "| testTechID = " << result -> getString("testTechID")
                     << "| date = " << result -> getString("date")
                     << "| hours = " << result -> getString("hours")
                     << "| score = " << result -> getString("score")
                     << "| testName = " << result -> getString("testName")
                     << "| maxScore = " << result -> getString("maxScore") << endl
                     << "------------------------------------------------" << endl;
            }
        }
            break;
        case 8:{
            result = statement -> executeQuery(
                                               "SELECT examDate, unionNum FROM `Employee`, `TrafficController` WHERE traffID = empID;"
                                               );
            
            while (result -> next()){
                cout << "| examDate = " << result -> getString("examDate")
                     << "| unionNum = " << result -> getString("unionNum")   << endl
                     << "------------------------------------------------"   << endl;
            }
        }
            break;
        case 9:{
            cout << "What plane would you like to get information for?" << endl;
            
            cin >> plane;
            
            
            result = statement -> executeQuery(
                                               " SELECT testTechID, COUNT(*) FROM `PlaneTest` WHERE testRegNum = "+plane+" GROUP BY testTechID;"
                                               );
            
            while (result -> next()){
                cout << "| testTechID = " << result -> getString("testTechID")
                     << "| count = " << result -> getString("COUNT(*)")<< endl
                     << "------------------------------------------------" << endl;
            }
        }
            break;
        case 10:{
            result = statement -> executeQuery(
                                               "SELECT testTestNum, empName, testRegNum FROM `PlaneTest`, `Employee` WHERE date BETWEEN '2005-09-01' AND '2005-12-31' AND empID = testTechID ORDER BY testTestNum;"
                                               );
            
            while (result -> next()){
                cout << "| testTestNum = " << result -> getString("testTestNum")
                     << "| empName = " << result -> getString("empName")
                     << "| testRegNum = " << result -> getString("testRegNum")   << endl
                     << "------------------------------------------------"       << endl;
            }
        }
            break;
        default:
            cout <<"Invalid input, please try again\n";
        
    }
    
    return 0;
}
