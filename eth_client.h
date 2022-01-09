#include "common.h"
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <linux/in.h>
#include <sys/socket.h>
#include <pthread.h>


char *aurix_ip;
int eth_hdl;
int ethclient_init_success = 0;
char *eth_rxbuf[4096];
char *eth_txbuf[4096];
uint64_t eth_client_rx_bytes;
uint64_t eth_client_tx_bytes;
unsigned int eth_rxbuf_bytes_pending;
unsigned int slen = 0x10;

struct sockaddr_in si_other; // Aurix
struct sockaddr_in si_myaddr; // Local
struct pduMessageHeader_t {
	uint32_t id;
	uint32_t len;
};

void *EthReceiveThread(void *args);
void EthTransmitData(uint8_t *DataPtr, uint32_t DataLen);