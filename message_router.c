#include <message_router.h>
#include <eth_client.h>

int IpcTestMessageRouter_DataReceived(uint8_t *DataPtr, uint32_t DataLen)
{
	uint32_t timestamp;
	struct msgHeader_t msgHeaderTx;
	struct msgHeader_t msgHeader;
	uint8_t i;
	
	if (globalSoftwareState == 0) {
		if (*DataPtr == 1) {
			globalSoftwareState = 1;
			if (globalTegraDevice == 1) {
				printf("I am Tegra A\r\n");
			}
		} else if (*DataPtr == 2) {
			globalSoftwareState = 1;
			if (globalTegraDevice = 2) {
				printf("I am Tegra B\r\n");
			}
		} else {
			printf("Error: Destination ID invalid.");
		}
	}
	if (DataPtr[1] == 4) {
		printf("CAN message 0x%x received\n", (unsigned long)*(unsigned int *)(DataPtr + 0xc));
		printf("Data: ");
		for (i = 0; i < 8; i++) {
			printf("0x%x", (unsigned long)DataPtr[(unsigned long)i + 0x10]);
		}
		printf("\r\n");
		
		if (globalTegraDevice == 1) {
			if ((*(int *)(DataPtr + 0xc) == 0x65) && (DataPtr[0x10] == 0x80)) {
				printf("IPC test state: test");
				globalSoftwareState = 3;
			} else if ((*(int *)(DataPtr + 0xc) == 0x65) && (DataPtr[0x10] == 0)) {
				printf("IPC test state: run");
				globalSoftwareState = 2;
			}
		}
		if (globalTegraDevice == 2) {
			if ((*(int *)(DataPtr + 0xc) == 0x85) && (DataPtr[0x10] == 0x80)) {
				printf("IPC test state: test");
				globalSoftwareState = 3;
			} else if ((*(int *)(DataPtr + 0xc) == 0x85) && (DataPtr[0x10] == 0)) {
				printf("IPC test state: run");
				globalSoftwareState = 2;
			}
		}
		if (globalSoftwareState == 3) {
			msgHeaderTx.dst = 4;
			if (globalTegraDevice == 1) {
				msgHeaderTx.src = 1;
				msgHeaderTx.app_selector = 0x16;
				msgHeader.msg_id = 0xaf;
			} else if (globalTegraDevice == 2) {
				msgHeaderTx.src = 2;
				msgHeaderTx.app_selector = 0x26;
				msgHeaderTx.msg_id = 0xbf;
			} else {
				msgHeaderTx.src = 0;
				msgHeaderTx.app_selector = 0x200;
			}
			if (msgHeaderTx.src != 0) {
				timestamp = time((time_t *)0x0);
				msgHeaderTx.timestamp = (uint32_t)timestamp;
				msgHeaderTx.length = (uint16_t)DataLen;
				memcpy(txMirror, DataPtr, (unsigned long)DataLen);
				memcpy(txMirror, &msgHeaderTx, 0x10);
				printf("IPC Test: Loopback CAN message\r\n");
				MessageRouter_TransmitData(1, (unsigned int)msgHeaderTx.length, (uint8_t *)txMirror);
			}
		}
	}
}

void MessageRouter_DataReceived(uint8_t *DataPtr, uint32_t DataLen)
{
	unsigned int msgType;
	struct msgHeader_t *msgHeader;
	
	msgr_rx_bytes = DataLen + msgr_rx_bytes;
	if (DataLen < 8) {
		printf("Error: payload length too small to contain a valid message header.");
	} else if ((*DataPtr == '\x01') || (*DataPtr == '\x02')) {
		msgType = *(unsigned int *)(DataPtr + 8);
		
		if (msgType == 0x33) {
			MessageRouter_L3Mon_L3MonFwdData(DataPtr, DataLen);
		} else {
			if (msgType < 0x34) {
				if (msgType == 0) {
					MessageRouter_IpcTest_IpcTestFwdData(DataPtr, DataLen);
					return;
				}
				if (msgType == 0x22) {
					MessageRouter_L2Mon_L2MonFwdData(DataPtr, DataLen);
					return;
				}
			} else {
				if (msgType == 0x66) {
					MessageRouter_IpcTest_IpcTestFwdData(DataPtr, DataLen);
					return;
				}
				if (msgType == 0xdd) {
					MessageRouter_DsCom_DsComFwdData(DataPtr, DataLen);
					return;
				}
			}
			printf("Unexpected message ID: %d \n", (unsigned long)*(unsigned int *)(DataPtr + 8));
		}
	} else if ((*DataPtr == '\x03') || (*DataPtr == '\x04')) {
		EthTransmitData(DataPtr, DataLen);
	}
	return;
}

int MessageRouter_TransmitData(uint8_t Channel, uint32_t DataLen, uint8_t *DataPtr)
{
	msgr_tx_bytes = DataLen + msgr_tx_bytes;
	
	if (Channel == 2) {
		if (Channel == 3) {
			MessageRouter_DsCom_DsComFwdData(DataPtr, DataLen);
		} else if (Channel == 1) {
			EthTransmitData(DataPtr, DataLen);
		} else {
			printf("Invalid channel: %d\n", (unsigned long)Channel);
		}
	}
	return;
}

void MessageRouter_L2Mon_L2MonFwdData(uint32_t *DataPtr, uint32_t DataLen) {
	return;
}

void MessageRouter_L3Mon_L3MonFwdData(uint32_t *DataPtr, uint32_t DataLen) {
	return;
}

void MessageRouter_IpcTest_IpcTestFwdData(uint32_t *DataPtr, uint32_t DataLen) {
	return;
}

void MessageRouter_DsCom_DsComFwdData(uint32_t *DataPtr, uint32_t DataLen) {
	return;
}

void MessageRouter_EthClient_EthClientFwdData(void)
{
	do {
		if (eth_rxbuf_bytes_pending < 9) {
			printf("Error: payload length mismatch.");
		} else {
			MessageRouter_DataReceived(eth_rxbuf + 8, eth_rxbuf_bytes_pending - 8);
			eth_client_rx_bytes = (eth_rxbuf_bytes_pending - 8) + eth_client_rx_bytes;
		}
	} while ( true );
}