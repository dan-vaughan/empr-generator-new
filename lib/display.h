enum Shift_Type
{
	LEFT,
	RIGHT
};

void printchar(char);
void setup_display();
void reset();
void printstr(char *);
void return_home();
void shift_line();
void cursor_shift(Shift_Type t, int amount);
void clear_display();
void putcustom(char custom);
