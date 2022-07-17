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
