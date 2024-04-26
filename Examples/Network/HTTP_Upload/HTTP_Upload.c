/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2024 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Upload.c
 * Purpose: HTTP File upload example
 *----------------------------------------------------------------------------*/

#include <stdio.h>

#include "main.h"

#include "cmsis_os2.h"                  // ::CMSIS:RTOS2

#include "rl_net.h"                     // Keil::Network&MDK:CORE
#include "rl_fs.h"                      // Keil::File System&MDK:CORE

/* IP address change notification */
void netDHCP_Notify (uint32_t if_id, uint8_t option, const uint8_t *val, uint32_t len) {
  char ip_ascii[16];
  (void)len;

  if ((if_id == (NET_IF_CLASS_ETH | 0)) && (option == NET_DHCP_OPTION_IP_ADDRESS)) {
    netIP_ntoa (NET_ADDR_IP4, val, ip_ascii, sizeof(ip_ascii));
    printf("IP4: %s\n",ip_ascii);
  }
}

/*-----------------------------------------------------------------------------
  Application Main Thread 'app_main_thread': Run Network
 *----------------------------------------------------------------------------*/
__NO_RETURN void app_main_thread (void *argument) {
  uint8_t ip_addr[NET_ADDR_IP6_LEN];
  char    ip_ascii[40];

  (void)argument;

  printf("Network HTTP Upload example\n");

  if (finit("M0:") == fsOK) {
    fmount("M0:");
  }
  netInitialize();

  printf("IP4: Waiting for DHCP\n");
  if (netIF_GetOption(NET_IF_CLASS_ETH | 0,
                      netIF_OptionIP6_LinkLocalAddress,
                      ip_addr, sizeof(ip_addr)) == netOK) {
    /* IPv6 enabled on ETH0, print Link-local address */
    netIP_ntoa(NET_ADDR_IP6, ip_addr, ip_ascii, sizeof(ip_ascii));
    printf("IP6: %s\n", ip_ascii);
  }
  osThreadExit();
}

/*-----------------------------------------------------------------------------
 *        Application main function
 *----------------------------------------------------------------------------*/
int app_main (void) {
  osKernelInitialize();
  osThreadNew(app_main_thread, NULL, NULL);
  osKernelStart();
  return 0;
}
