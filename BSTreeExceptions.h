#include <iostream>
#include <exception>

using namespace std;

class BSTreeDuplicateException: public exception
{
    virtual const char* what() const throw(){
	return "BSTreeDuplicateException thrown";
    }

};
