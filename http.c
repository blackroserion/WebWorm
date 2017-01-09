// 2016年 12月 14日 星期三 15:12:17 CST
// Author:ly
//程序功能:dns程序将解析后的IP地址，传递给http程序，然后，http程序向服务器发送http格式的请求，
//        接收服务器返回的数据，将这些数据输出到doc文件中。
//版本号：0.0.0

#include<sys/socket.h>  
#include<netinet/in.h>
#include<arpa/inet.h>
#include<strings.h>//uc
#include<stdio.h>
#include<stdlib.h>
#include<string.h>//标C
#include<unistd.h>

int main(int argc,char *argv[]){
    if(argc<3){
        printf("主机IP地址,主机域名[%s]\n",argv[1]);
        exit(-1);
    }   

    char const* ip=argv[1];
    char const* domain=argv[2];
    char const* path=argc<4?"":argv[3];
   
    //套接字
    int sockfd=socket(PF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("socket"),exit(-1);
    }   

    //准备通信地址
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(80);

    //字节序转换
    if(!inet_aton(ip,&addr.sin_addr)){
        perror("inet_aton"),exit(-2);
    }   

    //连接
       if(connect(sockfd,(struct sockaddr*)&addr,sizeof(addr))==-1){
        perror("connect"),exit(-3);
    }
    char request[1024];
    sprintf(request,"GET /%s HTTP/1.0\r\nHost: %s\r\nAccept: */*Connection: Keep-Alive\r\nUser-Agent: Mozilla/5.0\r\nRefer: %s\r\n\r\n",path,domain,domain);
    if(send(sockfd,request,strlen(request),0)==-1){
        perror("send"),exit(-4);
    }
    char respond[1024] ={};

    FILE* pf=fopen("doc","a");
    if(!pf){
        perror("File Open "),exit(-2);
    }
    for(;;){
       size_t rlen=recv(sockfd,respond,sizeof(respond)-1,0);
       if(rlen<0){
           perror("recv"),exit(-5);
       }
       if(!rlen){
           break;
       }
       fprintf(pf,"%s",respond);
    }
    close(sockfd);
    if(-1==fclose(pf)){
        perror("Close File "),exit(-1);
    }

    return 0;
}
