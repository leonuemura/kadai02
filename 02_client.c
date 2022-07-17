int watalab_tcp_connect(const char *hostname, int port)
{
  int sock;
  int ret;
  struct sockaddr_in addr;
  struct hostent *host;
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  addr.sin_family = AF_INET;
  host = gethostbyname(hostname);
  addr.sin_addr = *(struct in_addr *)(host->h_addr_list[0]);
  addr.sin_port = htons(port);
  ret = connect(sock, (struct sockaddr *)&addr, sizeof addr);
  if(ret < 0){
    return -1;
  }else{
    return sock;
  }
}

int main(int argc, char** argv)
{
  int sock;
  int rfd;
  char buf[BUFSIZE];
  int ret;
  
  if(argc !=3){
   printf("usage: %s [ip address][input filename]\r\n", argv[0]);
   exit(-1);
  }
  
  sock = watalab_tcp_connect(argv[1], 11111);
  rfd = open(argv[2], O_RDONLY);
  
  ret = read(rfd, buf, BUFSIZE);
  while(ret > 0){
    send(sock, buf, ret, 0);
    ret = read(rfd, buf, BUFSIZE);
  }
  
  close(rfd);
  close(sock);
  
  return 0;
}
