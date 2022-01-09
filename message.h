#include <stdint.h>

struct msgHeader_t {
	uint8_t dst;
	uint8_t src;
	uint16_t length;
	uint32_t timestamp;
	uint32_t app_selector;
	uint32_t msg_id;
};