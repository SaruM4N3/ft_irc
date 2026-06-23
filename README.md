*This project has been created as part of the 42 curriculum by erbuffet, vaamonch and zsonie.*

# ft_irc — Internet Relay Chat Server

## Description

**ft_irc** is a fully functional IRC server implemented in C++98. The goal of this project is to understand and apply the IRC protocol by building a server that real IRC clients can connect to and interact with.

The server supports multiple simultaneous clients, channel management, operator privileges, and the core IRC commands expected from any standard IRC server. It does **not** implement server-to-server communication, nor does it include an IRC client.

## Instructions

### Requirements

- A C++98-compatible compiler (`c++`)
- A Unix-based OS (Linux or macOS)

### Compilation

```bash
make
```

This will produce the `ircserv` executable. Other available Makefile rules: `all`, `clean`, `fclean`, `re`.

### Execution

```bash
./ircserv <port> <password>
```

- `<port>`: the port number the server will listen on for incoming IRC connections
- `<password>`: the connection password required by any IRC client connecting to the server

### Example

```bash
./ircserv 4242 mypassword
```

Then connect with your IRC client of choice (e.g., irssi, WeeChat, HexChat) to `127.0.0.1` on port `4242` using the password `mypassword`.

### Testing with nc

You can verify partial data handling using `nc`:

```bash
nc -C 127.0.0.1 4242
```

Type your command in parts using `ctrl+D` to flush each chunk (e.g., `com`, then `man`, then `d\n`). The server must correctly aggregate partial packets before processing a command.

You can also use `telnet`:
```bash
telnet localhost 4242
```


## Features

### Mandatory

- Authentication with a server password
- Nickname and username registration
- Channel joining and messaging
- Private messaging between users
- Operator and regular user roles
- Non-blocking I/O with a single `epoll()` call
- TCP/IP communication

### Channel operator commands

| Command  | Description                                      |
|----------|--------------------------------------------------|
| `KICK`   | Eject a client from a channel                    |
| `INVITE` | Invite a client to a channel                     |
| `TOPIC`  | View or change the channel topic                 |
| `MODE`   | Change channel modes (see below)                 |

### Supported MODE flags

| Flag | Effect                                              |
|------|-----------------------------------------------------|
| `i`  | Set/remove invite-only restriction                  |
| `t`  | Restrict TOPIC changes to operators only            |
| `k`  | Set/remove a channel key (password)                 |
| `o`  | Grant/revoke channel operator privilege             |
| `l`  | Set/remove a user limit for the channel             |

### Bonus

| Command  | Description                                      |
|----------|--------------------------------------------------|
| `PART`   | Part from a channel                              |
| `QUIT`   | Disconnect from the server                       |
| `PING`   | Ping another user			                      |


- File transfer (DCC)
- IRC bot

## Resources

### IRC Protocol & References

- [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2810 — IRC Architecture](https://datatracker.ietf.org/doc/html/rfc2810)
- [RFC 2811 — IRC Channel Management](https://datatracker.ietf.org/doc/html/rfc2811)
- [RFC 2812 — IRC Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
- [RFC 2813 — IRC Server Protocol](https://datatracker.ietf.org/doc/html/rfc2813)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [Modern IRC documentation (ircdocs.horse)](https://modern.ircdocs.horse/)

### AI Usage

AI tools (such as Claude) were used during this project for the following tasks:

- Generating a IRC numerics code table
- Helping to implement a dispatch table for IRC commands
- Code review
- Helping draft and format this README

All AI-generated content was reviewed, adapted manually, tested, and validated by the team before inclusion. No code was blindly copy-pasted: every function and logic block was understood, discussed with peers, and verified through testing.
