
#include <vector>
#include "mpq.h"

using namespace std;

//MOST CODE IS ADAPTED FROM LECTURE NOTES AND MODIFIED WHERE REQUIRED

MPQ:: MPQ()
{
}
MPQ::MPQ(int no_of_blocks)
{
	Heap.resize(no_of_blocks+1);
	Location.resize(no_of_blocks);
	currentSize=0; 
}
MPQ:: ~MPQ() 
{
	/*nothing allocated dynamically 
	everything delets automatically out of fucntion scope*/
}

bool MPQ::isEmpty() const
{
	if(currentSize==0)
	{
		return true;
	}
	else
		return false;
}

void MPQ:: insert( const int & no_of_seats, const int & block_no  )
{

	int hole= ++currentSize;
	for (; hole>1 && (no_of_seats < Heap[hole/2].seats || (no_of_seats == Heap[hole/2].seats && block_no < Heap[hole/2].block_num )) ; hole/=2)
	{
		Heap[hole] = Heap[hole/2];
		Location[Heap[hole].block_num]= hole;
	}
	Heap[hole].seats = no_of_seats;
    Heap[hole].block_num = block_no;
    Location[Heap[hole].block_num] = hole ;
}

void MPQ::percolateDown(int hole) 
{
	int child;
	in_view temp = Heap[hole]; 

	for (; hole * 2 <= currentSize; hole = child)
	{
		child = hole * 2;
		if (child != currentSize && (Heap[child+1].seats < Heap[child].seats || (Heap[child].seats == Heap[child + 1].seats && Heap[child].block_num > Heap[child+1].block_num)))
			child++;
		if (Heap[child].seats < temp.seats || (Heap[child].seats == temp.seats && Heap[child].block_num < temp.block_num)) 
		{
			Heap[hole].seats = Heap[child].seats;
			Heap[hole].block_num= Heap[child].block_num;
			Location[Heap[hole].block_num] = hole;
		}
		else
		{
			break;
		}
	}
	Heap[hole] = temp; 
	Location[Heap[hole].block_num] = hole;
}

int MPQ:: Remove(const int & remove_block)
{
	int block_capacity=0;
	block_capacity= Heap[Location[remove_block]].seats;
	Heap[Location[remove_block]]= Heap[currentSize--];
	percolateDown(Location[remove_block]);
	return block_capacity;
	
}


void MPQ:: deleteMin( int & minsize , int & buildingn)
{
	minsize= Heap[1].seats;
	buildingn = Heap[1].block_num;

	Heap[1]= Heap[currentSize--];
	percolateDown(1);
}

/*const int MPQ:: GetMin() const
{

	return Heap[ 1 ].block_num;
}*/
 