class packet_manager
{
	protected:
		void init(void);
	public:
		packet_manager();
		void write_packet(int packet_number, char * buf);
		char * read_packet(int packet_number);
		char * * packets;
		short * sequences;
};
