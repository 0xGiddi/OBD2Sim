#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum pid_data_index {
	PID_VALUE_1 = 1,
	PID_VALUE_2,
	PID_VALUE_3,
	PID_VALUE_4,
	PID_VALUE_5,
};

struct obd_pid {
	uint8_t pid;
	struct {
		uint8_t a;
		uint8_t b;
		uint8_t c;
		uint8_t d;
		uint8_t e;
	} bytes;
	char *name;
	char *units;
	float max;
	float min;

	struct obdPid *next;
};

float obd_m1m2_std_decode(struct obd_pid *pid, int data_index) {
	float fret;
	switch (pid->pid) {
		case 0x61: case 0x62: case 0x8e:
			fret = pid->bytes.a - 125.f;
			break;
		case 0x05: case 0x0f: case 0x46: case 0x5c:
			fret = pid->bytes.a - 40.f;
			break;
		case 0x50:
			fret = pid->bytes.a * 10.f;
			break;
		case 0x14: case 0x15: case 0x16: case 0x17: case 0x18: case 0x19: case 0x1a: case 0x1b:
			if (data_index == PID_VALUE_2)
				fret = (100.f / 128.f * pid->bytes.b) - 100.f;
			else 
				fret = pid->bytes.a / 200.f;
			break;
		case 0x0e:
			fret = (pid->bytes.a / 2.f) - 64.f;
			break;
		case 0xa6:
			fret = (pid->bytes.a << 24 | pid->bytes.b << 16 | pid->bytes.c << 8 | pid->bytes.d) / 10.f;
		case 0x4f:
			if (data_index == PID_VALUE_2)
				fret = pid->bytes.b;
			else if (data_index == PID_VALUE_3)
				fret = pid->bytes.c;
			else if (data_index == PID_VALUE_4)
				fret = pid->bytes.d * 10.f;
			else
				fret = pid->bytes.a;
		case 0x0b: case 0x0d: case 0x30: case 0x33:
			fret = pid->bytes.a;
			break;
		// cases 54 1f 21  31  4d 4e 63
		case 0x54: case 0x1f: case 0x21: case 0x31: case 0x4d: case 0x4e: case 0x63:
			fret = pid->bytes.a << 8 | pid->bytes.b;
			break;
		case 0x42:
			fret = (pid->bytes.a << 8 | pid->bytes.b) / 1000.f;
			break;
		case 0x53:
			fret = (pid->bytes.a << 8 | pid->bytes.b) / 200.f;
			break;
		case 0x5d:
			fret = ((pid->bytes.a << 8 | pid->bytes.b) / 128.f) - 210;
			break;
		case 0x10:
			fret = (pid->bytes.a << 8 | pid->bytes.b) / 100.f;
			break;
		case 0xa2:
			fret = (pid->bytes.a << 8 | pid->bytes.b) / 32.f;
			break;
		case 0x5e:
			fret = (pid->bytes.a << 8 | pid->bytes.b) / 20.f;
			break;
		// cases 3c 3d 3e 3f 7c
		case 0x3c: case 0x3d: case 0x3e: case 0x3f: case 0x7c:
			fret = ((pid->bytes.a << 8 | pid->bytes.b) / 10.f) - 40.f;
			break;
		case 0x32:
			fret = ((int8_t)pid->bytes.a << 8 | (int8_t)pid->bytes.b) / 4.f;
			break;
		case 0x0c:
			fret = (pid->bytes.a << 8 | pid->bytes.b) / 4.f;
			break;
		case 0x47: case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c:
		case 0x04: case 0x11: case 0x2c: case 0x2e: case 0x2f: case 0x45:
		case 0x52: case 0x5a: case 0x5b:
			fret = (100.f / 255.f) * pid->bytes.a;
			break;
		case 0x07: case 0x08: case 0x09: case 0x2d:
			fret = ((100.f / 128.f) * pid->bytes.a) - 100.f;
			break;
		case 0x55: case 0x56: case 0x57: case 0x58:
			if (data_index == PID_VALUE_2)
				fret = ((100.f / 128.f) * pid->bytes.b) - 100.f;
			else
				fret = ((100.f / 128.f) * pid->bytes.a) - 100.f;
			break;
		case 0x43:
			fret = (100.f / 255.f) * (pid->bytes.a << 8 | pid->bytes.b);
			break;
		case 0x23: case 0x59:
			fret = (pid->bytes.a << 8 | pid->bytes.b)  * 10.f;
			break;
		case 0x0a:
			fret = pid->bytes.a * 3.f;
			break;
		case 0x24: case 0x25: case 0x26: case 0x27: case 0x28: case 0x29: case 0x2a: case 0x2b:
			if (data_index == PID_VALUE_2)
				fret = (8.f / 65536.f) * (pid->bytes.c << 8 | pid->bytes.d);
			else 
				fret = (2.f / 65536.f) * (pid->bytes.a << 8 | pid->bytes.b);
			break;
		case 0x34: case 0x35: case 0x36: case 0x37: case 0x38: case 0x39: case 0x3a: case 0x3b:
			if (data_index == PID_VALUE_2)
				fret = ((pid->bytes.c << 8 | pid->bytes.d) / 256.f) - 128.f;
			else 
				fret = (2.f / 65536.f) * (pid->bytes.a << 8 | pid->bytes.b);
			break;
		case 0x44:
			fret = (2.f / 65536.f) * (pid->bytes.a << 8 | pid->bytes.b);
		case 0x22:
			fret = 0.079 * (pid->bytes.a << 8 | pid->bytes.b);
			break;
		case 0xa4:
			fret = (pid->bytes.c << 8 | pid->bytes.d) / 1000.f;
			break;
		case 0x66:
			if (data_index == PID_VALUE_2)
				fret = (pid->bytes.d << 8 | pid->bytes.e) / 32.f;
			else
				fret = (pid->bytes.b << 8 | pid->bytes.c) / 32.f;
		case 0x67:
			if (data_index == PID_VALUE_2)
				fret = pid->bytes.c - 40.f;
			else
				fret = pid->bytes.b - 40.f;
		case 0x68:
			if (data_index == PID_VALUE_2)
				fret = pid->bytes.c - 40.f;
			else
				fret = pid->bytes.b - 40.f;
		case 0xa9:
			fret = pid->bytes.b & 1;
			break;
		case 0xa5:
			fret = pid->bytes.b / 2.f;
			break;
		default:
			return -1;
	}
	return fret;
}
