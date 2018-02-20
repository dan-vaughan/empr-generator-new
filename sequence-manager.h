#define SLEN 10   //Max length of a sequence
#define SMAX 10   //Max number of sequences

class sequence_manager
{
	protected:
		void * sequences;
	public:
		sequence_manager();
		void init(void);
		char * getptr(int sequence_number);
};
