# Computer-Network-Project
This is the Hello message, TCP and UDP connection implementation for the final Computer Networks project at Ontario Tech University, year 2023.  
**Prepared and Presented by: Jacky Lee, Nathanael Selvaraj and Susan Ahmadi.**  
# How to run:  
Assuming VM1 has IP address of 192.168.0.10 and VM2 has IP address of 192.168.0.20 and port is 8000.  
Assuming this is being run on virtual box and the 2 computers are capable of bridge adapter connections. 
1. Clone the repository on VM1 and VM2.  
2. Assuming VM1 is your server and VM2 is your client, delete all files contianing "client" from VM1 and all files containing "server" from VM2  
3. For the Hello code, on VM1 type ```gcc HelloServer.c -o HelloServer``` and on VM2 type ```gcc HelloClient.c -o HelloClient```.  
4. On VM1 type ```./HelloServer 8000``` and on VM2 type ```./HelloClient 192.168.0.10 8000```.  
5. For TCP file transfer, on VM1 type ```gcc part3server.c -o part3server``` and on VM2 type ```gcc part3client.c -o part3client```.  
6. On VM1 type ```./part3server 8000``` and on VM2 type ```./part3client 192.168.0.10 8000```.  
7. For UDP file transfer, on VM1 type ```gcc time_server.c -o time_server``` and on VM2 type ```gcc time_client.c -o time_client```.  
8. On VM1 type ```./time_server 8000``` and on VM2 type ```./time_client 192.168.0.10 8000```.  
