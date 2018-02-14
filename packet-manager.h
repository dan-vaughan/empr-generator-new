#define PLEN 10   //Max length of one packet, including start code
#define PMAX 50   //Max number of packets
#define SLEN 10   //Max length of a sequence
#define SMAX 10   //Max number of sequences

class packet_manager
{
	protected:
		void * packets;
	public:
		packet_manager();
		void init(void);
		//void write_packet(int packet_number, char * buf, int length);
		char * getptr(int packet_number);
};
