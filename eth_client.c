
#include "eth_client.h"

void *EthReceiveThread(void *args)
{
	ssize_t bytesReceived;
	ssize_t numRead;

	printf("Starting receiver thread");
		
	do {
		while (ethclient_init_success == 0) {
			printf("Error: client not initialized.\r\n");
			sleep(1);
		}
		if (eth_rxbuf_bytes_pending = 0) {
			bytesReceived = recvfrom(eth_hdl, eth_rxbuf, 0x1000, 0, (struct sockaddr *)&si_other, &slen);
			
			if (bytesReceived < 1) {
				printf("Error: failed to read from socket.");
			} else {
				eth_rxbuf_bytes_pending = (uint8_t)bytesReceived;
				pthread_kill();
			}	
		}
	} while ( true );
}

void EthTransmitData(uint8_t *DataPtr, uint32_t DataLen)
{
	int packetLen;
	ssize_t bytesSent;
	struct pduMessageHeader_t pduData;
	int txDataLen;
	
	packetLen = DataLen + 8;
	
	if (ethclient_init_success == 0) {
		printf("Error: client not initialized.");
	} else if (DataLen < 0x1000) {
		pduData.id = htonl(0x21);
		pduData.len = htonl(DataLen);
		
		memcpy(eth_txbuf, &pduData, 8);
		memcpy(eth_txbuf + 8, DataPtr, (unsigned long)DataLen);
		
		bytesSent = sendto(eth_hdl, eth_txbuf, (long)txDataLen, 0, (struct sockaddr *)&si_other, slen);
		
		if (bytesSent == packetLen) {
			eth_client_tx_bytes = (long)packetLen + eth_client_tx_bytes;
		} else {
			printf("Error: failed to transmit the message.");
		}
	}
}
