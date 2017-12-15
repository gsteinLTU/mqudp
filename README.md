# mqudp
Send/Receive POSIX message queue over UDP

#### Usage:

  On the computer with the queue you wish to send over the network:
  
    mqudp transmit <queue name> <message size> <client> <client port>
    
  On the comptuer you wish to receive the queue on:
  
    mqudp receive <queue name> <message size> <host> <host port>
