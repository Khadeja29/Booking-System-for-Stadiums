#ifndef mpq_h
#define mpq_h
#include <string>
#include <vector> 
using namespace std;

struct in_view
{
	int block_num;
	int seats;
};


class MPQ
{
public:
	MPQ();
	MPQ( int no_of_blocks); //	constructor
	~MPQ();//destructor
	bool isEmpty( ) const;//return true if 
	void insert( const int & no_of_seats,const int & block_no );
	void deleteMin( int & minsize , int & buildingn);
	int Remove(const int & x);
	const int GetMin() const;

private:
	int currentSize; 
	vector<in_view> Heap;
	vector<int> Location;
	void percolateDown( int hole );
};
#endif