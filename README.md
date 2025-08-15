=======================================================================
### NOTA: 
Este codigo esta copiado del repositorio de mis companyeros de 42: https://github.com/SimonIsCoding/ft_irc/tree/main
al cual he eliminado los archivos y funciones de la parte Bonus para poder indexarlo en DEEPWIKI y analizar en rofundidad.
 
<a href="https://deepwiki.com/jocuni-p/test"><img src="https://deepwiki.com/badge.svg" alt="Ask DeepWiki"></a>

=======================================================================

# IRC Server Project
This project implements an IRC (Internet Relay Chat) server using C++98 and Linux system calls. The server handles multiple client connections, channels, and various IRC commands.

## Socket Programming Basics

### What is a Socket?
A socket is an endpoint for communication between two machines over a network. It's a software construct that provides a bidirectional communication channel between processes, either on the same machine or across different machines. In this project, we use TCP sockets which provide:
- Reliable, ordered, and error-checked delivery of data
- Connection-oriented communication
- Stream-based data transfer

### What is epoll?
epoll is a Linux-specific I/O event notification mechanism that efficiently monitors multiple file descriptors for events. It's an improvement over older mechanisms like select() and poll() because:
- It scales better with large numbers of file descriptors
- It uses a more efficient event notification system
- It doesn't require scanning all file descriptors on each call
- It supports edge-triggered and level-triggered modes

The epoll API consists of three main system calls:
1. `epoll_create()`: Creates an epoll instance
2. `epoll_ctl()`: Registers/unregisters file descriptors
3. `epoll_wait()`: Waits for events on registered file descriptors

### Key Socket Functions

1. **socket()**
   - Creates a new socket
   - Usage: `socket(AF_INET, SOCK_STREAM, 0)`
   - Returns a file descriptor for the new socket
   - AF_INET: IPv4 protocol family
   - SOCK_STREAM: TCP socket type

2. **setsockopt()**
   - Sets socket options
   - Usage: `setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))`
   - SO_REUSEADDR: Allows reuse of local addresses
   - Important for server restarts to avoid "Address already in use" errors

3. **bind()**
   - Binds a socket to an address and port
   - Usage: `bind(socket_fd, (struct sockaddr *)&address, sizeof(address))`
   - Associates the socket with a specific port and IP address
   - Returns 0 on success, -1 on error

4. **listen()**
   - Marks the socket as passive (server socket)
   - Usage: `listen(socket_fd, backlog)`
   - backlog: Maximum length of the queue of pending connections
   - Returns 0 on success, -1 on error

5. **epoll_create()**
   - Creates an epoll instance
   - Usage: `epoll_create1(0)`
   - Returns a file descriptor for the epoll instance
   - Used for efficient I/O event monitoring

6. **fcntl()**
   - Manipulates file descriptor properties
   - Usage: `fcntl(fd, F_SETFL, O_NONBLOCK)`
   - Sets socket to non-blocking mode
   - Important for asynchronous I/O operations

7. **accept()**
   - Accepts a new connection
   - Usage: `accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)`
   - Returns a new socket descriptor for the accepted connection
   - Creates a new socket for client communication

8. **epoll_ctl()**
   - Controls an epoll instance
   - Usage: `epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev)`
   - Adds/modifies/removes file descriptors to/from the epoll instance
   - EPOLL_CTL_ADD: Adds a new file descriptor

9. **epoll_wait()**
   - Waits for I/O events
   - Usage: `epoll_wait(epoll_fd, events, MAX_EVENTS, timeout)`
   - Returns the number of ready file descriptors
   - Used for event-driven programming

## Building and Running

1. Clone the repository
2. Run `make` to build the project
3. Start the server: `./ircserv <port> <password>`

Example:
```bash
./ircserv 6667 password123
```

## Connecting to the Server

> [!NOTE]
> For the project, the correction sheets specifies that you can "do some test with the IRC client and nc at the same time". Each time, both syntax are written. But they have to be executed in two diferent windows.

### Using Netcat
1. Connect to the server:
```bash
nc localhost <port>
```

2. Register with the server:
```
PASS <password>
NICK <nickname>
USER <username> <hostname> <servername> :<realname>
```

Example:
```bash
nc localhost 6667
```
```bash
PASS password123
```
```bash
NICK user1
```
```bash
USER user1 0 * :John Doe
```

### Using the Official IRC Client (sic)
1. Install sic:
```bash
sudo apt-get install sic
```

2. Connect to the server:
```bash
sic -h localhost -p <port> -n <nickname> -k <password>
```

Example:
```bash
sic -h localhost -p 6667 -n user1 -k password123
```
> [!NOTE]
> By setting sic that way, you will not have to use PASS ans NICK commands. The next one to enter will be USER.

## IRC Commands

### Basic Commands

#### PASS (Set password to enter in the server - used only once at the beginning)
- **Purpose**: Set your password
- **sic Syntax**: `:PASS <password>`
- **netcat Syntax**: `PASS <password>`
- **Example**:
  ```bash
  # sic
  :PASS john_doe
  
  # netcat
  PASS john_doe
  ```

#### NICK (Set Nickname - used only once at the beginning)
- **Purpose**: Set your nickname
- **sic Syntax**: `:NICK <new_nickname>`
- **netcat Syntax**: `NICK <new_nickname>`
- **Example**:
  ```bash
  # sic
  :NICK john_doe
  
  # netcat
  NICK john_doe
  ```

  #### USER (Set Username - used only once at the beginning)
- **Purpose**: Set your Username
- **sic Syntax**: `:USER <username> <hostname> <servername> :<realname>`
- **netcat Syntax**: `USER <username> <hostname> <servername> :<realname>`
- **Example**:
  ```bash
  # sic
  :USER user1 0 * :John Doe
  
  # netcat
  USER user1 0 * :John Doe
  ```

#### JOIN (Join Channel)
> [!NOTE]
> To join a channel, you can use the '#' or the '&' before the channel's name. But #channel is different from &channel.
- **Purpose**: Join a channel
- **sic Syntax**: `:JOIN &<channel_name>`
- **netcat Syntax**: `JOIN &<channel_name>`
- **Example**:
  ```bash
  # sic
  :JOIN &general
  :JOIN #general
  
  # netcat
  JOIN &general
  JOIN #general
  ```

#### PRIVMSG (Private Message)
- **Purpose**: Send a private message to a user or channel
- **sic Syntax**: `:PRIVMSG <target> <message>`
- **netcat Syntax**: `PRIVMSG <target> <message>`
- **Example**:
  ```bash
  # sic
  :PRIVMSG &general Hello everyone!
  :PRIVMSG john_doe Hi there!
  
  # netcat
  PRIVMSG &general Hello everyone!
  PRIVMSG john_doe Hi there!
  ```

### Channel Management Commands

#### TOPIC (Set Channel Topic)
- **Purpose**: Set or view channel topic
- **sic Syntax**: `:TOPIC &<channel_name> :<new_topic>`
- **netcat Syntax**: `TOPIC &<channel_name> :<new_topic>`
- **Example**:
  ```bash
  # sic
  :TOPIC &general :Welcome to the general chat!
  
  # netcat
  TOPIC &general :Welcome to the general chat!
  ```

#### KICK (Remove User from Channel)
- **Purpose**: Remove a user from a channel
- **sic Syntax**: `:KICK &<channel_name> <nickname> :<reason>`
- **netcat Syntax**: `KICK &<channel_name> <nickname> :<reason>`
- **Example**:
  ```bash
  # sic
  :KICK &general john_doe
  :KICK &general john_doe :Being disruptive
  
  # netcat
  KICK &general john_doe
  KICK &general john_doe :Being disruptive
  ```
> [!NOTE]
> The reason argument is optionnal.

#### INVITE (Invite User to Channel)
- **Purpose**: Invite a user to join a channel
- **sic Syntax**: `:INVITE <nickname> &<channel_name>`
- **netcat Syntax**: `INVITE <nickname> &<channel_name>`
- **Example**:
  ```bash
  # sic
  :INVITE john_doe &general
  
  # netcat
  INVITE john_doe &general
  ```


#### MODE (Set Channel Modes) - Detailed Explanation
- **Purpose**: Set channel modes to control channel behavior and user permissions
- **Available Modes**:
  - `+i` / `-i`: Invite-only mode
    - `+i`: Only users with an invite can join
    - `-i`: Anyone can join (default)
  - `+t` / `-t`: Topic protection
    - `+t`: Only operators can change the topic
    - `-t`: Anyone can change the topic (default)
  - `+k` / `-k`: Channel key (password)
    - `+k <key>`: Set a password to join the channel
    - `-k`: Remove the password
  - `+o` / `-o`: Operator status
    - `+o <nickname>`: Give operator status to a user
    - `-o <nickname>`: Remove operator status from a user
  - `+l` / `-l`: User limit
    - `+l <number>`: Set maximum number of users
    - `-l`: Remove user limit

- **Examples of Multiple Modes**:
  ```bash
  # sic
  :MODE &general +i  # Set invite-only
  :MODE &general +t  # Set topic protection
  :MODE &general +k password123  # Set channel password
  :MODE &general +o john_doe  # Make john_doe an operator
  :MODE &general +l 10  # Set user limit to 10
  
  # Combine multiple modes in one command
  :MODE &general +itk password123  # Set invite-only, topic protection, and password
  :MODE &general +ol john_doe 10  # Make john_doe operator and set user limit
  
  # netcat
  MODE &general +i  # Set invite-only
  MODE &general +t  # Set topic protection
  MODE &general +k password123  # Set channel password
  MODE &general +o john_doe  # Make john_doe an operator
  MODE &general +l 10  # Set user limit to 10
  
  # Combine multiple modes in one command
  MODE &general +itk password123  # Set invite-only, topic protection, and password
  MODE &general +o john_doe +l 10  # Make john_doe operator and set user limit
  ```

> [!NOTE]
> When combining multiple modes in one command:
> - Modes that require parameters (like +k and +l) must be specified in the correct order
> - The parameters must match the order of the modes that require them
> - Example: `MODE &channel +kl password 10` is correct, but `MODE &channel +lk 10 password` is not



## Main Classes and Methods

### Server Class
- **Constructor**: `Server(int port, std::string password)`
  - Initializes the server socket
  - Sets up epoll for event handling
  - Configures socket options

- **Key Methods**:
  - `run()`: Main server loop
  - `handleClientMessage(int client_fd)`: Processes client messages
  - `removeClient(int fd)`: Removes a client from the server
  - `clientLog(int fd, std::string message)`: Logs client activities

### Client Class
- **Constructor**: `Client(int fd)`
  - Initializes a new client connection
  - Sets up client-specific properties

- **Key Methods**:
  - `getFd()`: Returns client's file descriptor
  - `getNickname()`: Returns client's nickname
  - `setNickname(std::string nickname)`: Sets client's nickname
  - `sendMessage(std::string message)`: Sends a message to the client

### Channel Class
- **Constructor**: `Channel(std::string name)`
  - Creates a new channel
  - Initializes channel properties

- **Key Methods**:
  - `addClient(Client* client)`: Adds a client to the channel
  - `removeClient(Client* client)`: Removes a client from the channel
  - `broadcastMessage(std::string message)`: Sends a message to all channel members
  - `getClients()`: Returns list of channel clients


## Dependencies
- C++ compiler with C++98 support
- Linux system (for epoll)
- Make
- sic (for official client testing)
- netcat (for basic testing)

## Security Considerations
- Password protection for server access
- Input validation for all commands
- Proper client disconnection handling
- Memory leak prevention in destructors
