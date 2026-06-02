#ifndef __INTFACE_W5500_HTTPSERVER_H__
#define __INTFACE_W5500_HTTPSERVER_H_
#include "w5500.h"
#include "socket.h"
#include "stdio.h"
#include "httpserver.h"
#include "httpParser.h"
#include "Driver_Led.h"
void HttpServer_Init(void);
void HttpServer_Start(void);
#endif

