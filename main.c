#include "fcp.hpp"

int main(int argc, char *argv[])
{
	Fcp fcp;

	fcp_signal_t signal =  {
		.start = 0,
		.length = 16,
		.scale = 0.01,
		.offset = 0,
		.type = UNSIGNED,
		.endianess = LITTLE,
	};

	CANdata msg = {
		.sid = 9 + 10*32,
		.dlc = 8,
		.data = {10000, 10000, 10000, 10000}
	};

	printf("%f\n", fcp_decode_signal_double(msg, signal));

	return 0;
}
