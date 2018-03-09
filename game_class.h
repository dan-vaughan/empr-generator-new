class GAME {
  public:
    GAME();
    int sequence[9];
    int user_sequence[9];
    int limit;
    int flag;
    int index;
    void set_sequence(int difficulty);
    int test_sequence();
    void edit_user_sequence(int input);
    int check_flag();
    void send_sequence();
};
