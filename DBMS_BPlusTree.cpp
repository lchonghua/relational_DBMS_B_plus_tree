#include <iostream>
#include <string>
#include <fstream>
#include <list>

#include <algorithm>
#include "bpt.h"
using namespace std;

//*************************************************************
//                     Variables
//*************************************************************
const int INSERT = 1;
const int DEL = 2;
const int SNAP = 3;
const int UPDATE = 4;
const int SEARCH = 5;
const int DATA = 6;
ifstream infile;
string inputStr;
int recordCount = 0;
list<record> recordList;


int inputType;//1-INSERT, 2-DELETE, 3-SNAPSHOT, 4-UPDATE, 5-SEARCH
int commandMarker;//1-INSERT, 2-DELETE, 3-SNAPSHOT, 4-UPDATE, 5-SEARCH
//*************************************************************
//                     Function Prototypes
//*************************************************************
int checkInputType(string);

int main()
{
	node * root;
	root = NULL;
	
	infile.open("input.txt");
	if(infile)
	{
		while(!infile.eof())
		{
			getline(infile, inputStr);
			if(!inputStr.empty())
			{
				transform(inputStr.begin(), inputStr.end(), inputStr.begin(), ::tolower);
				inputType = checkInputType(inputStr);
				
				switch(inputType)
				{
				case INSERT:
					cout<<endl;
					commandMarker = INSERT;
					cout<<"\n-------";
					cout<<"\n*INSERT";
					cout<<"\n-------";
					break;
				case DEL:
					cout<<endl;
					commandMarker = DEL;
					cout<<"\n-------";
					cout<<"\n*DELETE";
					cout<<"\n-------";
					break;
				case SNAP:
					cout<<endl;
					commandMarker = SNAP;
					cout<<"\n---------";
					cout<<"\n*SNAPSHOT";
					cout<<"\n---------";
					cout<<"\nThe SnapShot of the tree:";
					cout<<"\nThe number of records in the table:      "<<recordCount;
					cout<<"\nThe number of blocks for the records:    "<<ceil(((double)recordCount)/4);
					cout<<"\nThe depth of the tree:                   "<<height(root);
					cout<<"\nThe first and last key of each node are:\n";
					print_tree(root);
					cout<<endl;
					system("pause");
					break;
				case UPDATE:
					cout<<endl;
					commandMarker = UPDATE;
					cout<<"\n-------";
					cout<<"\n*UPDATE";
					cout<<"\n-------";
					break;
				case SEARCH:
					cout<<endl;
					commandMarker = SEARCH;
					cout<<"\n-------";
					cout<<"\n*SEARCH";
					cout<<"\n-------";
					break;
				case DATA:
					cout<<"\n"<<inputStr;
					if (commandMarker == INSERT)
					{
						if(find(root, inputStr) != NULL)
							cout<<"\n\nErorr: Record already exits!\n";
						else
						{
							root = insert(root, inputStr, recordList);
							record * temp_record;
							temp_record = find(root, inputStr);
							recordCount ++;
						}
					}
					else if (commandMarker == DEL)
					{
						record * temp_record;
						temp_record = find(root, inputStr);
						if(temp_record != NULL)
						{
							recordCount --;
							for(list<record>::iterator it = recordList.begin(); it != recordList.end(); it++)
							{
								if(inputStr.compare(it->name) == 0)
								{	
									it = recordList.erase(it);
								}

							}
							root = deletion(root, recordList);
						}
						else
							cout<<"\nRecord not found!";

					}
					else if (commandMarker == UPDATE)
					{
						record * temp_record;
						temp_record = find(root, inputStr);
						if(temp_record != NULL)
						{
							getline(infile, inputStr);
							while (inputStr.empty())//if there is empty line, keep reading next one
													//potential problem: if this is the last line--infinite looping
								getline(infile, inputStr);
							int temp_type = checkInputType(inputStr);
							if(temp_type == DATA)
								temp_record->info = inputStr;					
							else
							{
								cout<<"\nError in updating information";
								commandMarker = temp_type;
								if(temp_type == SNAP)
								{
									cout<<"\n"<<inputStr;
									cout<<"\nThe SnapShot of the tree:";
									cout<<"\nThe number of records in the table: "<<recordCount;
									cout<<"\nThe depth of the tree: "<<height(root);
									cout<<"\nThe first and last key of each node are:\n";
									print_tree(root);
									
								}
							}

						}
						else
							cout<<"\nNo record found.";
							
						
					}
					else if (commandMarker == SEARCH)
					{
						record * temp_record;
						temp_record = find(root, inputStr);
						
						if(temp_record != NULL)
						{
							cout<<"\nStudent Name: "<<temp_record->name;
							cout<<"\nStudent information: "<<temp_record->info;
							
						}
						else
							cout<<"\nNo record found.";
					}
					break;	
				}
			}
		}
	}
	infile.close();
		
	/*for (list<record>::iterator it= recordList.begin() ; it != recordList.end(); ++it)
		cout <<"\nStudent name: "<<(*it).name<<" record address: "<<&(*it);*/
	
	cout<<"\nEND OF PROGRAM"<<endl;
	system("pause");
	return 0;
}
int checkInputType(string str)
{
	if(str == "*insert")
		return INSERT;
	else if(str == "*delete")
		return DEL;
	else if(str == "*snapshot")
		return SNAP;
	else if(str == "*update")
		return UPDATE;
	else if(str == "*search")
		return SEARCH;
	else
		return DATA;
}