# Fanshawe Network Chat Application <br>

Term-1 : Assignment to create an Server Client Chat Application  <br>
<br>
Goals of the assignment : <br>

> Create a Chat application using Winsock and TCP
> 
> Implement a Buffer Class that encodes/decodes 32bit unsigned Integers using BIG/LITTLE endian
> 
> The Buffer should also dynamically grow depending on the message size
> [Currently the buffer is set to 16 bytes just to show the growing process]
> 
> Have a custom protocol with Length prefixing
> 
> Multiple (non - Blocking) connection for clients
> 
> Server Messgage replication for User activites (join, leave)
>

<br>
Extra Features Implemented : <br>



> Replicated User Colors and Message colors
> [The server will assign colors to the client making sure no two clients have the same color]
> 
> Max connection limit (any connection after that get feedback msg from the server saying server Full)
> [Currently Max client is set to 10 concurrent clients, that is changable in the code]
> 
> Username Clash check (Two client wont have same username)
>

 ### Installation notes : <br>

> Step 1: Pull the Repository
>
> Step 2: Open the ChatApplication.sln and compile both the Projects [TCPServer](https://github.com/RedBellPepperoni/ChatApplication/blob/main/TCPServer/TCPServer.vcxproj "Server") and [TCPClient](https://github.com/RedBellPepperoni/ChatApplication/blob/main/TCPClient/TCPClient.vcxproj "Client")
>
> Step 3: Go to x64/Debug otr x64/Release depending on build config and run the TCPServer.exe to start the server
>
> Step 4: Run TCPClient.exe to run client and input username.
> 
> Step 5: Run multiple instances of TCPClients to simulate multiple users
<br>



## Multiple clients sending messgaes 
<br>
<br>
<br>

![](https://github.com/RedBellPepperoni/ChatApplication/blob/main/multiclient.png)
 <br>

## Server Side View
<br>
<br>
<br>

![](https://github.com/RedBellPepperoni/ChatApplication/blob/main/Server.png)
<br>

## UserName Clash 
<br>
<br>
<br>

![](https://github.com/RedBellPepperoni/ChatApplication/blob/main/UsernameClash.png)

<br>

# MESSAGE FORMATS
<br>

## Client Message Format

> The client sends Raw messages with length prefixing to the server

<br>


![](https://github.com/RedBellPepperoni/ChatApplication/blob/main/ClientMsg.png)

<br>

## Server Message Format

> The server sends the client color, client username length, client username and the raw messages with length prefixing to the clients

<br>

![](https://github.com/RedBellPepperoni/ChatApplication/blob/main/serverMsg.png)

