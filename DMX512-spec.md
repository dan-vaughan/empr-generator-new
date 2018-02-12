#DMX512 Spec

Data is transmitted sequentially in asynchronous serial format starting with slot 0 and ending with slot 512. Prior to the first data slot being transmitted, there is a reset sequence consisting of a BREAK followed by a MARK AFTER BREAK and a START code (0x00).

BREAK (88 - 1000000 usec) LOW

MARK AFTER BREAK (~88 usec) HIGH

START CODE (usually 0x00)

MTBF (Mark Time Between Frame) - unspecified time between start code and first framed slot.  

The data transmission format for a slot is as follows:

	- Bit 1 start bit, low or space
	- Bit 2-9 data bits (least significant bit to most significant bit)
	- Bit 10,11 stop bits, high or MARK

Both the start code and the slots are framed as follows:

Bit 1 2 3 4 5 6 7 8 9 10 11
    L X X X X X X X X H  H


DMX programs:
-QLC+
-Freestyler DMX

