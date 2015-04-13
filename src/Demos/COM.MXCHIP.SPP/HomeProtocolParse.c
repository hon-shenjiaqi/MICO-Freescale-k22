#include "MICOAppDefine.h"
#include "SppProtocol.h"
#include "SocketUtils.h"
#include "debug.h"
#include "MicoPlatform.h"
#include "MICONotificationCenter.h"
#include <stdio.h>
#include <string.h>
#include "SparkLight.h"
#define home_log(M, ...) custom_log("SPP", M, ##__VA_ARGS__)
#define home_log_trace() custom_log_trace("SPP")


/*
*  terminal-->panel
*  1.{lightcfg,distsubnet,distdevid,loop,dimm}
*     if distsubnet=999, distdevid=999  then control local relay
*  2.
*  
*/
const char cmdOpen[] = "open";
const char cmdClose[] = "close";
const char tempAckOpen[] = "open ok!\n";
const char tempAckClose[] = "close ok!\n";
const char tempAckErr[] = "err cmd!\n";
unsigned char sendBuffer[256]={0};

extern void HomeSwitch1Control(bool);
extern void HomeSwitch2Control(bool);


OSStatus HomeWlanCommandProcess(unsigned char *inBuf, int *inBufLen, int inSocketFd, mico_Context_t * const inContext)
{
  home_log_trace();
  (void)inSocketFd;
  (void)inContext;
  OSStatus err = kUnknownErr;

  if(memcmp(inBuf,cmdOpen,strlen(cmdOpen))==0){

	memset(sendBuffer,0,256);
	LightCfgPackage(SRC_SUBNET_ID, SRC_DEV_ID, 1, 17, 1,100);
	//err = MicoUartSend(UART_FOR_APP,tempAckOpen, strlen(tempAckOpen));
	//add by jacky for smarthome
	HomeSwitch1Control(TRUE);
	
  }else if(memcmp(inBuf,cmdClose,strlen(cmdClose))==0){

	memset(sendBuffer,0,256);
	LightCfgPackage(SRC_SUBNET_ID, SRC_DEV_ID, 1, 17, 1,0);
	//err = MicoUartSend(UART_FOR_APP, tempAckClose, strlen(tempAckClose));
	//add by jacky for smarthome
	HomeSwitch1Control(FALSE);
  }
  else{
  	err = MicoUartSend(UART_FOR_APP, tempAckErr, strlen(tempAckErr));
  }
  
  *inBufLen = 0;
  return err;
}
