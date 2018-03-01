#define SIZE 5

class Queue
{
	protected:
		int pos;
		int queue[SIZE];
	public:
		Queue(int initial);
    void add(int val);
		int avg();
};
