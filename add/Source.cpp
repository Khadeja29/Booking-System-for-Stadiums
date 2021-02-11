#include <iostream>
#include <string>
#include <unordered_map> 
#include <fstream>
#include <sstream>
#include <vector>
#include<tuple>
#include "mpq.h"
using namespace std; 

struct column_info
{
	int col_no;
	string custom_name;
	int block;
	string row;

};

struct seat_info
{
	string B;
	string R;
	int C;
	
};
ofstream MyFile("filename.txt");
int columns;
vector<string> roname;	
vector<string> blkname;

/*Unordered map1: to store stadium information
	  string block name as key with value as an unordered_map of row names
	  The key of unordered maps is the string row name and its value is a vector 
	  that store a struct for each column containing column number 
	  and customer infromation
*/
unordered_map<string,unordered_map <string, vector<column_info>>> stadium;

/*Unordered map2: get function string is customer name and seat info is a 
struct storing B R C info to make it O(1)*/
unordered_map<string,seat_info> stad;

/*
   unordered map heap: This map has a string key for rowname
   and the value for each row name is a Minheap that stores the blocks 
   in that row and maintains a the minium block 
*/
unordered_map<string,MPQ> rowheap;

/*
    cancel_reservation: parameter (Customer name)
	Cancel a seat if customer has already booked a seat
	COMPLEXCITY:
	-Finding if customer has an already reserved seat/unordered map =O(1)
	-Changing block capacity to mintain row heap:
	  - remove block = O(log B)
	  - decrease capacity , insert the same block =O(log B)
	  heence total complexity = 0(log B)

*/

void cancel_reservation( string customer_name)
{
	//The customer has a reserved seat
	string nametoprint = customer_name;
	customer_name = customer_name.substr(0,3);
	if ( stad.find(nametoprint) != stad.end())
	{
		string b =stad[nametoprint].B;
		string r = stad[nametoprint].R;
		int c = stad[nametoprint].C;

		//cancel rfrom stadium unorderedmap1
		stadium[b][r][c].custom_name= "---";

		int toremove= stadium[b][r][c].block;

		//reduce seat capacity from row heap 
		//Remove block

		int block_capaci = rowheap[r].Remove(toremove);
		//add block back with decresed capacpity 
		rowheap[r].insert(block_capaci-1,toremove);
		//cancel from stad unorderedmap2
		stad.erase(nametoprint);

		MyFile<<"Cancelled the reservation of " << nametoprint <<endl;

	}
	else
	{
		MyFile<<"Could not cancel the reservation for " << nametoprint <<"; no reservation found!"<<endl;
	}
}
/*
   reserve_seat_by_row : parameters( takes a customer name and row name)
   Reserves a seat in the block with the lowest capacity and if the capacities 
   of any two block is same
   it reserves in the left-most block amonsgt the blocks with the same capacity
   It the also occupies the first empty/left-most avaiavble column in the most empty block
   Complexity breakdown:
   -Finding customer = O(1) unordered map_2 used
   -Delete Minblock from heap = O(log B) 
   -Increase capacity of block and insert in heap = O(1)+ O(log B)
   -Check if capacity of row not full and filling unordered maps with info = O(1)
   -Finding the left most column = O(C)
   hence total complexity - O(Log B+ C)
*/

void reserve_seat_by_row (string customer_name, string row_name)
{
	string nametoprint = customer_name;
	customer_name = customer_name.substr(0,3);
	//check if customer doesnt have a reserved seat
	if( stad.find(nametoprint) == stad.end())
	{
		int minsize=0;
		int bln=0;
		//finds minimum block from row heap
		rowheap[row_name].deleteMin(minsize,bln);
		minsize++; //increases capacity of block
		rowheap[row_name].insert(minsize,bln); //inserts that block again
		//Find the leftmost empty column, the first empty column
		int test = 0; 
		for (; test< columns; test++)
		{
			string bn= blkname[bln];
			if(stadium[bn][row_name][test].custom_name == "---")
			{
				break;
			}
		}
		if (  test >= columns) // if row capacity is full no reservation possible/made
		{
			MyFile<< nametoprint <<" could not reserve a seat!" << endl;
		}
		else 
		{
			//if row's min block has space
			//add customer to unordered map2
			stad[nametoprint];
			stad[nametoprint].B=blkname[bln];
			stad[nametoprint].R= row_name;
			stad[nametoprint].C = test;
			//ass customer to unordered map1
			int temp = test;
			stadium[blkname[bln]][row_name][test].custom_name = customer_name;
			
			MyFile << nametoprint << " has reserved " << blkname[bln] << " " ;
			MyFile <<  row_name << "-" << temp <<" by emptiest block" << endl;
		}
	}
	else
	{
		//if cutomer has an already resereved seat
		MyFile<< nametoprint <<" could not reserve a seat!" << endl;
	}
}

/*
    reserves_seat : parameter (Customer name, block ,row and column to be reserved)
	Reserves a seat if customer is not already booked or the seat is not already occupied 
	COMPLEXCITY:
	-Finding if customer has an already reserved seat/unordered map =O(1)
	-Changing block capacity to mintain row heap:
	  - remove block = O(log B)
	  - increase capacity , insert the same block =O(log B)
	  heence total complexity = 0(log B)

*/
void reserve_seat( string customer_name, string block_name, string row_name, int column_num)
{
	string nametoprint = customer_name;
	customer_name = customer_name.substr(0,3);
	//check if seat is not occupied
	if( stadium[block_name][row_name][column_num].custom_name == "---")
	{
		//check if customer doesnot have an already reserved seat
		if (stad.find(nametoprint) == stad.end())
		{
			//reserve customer in unordered_map1 : stadium with three letter name
			stadium[block_name][row_name][column_num].custom_name = customer_name;
			stadium[block_name][row_name][column_num].col_no = column_num;

			//reserve customer is unordred map2: stad with full name to avoid clash of same name
			stad[nametoprint];
			stad[nametoprint].B=block_name;
			stad[nametoprint].R= row_name;
			stad[nametoprint].C=column_num;

			//find the block number that has increased capacity for heap
			int addto=stadium[block_name][row_name][column_num].block;

			//remove block from heap
			int block_capacity = rowheap[row_name].Remove(addto);

			//increase capacity of that block by one and re-insert
			rowheap[row_name].insert(block_capacity+1,addto);
			MyFile << nametoprint <<" has reserved " << block_name << " ";
			MyFile << row_name << "-" <<  column_num <<endl;
		}
		else
		{
			//if customer has an already reseved seat, no reservation made
			MyFile<< nametoprint <<" could not reserve a seat!" << endl;
		}
	}
	else
	{ 
		//if the specific required seat is already occupied/reserved
		MyFile<< nametoprint <<" could not reserve a seat!" << endl;
	}

}

/* get_seat : parameter( customer_name)
   uses stad i.e unordered map with key as customer to identify 
   if a customer has a reserved seat or not
   Complexity = O(1)
*/

void get_seat (string customer_name)
{
	string nametoprint = customer_name;
	//If customer is registered/has a seat using unorderedmap2: stad
	if ( stad.find(nametoprint)!= stad.end())
	{
		//retiriving block,row,column info of reserved seat
		string block_name= stad[nametoprint].B ;
		string row_name= stad[nametoprint].R ;
		int column_number= stad[nametoprint].C;
		MyFile<<"Found that " <<nametoprint<<" has a reservation in " <<block_name<<" "<<row_name<<"-"<<column_number<<endl;
		
	}
	else
	{ //customer has no reserved seat
		MyFile<<"There is no reservation made for "<<nametoprint<<"!"<<endl;
	
	}
}

void print()
{
	for (int i = 0; i < blkname.size(); i++)
	{
		string B_name = blkname[i];
		MyFile << B_name << endl;
		MyFile << "~~~~~~~"<<endl;

		for (int j = 0; j < roname.size(); j++)
		{
			string R_name = roname[j];
			MyFile<<R_name<<" "<<": ";

			vector<column_info>ptemp= stadium[B_name][R_name];
			for (int k = 0; k < columns; k++)
			{
				MyFile<< ptemp[k].custom_name<<" ";
			}
			MyFile<<endl;
		}
		MyFile<<"======="<< endl<<endl; 

	}

}

int main()
{
	//OPEINING AND READING FROM INPUT FILE
	ifstream input;
	string filename ="inputs.txt";
	input.open(filename.c_str());

	string line1,sub;
	 //reading block names into a vector
	
	getline(input,line1);
	
	istringstream s(line1);
	while(s >> sub)
	{
		
		blkname.push_back(sub);
	}

	string line2,sub2;
	//reading row names into a vector
	getline(input,line2);
	istringstream ss(line2);
	while(ss >> sub2)
	{
		roname.push_back(sub2);
	}

	// reading colum number to a integer
	string line3,col;
	getline(input,line3);
	istringstream sss(line3);
	sss>>col;
	columns= stoi(col);

	//Initallizing empty Stadium
	for (int i = 0; i < blkname.size(); i++)
	{
		string block= blkname[i];
		stadium[block];
		
		for (int j = 0; j < roname.size(); j++)
		{
			
			string row_name = roname[j];
			stadium[block][row_name];
			vector<column_info> tempcol;
			for (int k = 0; k < columns; k++)
			{
				column_info temp;
				temp.col_no=k;
				temp.custom_name="---";
				temp.block=i;
				temp.row= roname[j];
				tempcol.push_back(temp);
			}
			stadium[block][row_name]= tempcol;
		}

	}
	
	//Filling the row heap
	for (int m = 0; m < roname.size(); m++)
	{
		MPQ temp1(blkname.size());
		string row_n= roname[m];
		rowheap[row_n];
		for (int n = 0; n < blkname.size(); n++)
		{
			temp1.insert(0,n);
		}
		rowheap[row_n]=temp1;                                    
	}

	//Reading the imputs for different commands
	string end,command;
	string customer,block,row,colum;

	while(getline(input,end))
	{
		istringstream sq(end);
		sq>> command;
		
		if ( command == "print")
		{
			MyFile<<endl;
			print();
		}
		else if (command == "reserve_seat")
		{
			sq>>customer>>block>>row>>colum;
			int column = stoi(colum);
			reserve_seat(customer,block,row,column);

		}
		else if ( command == "cancel_reservation")
		{
			sq>>customer;
			cancel_reservation(customer);
		}
		else if (command =="reserve_seat_by_row")
		{
			sq>>customer>>row;
			reserve_seat_by_row(customer,row);
		}
		else if ( command == "get_seat")
		{
			sq>>customer;
			get_seat(customer);
		}
	}
	
	return 0;	

}
