# ft_irc

![1337 Badge](https://img.shields.io/badge/1337-ft__irc-00babc)
![C++ Badge](https://img.shields.io/badge/Language-C%2B%2B-blue)
![IRC Badge](https://img.shields.io/badge/Protocol-IRC-orange)

## 📚 Overview

ft_irc is an advanced networking project at 1337 School that challenges students to create their own Internet Relay Chat (IRC) server from scratch in C++. The project implements a subset of the IRC protocol, allowing real IRC clients to connect and communicate with the server.

## 🔍 Description

The goal is to develop a fully functional IRC server that follows the RFC 1459 (Internet Relay Chat Protocol) and can handle multiple clients simultaneously. The server supports essential IRC features such as channels, private messages, channel operators, and various IRC commands.

## 🛠️ Implementation

This IRC server is implemented in C++98 and uses:
- Non-blocking I/O multiplexing with `poll()` or `select()`
- Socket programming for network communications
- Custom command parsing and handling
- Robust error handling and client management

## ✨ Features

- **User Registration**: Nickname, username, and real name registration
- **Channels**: Create, join, and leave channels
- **Messaging**: Private messages between users and channel messaging
- **Operator Commands**: Kick, invite, topic management, and mode changes
- **Network Commands**: PING/PONG for connection maintenance

## 🚀 How to Use

### Prerequisites

- C++ compiler (g++ or clang++)
- Make
- Linux/Unix environment (MacOS supported)

### Compilation

To compile the server:

```bash
make
```

To clean the object files:

```bash
make clean
```

To clean everything (object files and executable):

```bash
make fclean
```

To recompile everything:

```bash
make re
```

### Running the Server

```bash
./ircserv <port> <password>
```

Parameters:
- `port`: The port number on which your IRC server will be listening
- `password`: The connection password required for authentication

Example:
```bash
./ircserv 6667 secretpassword
```

### Connecting with a Client

You can connect to your IRC server using any standard IRC client such as:
- irssi
- WeeChat
- HexChat
- mIRC

Example with irssi:
```bash
irssi -c localhost -p 6667 -w secretpassword
```

## 📋 Supported IRC Commands

| Command | Description |
|---------|-------------|
| PASS | Set a connection password |
| NICK | Set or change your nickname |
| USER | Specify username and real name |
| JOIN | Join a channel |
| PART | Leave a channel |
| PRIVMSG | Send private messages to users or channels |
| NOTICE | Send notices to users or channels |
| KICK | Remove a user from a channel |
| INVITE | Invite a user to a channel |
| TOPIC | Set or view the channel topic |
| MODE | Change channel or user modes |
| QUIT | Disconnect from the server |
| PING/PONG | Server connection checking |
| WHO | List users in a channel |
| WHOIS | Display information about a user |

## 🧩 Project Structure

```
.
├── Makefile
├── includes/
│   ├── Server.hpp
│   ├── Client.hpp
│   ├── Channel.hpp
│   ├── Command.hpp
│   └── ...
├── srcs/
│   ├── main.cpp
│   ├── Server.cpp
│   ├── Client.cpp
│   ├── Channel.cpp
│   ├── commands/
│   │   ├── join.cpp
│   │   ├── nick.cpp
│   │   ├── privmsg.cpp
│   │   └── ...
│   └── utils/
│	   ├── Parser.cpp
│	   └── ...
└── README.md
```

## 🔬 Key Learning Objectives

- Network programming and socket management
- Implementing communication protocols
- Non-blocking I/O and concurrency in C++
- Object-oriented design for complex systems
- Error handling and robust system design

## 📝 Technical Constraints

- The server must be written in C++98
- No external libraries except the standard C++ library
- Non-blocking I/O operations with a single poll/select
- No forking or threading allowed
- The code must comply with the 1337/42 Norm

## 🧪 Testing

You can test your IRC server using:
- Real IRC clients (irssi, WeeChat, etc.)
- Multiple simultaneous connections
- Edge cases like disconnections, wrong commands, etc.

## 👤 Authors

- Oussama Zahdi (@Oussamazahdi)
- Kemal El Alami (@KamalElAlami)
- Soufiane Arif (@Soufiixarif)

## 💬 Acknowledgements

- 1337 School for providing this challenging project
- RFC 1459 and subsequent RFCs for the IRC protocol specification
- All fellow students who provided feedback and testing