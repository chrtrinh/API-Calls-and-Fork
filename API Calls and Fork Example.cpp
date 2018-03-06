/*

	Programmer: Christopher Trinh
	id: 	    z1753883
	date:       3/2/2018
	class:      CSCI 480
	purpose:    Creating a linux program that uses the API for uname, getenv, fork, and system.
              Also uses ostringstream and ifstream. Output is formatted to specifications in the 
              the assignment.

*/


#include<stdlib.h>
#include<cstdlib>
#include<iostream>
#include<iomanip>
#include<string>
#include<sstream>
#include<vector>
#include<sys/utsname.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string>
#include<fstream>

using namespace std;

class HW2 {
private:
public:
	HW2();
	string strShorten(string s);
	string getFileData(string filename);
};//end of class

//Default constructor no parameters
HW2::HW2(){}

//Pass in the parameter for the file to be opened, open file and return a string 
string HW2::getFileData(string filename){
	string pathname = "/proc/sys/kernel/";
	ostringstream oss;
    istringstream iss;

	ifstream DataFile;

	string file, retValue;

    oss.clear();
    oss.str("");

	oss << pathname << filename;
	file = oss.str();

	ifstream myfile(file);

	while(true){
		if(!myfile){
			cout << "Failed to open File" << endl;
			exit(-1);
		}
		getline(myfile, retValue);
		break;
	}


	return retValue;

}

//function to shorten a string for better line formatting
string HW2::strShorten(string s){
	int limit = 22;

	s.erase(0,22);

	return s;
}

int main(){

  //initializing variables
        int found;
        istringstream iss;
        ostringstream oss;

	 // reset before each use
        oss.clear();
        oss.str("");

  // required struct for api call to uname
	struct utsname unameData;
	uname(&unameData);

	HW2 hw2;

  // preping the strings for output
	string version = unameData.version;
	string shorten, prefix;
	prefix = version.substr(0,22);
	shorten = hw2.strShorten(version);

	cout << endl;

	cout << "SECTION A ****************************************** " << endl << endl;

  //function calls to retrieve the necessary data
	string osRelease;
	osRelease = hw2.getFileData("osrelease");

	string hostName;
	hostName = hw2.getFileData("hostname");

	string osType;
	osType = hw2.getFileData("ostype");

	string ver, shrtVer;
	ver = hw2.getFileData("version");
	shrtVer = hw2.strShorten(ver);

  //uses oss to create the output to specification
	oss << "Field" << setw(25) << "File data" << setw(30) << "API data" << "\n" << "\n"
      	    << "ostype" << setw(20) << unameData.sysname << setw(31) << osType << "\n"
	    << "hostname" << setw(19) << unameData.nodename << setw(31) << hostName << "\n"
	    << "osrelease" << setw(25) << unameData.release << setw(31) << osRelease << "\n"
            << "version" << setw(36) << prefix << setw(31) << prefix  << "\n"
	    << setw(41) << shorten << setw(31) << shrtVer << "\n"
	    << "machine" << setw(51) << unameData.machine;
	cout << oss.str() << endl << endl;

	cout << "SECTION B ****************************************** " << endl << endl;

  //default path
	string s = getenv("PATH");
	string subString;

  //iterator for vector traversal
	vector<string> vect;
	vector<string>::iterator it;

        cout << "The path is: " << endl << s << "   " << s.length() << " bytes"  << endl;

	cout << endl <<  "Path: " << setw(25) << "Length: " << endl << endl;
	int i = 0;
  //find all semi colons, capture the substring, then delete the substring
	while(s != "\0"){
		found = s.find(":");
		if (found != std::string::npos){
			subString = s.substr(0, found);
			vect.push_back(subString);
			s.erase(0, found + 1);
		}
		else {
			subString = s.substr(0);
			vect.push_back(subString);
			break;
		}
	}

	int totalBytes = 0;
  
  //printing the vector to the required specifications
	for (it = vect.begin(); it != vect.end(); ++it){
		int x = 27;
		int strLength = (*it).length();
		int filler = x - strLength;

		totalBytes += strLength;

		cout << *it;

		for(int i = 0; i < filler;  i++){
			cout << " ";
		}

		cout << strLength << endl;

	}

	cout << endl << left << "Total: " << setw(22) << right << totalBytes << " bytes" << setw(3) << vect.size() << " entries" << endl;

	cout << endl << "SECTION C ****************************************** " << endl << endl;

	int pid;
	int c_pid;

	//fork another process
	cout << "Parent: My PID is " << (int)getpid() << endl;
	cout << "My parent is " << getppid() << endl;

	pid = fork();
	c_pid = fork();

	if (pid <0 || c_pid<0) {
		cerr << "Fork Failed" << endl;
		exit(-1);
	}
	//Middle Child
	else if(pid == 0 & c_pid > 0){
		cout << "Middle: My PID is " << (int)getpid() << endl;
		cout << "My parent is " << getppid() << endl;
		cout << "My child is " << c_pid << endl;
		sleep(3);
		cout << "\nMiddle: " << (int)getpid() << " is awake." << endl;
	}
	//Child
	else if(c_pid == 0 & pid > 0){
		cout << endl;
		sleep(1);
		cout << "Child: My PID is " << (int)getpid() << endl;
		cout << "My parent is " << getppid() << endl << endl;
		sleep(3);
		cout << "\nChild: " << (int)getpid() << " is awake." << endl;
	}
	//Grandchild
	else if(pid == 0 & c_pid == 0) {
		sleep(2);
		cout << "Child: My PID is " << (int)getpid() << endl;
		cout << "My parent is " << getppid() << endl << endl;
		sleep(3);
		cout << "\nChild: " << (int)getpid() << " is awake." << endl;

	}
	else {
		//Printing out hierarchy

		cout << "My children are " << pid << " " << c_pid << endl;

		//initializing parts of the command

		string s = "/bin/ps -f --pid ";
		string sym = ",";
		string gparent = to_string(getppid());
		string parent = to_string((int)getpid());
		string child1 = to_string(pid);
		string child2 = to_string(c_pid);
		string arg1 = to_string(c_pid+1);
		string arg2 = to_string(c_pid+2);

		//Attaching all parts to make the command

		string command = "";
		command.append(s); command.append(gparent);
		command.append(sym); command.append(parent); command.append(sym);
		command.append(child1); command.append(sym); command.append(child2);


		//Print command being issued
		cout << "\nParent: Issuing command: " << command << endl;
		
		//system only takes c str to convert str to cstr
		const char *cstr = command.c_str();

		cout << endl;
		sleep(2);
		system(cstr);

		//before temnination wait for children to finish
		wait(&pid);
		wait(&c_pid);

		cout << "\nParent: All processes finished." << endl;

	}
	wait(0);
	return 0;

}
