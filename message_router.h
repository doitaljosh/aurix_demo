#include "common.h"
#include "message.h"

uint64_t msgr_rx_bytes;
uint64_t msgr_tx_bytes;
char *txMirror[64];

int IpcTestMessageRouter_DataReceived(uint8_t *DataPtr, uint32_t DataLen);
void MessageRouter_DataReceived(uint8_t *DataPtr, uint32_t DataLen);
int MessageRouter_TransmitData(uint8_t Channel, uint32_t DataLen, uint8_t *DataPtr);
void MessageRouter_L2Mon_L2MonFwdData(uint32_t *DataPtr, uint32_t DataLen);
void MessageRouter_L3Mon_L3MonFwdData(uint32_t *DataPtr, uint32_t DataLen);
void MessageRouter_IpcTest_IpcTestFwdData(uint32_t *DataPtr, uint32_t DataLen);
void MessageRouter_DsCom_DsComFwdData(uint32_t *DataPtr, uint32_t DataLen);
void MessageRouter_EthClient_EthClientFwdData(void);