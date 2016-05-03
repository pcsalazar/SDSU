#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct item
{	
	string name;
	int rate;
	int hours;
};

struct item* ReadFile(char *filename, int *count);
void WriteFile(char *filename, struct item *inv, int count);

int main(int argc, char **argv){
	cout << "Assignment #4-2, Carlo Alejandro Salazar, masc1415" << endl;
	
	int N;
	int i;
	int select;
	struct item* inv = ReadFile(argv[1], &N);
	while(1){
		for(i=0;i<N;i++){
			cout << (i+1) << ": " << inv[i].name << ". Rate: " << 
			inv[i].rate << " Hours: " << inv[i].hours << endl;
		}
		cout << (N+1) << ": [NEW ENTRY]" << endl;
		cout << "Choice >>> ";
		cin >> select;
		
		if(select == 0 || cin.fail()){break;}
		
		select-= 1;
		if(select == N){	//If [NEW ENTRY] chosen
			item *temp_inv = new item[N+1];
			for(i=0;i<N;i++){
				temp_inv[i]=inv[i];
			}
			delete inv;
			inv = temp_inv;
			string newname;
			int newrate;
			int newhours;
			cout << "Name >>> ";
			cin.ignore();
			getline(cin,newname);
			cout << "Rate >>> ";
			cin >> newrate;
			cout << "Additional Hours >>> ";
			cin >> newhours;
			inv[N].name = newname;
			inv[N].rate = newrate;
			inv[N].hours = newhours;
			N++;
		}
		else if(select > N){break;}
		else{
			int amt;
			cout << "Additional Hours >>> ";
			cin >> amt;
			inv[select].hours += amt;
		}
	}
	WriteFile(argv[1],inv,N);
	return 0;
}

void WriteFile(char *filename, struct item *inv, int count){
	int i;
	ofstream fp;
	fp.open(filename);
	fp << count << endl;
	for(i=0;i<count;i++){
		fp << inv[i].name << "," << inv[i].rate << "," << inv[i].hours << endl;
	}
	fp.close();
}

struct item* ReadFile(char *filename, int *count){
	int i;
	string trash;
	
	ifstream fp;
	fp.open(filename); 
	fp >> *count;
	item *inventory = new item[*count];
	getline(fp,trash);	//Ignore N in text file
	for(i=0;i<*count;i++){
		getline(fp,inventory[i].name,',');
		fp >> inventory[i].rate;
		fp.ignore(1,',');
		fp >> inventory[i].hours;
		fp.ignore(1,'\n');
	}
	fp.close();
	return inventory;
}


