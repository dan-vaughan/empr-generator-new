#define PLEN 10   //Max length of one packet, including start code
#define PMAX 50   //Max number of packets


class packet_manager
{
	protected:
		void * packets;
	public:
		packet_manager();
		void init(void);
		char * getptr(int packet_number);
};
