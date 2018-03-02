#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream> 
#include <vector>
#include <sstream>
#include <cstdlib>
using namespace std;

struct process{
	pid_t pid;
	string name, status;
};

vector<process> P;

bool valid(pid_t pid){
	bool found = false;
	for(int i=0; i<P.size(); i++)
		if(P[i].pid == pid)
			found = true;
	return found;
}

int main()
{
	while(true){
		vector<string> input;
		cout<<"BP > ";
		string command;
		getline (cin, command);
		istringstream s(command);
		
		while(s>>command)
	       input.push_back(command);			
			
		if(input.at(0) == "bg"){
			
			if(input.size() == 4){
				cout<<"Fork Bomb"<<endl;
				continue;	
			}
			
			process p;
			p.name = input[1];
				
        	input[1] = "./" + input[1];
			char *arguments[input.size()-1];
			for(int i=0; i<input.size()-1; i++)
    			arguments[i] = (char*)input[i+1].c_str();
    		arguments[input.size()-1] = NULL;
			pid_t pid = fork();
			if(pid == 0){
				execvp(arguments[0],arguments);
			}
			else if(pid > 0){
				p.pid = pid;
				p.status = "running";
        		P.push_back(p);
			}
		}
		else if(input.at(0) == "bglist"){
			if(input.size() != 1){
				cout<<"Invalid Input"<<endl;
				continue;	
			}

			int count = 0;
			for(int i=0; i<P.size(); i++){
				if(P[i].status != "terminated"){
					count++;
					cout<<P[i].pid<<"	:	"<<P[i].name<<endl;
				}	
			}
			if(count == 0)
				cout<<"No Background Processes"<<endl;
		}
		else if(input.at(0) == "bgstop"){
			if(input.size() != 2){
				cout<<"Invalid Input"<<endl;
				continue;
			}
			pid_t pid = atoi(input[1].c_str());
			if(valid(pid)){
				kill(pid, SIGSTOP);
				for(int i=0; i<P.size(); i++)
					if(P[i].pid == pid)
						P[i].status = "stopped";
				cout<<pid<<" stopped"<<endl;
			}
			else{
				cout<<"Invalid PID"<<endl;
			}
		}
		else if(input.at(0) == "bgrestart"){
			if(input.size() != 2){
				cout<<"Invalid Input"<<endl;
				continue;
			}
			pid_t pid = atoi(input[1].c_str());
			if(valid(pid)){
				kill(pid, SIGCONT);
				for(int i=0; i<P.size(); i++)
					if(P[i].pid == pid)
						P[i].status = "running";
				cout<<pid<<" restarted"<<endl;
			}
			else{
				cout<<"Invalid PID"<<endl;
			}
		}
		else if(input.at(0) == "bgstate"){
			if(input.size() != 2){
				cout<<"Invalid Input"<<endl;
				continue;
			}
			pid_t pid = atoi(input[1].c_str());
			if(valid(pid)){
				for(int i=0; i<P.size(); i++)
					if(P[i].pid == pid)
						cout<<pid<<": "<<P[i].status<<endl;
			}
			else{
				cout<<"Invalid PID"<<endl;
			}
		}
		else if(input.at(0) == "bgkill"){
			if(input.size() != 2){
				cout<<"Invalid Input"<<endl;
				continue;
			}
			pid_t pid = atoi(input[1].c_str());
			if(valid(pid)){
				kill(pid,SIGKILL);
				cout<<pid<<" killed"<<endl;
					for(int i=0; i<P.size(); i++)
						if(P[i].pid == pid)
							P[i].status = "terminated";
			}
			else{
				cout<<"Invalid PID"<<endl;
			}
		}
		else if(input.at(0) == "exit"){
			if(input.size() != 1){
				cout<<"Invalid Input"<<endl;
				continue;
			}
			for(int i=0; i<P.size(); i++){
				int status;
				pid_t result = waitpid(P[i].pid, &status, WNOHANG);
				if(result == 0){
					kill(P[i].pid,SIGKILL);
					cout<<P[i].pid<<" killed"<<endl;
				}
			}	
			exit(0);
		}
		else{
			cout<<"Invalid Input"<<endl;
		}
	}
}
