int watalab_tcp_listen(int port)
{
  int sock;
  struct sockaddr_in addr;
  int yes = 1;
  int ret;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) {
    perror("socket");
    exit(-1);
  }
  
  bzero((char *) &addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  if(ret < 0) {
    perror("bind");
    exit(-1);
  }
  ret = listen(sock, 1024);
  if(ret < 0) {
    perror("listen");
    close(sock);
    exit(-1);
  }
  return sock;
}

int watalab_accept(int sock_listen){
  struct sockaddr addr;
  int len;
  int ret;
  len = sizeof(struct sockaddr);
  ret = accept(sock_listen, &addr, (socklen_t*) &len);
  return ret;
}


int main(int argc, char** argv)
{
  int sock_listen;
  int sock_client;
  int ret;
  char buf[BUFSIZE];
  int wfd;
  
  if(argc !=2){
   printf("usage: %s [output filename]\r\n", argv[0]);
   return -1;
  }
  
  sock_listen = watalab_tcp_listen(11111);
  sock_client = watalab_accept(sock_listen);
  
  wfd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNK, S_IRWXU);
  
  ret = recv(sock_client, buf, BUFSIZE, 0);
  while(ret > 0){
    write(wfd, buf, ret);
    ret = recv(sock_client, buf, BUFSIZE, 0);
  }
  
  close(sock_client);
  close(sock_listen);
  
  return 0;
}
