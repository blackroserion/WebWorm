// 2016年 12月 14日 星期三 14:38:02 CST
// Author:ly
//程序功能 根据命名行参数2提供的网址，进行dns解析
  
#include<stdio.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<stdlib.h>

int main(int argc, char* argv[]){
    if(argc<2){
        printf("主机域名is needed\n");
        exit(EXIT_FAILURE);
    }   
    struct hostent* host=gethostbyname(argv[1]);
    if(NULL==host){
        perror("gethostbyname"),exit(-2);
    }   
    if(host->h_addrtype==AF_INET){
        printf("主机官方名称: %s\n",host->h_name);
        printf("别名表: \n");
        char**ppaaliase=host->h_aliases;
        while(*ppaaliase){
            printf("        %s\n",*ppaaliase++);
        }
        struct in_addr **ppaddrlist=(struct in_addr**)host->h_addr_list;
        while(*ppaddrlist){
            printf("%s\n",inet_ntoa(**ppaddrlist++));
            //printf("%s\n",*host->h_addr_list++);
        }
    }   
    return 0;
}
