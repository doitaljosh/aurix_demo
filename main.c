#include "common.h"
#include "eth_client.h"
#include "message_router.h"

int Aurix_IPC_Eth_Test(void)
{
	int retval = 0;

	eth_hdl = socket(2, 2, 0x11);
	aurix_ip = strdup("10.42.0.73");
	
	pthread_t ethclient_thread;
	
	if (eth_hdl == -1) {
		printf("Error opening socket\r\n");
		ethclient_init_success = 0;
	} else {
		memset(&si_other, 0, 0x10);
		si_other.sin_family = 2; // UDP
		si_other.sin_port = htons(50000);
		retval = inet_aton(aurix_ip, (struct in_addr *)&si_other.sin_addr);
		
		if (retval == 0) {
			printf("Error: Wrong address for Aurix.");
			exit(-1);
		}
		
		memset(&si_myaddr, 0, 0x10);
		si_myaddr.sin_family = 2;
		inet_aton("0.0.0.0", (struct in_addr *)&si_myaddr.sin_addr);
		si_myaddr.sin_port = htons(0xebeb);
		retval = bind(eth_hdl, (struct sockaddr *)&si_myaddr, 0x10);
		
		if (retval < 0) {
			printf("Error: Socket binding failed.");
			ethclient_init_success = 0;
			exit(-1);
		} else {
			ethclient_init_success = 1;
		}
		
		pthread_create(&ethclient_thread, NULL, EthReceiveThread, NULL);	
		pthread_join(ethclient_thread, NULL);	
	}
}

int main()
{
    printf("Starting IPC test");
    Aurix_IPC_Eth_Test();
}