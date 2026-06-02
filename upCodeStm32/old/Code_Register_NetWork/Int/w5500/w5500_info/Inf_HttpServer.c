#include "Inf_HttpServer.h"
static uint8_t txBuff[2048] = {0};
static uint8_t rxBuff[2048] = {0};
static uint8_t socketCount = 1;
static uint8_t socketList[] = {0};

static uint8_t *content_name = "index.html";
/* 响应的网页的内容 */
uint8_t content[2048] = "<!doctype html>\n"
                        "<html lang=\"en\">\n"
                        "<head>\n"
                        "<meta charset=\"GBK\">\n"
                        "<meta name=\"viewport\"\n"
                        "content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\">\n"
                        "<meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\n"
                        "<title>嵌入式课程</title>\n"
                        "\n"
                        "<style type=\"text/css\">\n"
                        "#open_red{\n"
                        "color: red;\n"
                        "width: 100px;\n"
                        "height: 40px;\n"
                        "}\n"
                        "#close_red{\n"
                        "color: black;\n"
                        "width: 100px;\n"
                        "height: 40px;\n"
                        "}\n"
                        "</style>\n"
                        "</head>\n"
                        "<body>\n"
                        "<a href=\"/index.html?action=1\"><button id=\"open_red\" >开灯</button></a>\n"
                        "<a href=\"/index.html?action=2\"><button id=\"close_red\" >关灯</button></a>\n"
                        "</body>\n"
                        "</html>";
void WebServer_Init(void)
{
    /*
    *1. 初始化 http 服务器
    *参数1：发送缓冲区
    *参数2：接收缓冲区
    *参数3：使用的socket的个数
    *参数4：使用的socket列表
    */
    httpServer_init(txBuff, rxBuff, socketCount, socketList);
    /* 2. 注册http服务器要响应的内容(网页内容) */
    reg_httpServer_webContent(content_name, content);
}  

/**
* @description: 启动web服务器
* @return {*}
*/
void WebServer_Start(void)
{
    while(1)
    {
        httpServer_run(0);
    }
}

/**
* @description: http://192.168.0.98/index.html?action=2
*解析url中action参数的值
* @param {uint8_t} *url
*/
uint8_t parse_url_action(uint8_t *url)
{
   uint8_t *params = (uint8_t *)strstr((char *)url, "action=");
   if (params == NULL)
   {
     return '0'; // 没有找到 action
   }
   else{
     return (uint8_t)(*(params + 7));
   }
}

/**
* @description: 根据传入的不同参数执行开灯or关灯
* @param {st_http_request} *parsed_http_request
* @return {*}
*/
void handle_user_function(uint8_t *url)
{
    uint8_t action = parse_url_action(url);
    if (action == '1')
    {
        Dri_Led_On(LED2_BLUE);
    }
    else if (action == '2')
    {
        Dri_Led_Off(LED2_BLUE);
    }
}