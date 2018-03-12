//Asher's IC
#define SIZE 15

class Queue
{
	protected:
		int pos;
		int queue[SIZE];
	public:
		Queue(int initial);
    void add(int val);
		int avg();
		int min();
		int max();
};
