#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h> 

#define MSG_MAX_COUNT 1000

// Outputs usage instructions, then exits
void printUsage()
{
    puts("Usage:\n");
    puts("\tmqudp transmit <queue name> <message size> <client> <client port>\n");
    puts("\tmqudp receive <queue name> <message size> <host> <host port>\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    // Check arguments    
    if(argc != 6)
      printUsage();

    if(strcmp("transmit", argv[1]) != 0 && strcmp("receive", argv[1]) != 0)
      printUsage();
    
    // Get arguments
    int msg_size = atoi(argv[3]);
    int port = atoi(argv[5]);
    char * qname = argv[2];
    char * host = argv[4];
    if(msg_size < 1 || port < 1)
      printUsage();
    
    char transmit = strcmp("transmit", argv[1]) == 0;


    // Create buffer for mqueue
    unsigned char *buffer = (unsigned char *) malloc(msg_size + 1);
    if (buffer == NULL) {
      perror("Failed to allocate memory");
      return -1;
    }
    
    struct mq_attr ma;          // message queue attributes
    // Specify message queue attributes.
    ma.mq_maxmsg = MSG_MAX_COUNT;       // maximum number of messages allowed in queue
    ma.mq_msgsize = msg_size;   // messages are contents of size...
    ma.mq_flags = 0;            // blocking read/write
    ma.mq_curmsgs = 0;          // number of messages currently in queue

    mqd_t mq;                   // message queue
    
    // Create the message queue with some default settings.
    mq = mq_open(qname, O_RDONLY | O_NONBLOCK, 0700, &ma);

    if (mq == -1) {
      perror("Failed to open input queue\n");
      return 1; 
    }

    // Open socket
    int fd;
    if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) { 
      perror("Failed to create socket"); 
      return 1; 
    }
        
    struct sockaddr_in addr = {0};
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    
    if (inet_aton(host, &addr.sin_addr) == 0) 
    {
        perror("Failed to convert address");
        return 1;
    }
    
    if(transmit)
    {
      if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) { 
        perror("Failed to bind socket");
        return 1;
      }
      
      // Read from queue
      while (1) {
          int status = mq_receive(mq, (char *) (buffer), msg_size, NULL);
          if (status != -1) {
              int slen = sizeof(addr);
              if (sendto(fd, buffer, msg_size, 0 , (struct sockaddr *) &addr, slen) == -1)
              {
                perror("Failed to transmit");
                return 1;
              }
          }
      }

    }
    
    return 0;
}
