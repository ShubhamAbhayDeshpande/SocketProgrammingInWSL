# C++ Network Programming Cheatsheet

## 1. Basic Program Structure

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
```

---

# 2. Common Header Files

## General C++

```cpp
#include <iostream>     // Input/output
#include <string>       // std::string
#include <vector>       // std::vector
#include <array>        // std::array
#include <cstring>      // memset, memcpy
#include <cstdlib>      // atoi, exit
#include <cstdio>       // printf
#include <cstdint>      // uint32_t etc.
```

## POSIX / Linux Networking

```cpp
#include <unistd.h>         // close()
#include <arpa/inet.h>      // inet_pton(), htons()
#include <netinet/in.h>     // sockaddr_in
#include <sys/socket.h>     // socket APIs
#include <sys/types.h>
#include <netdb.h>          // getaddrinfo()
#include <fcntl.h>          // non-blocking sockets
#include <poll.h>           // poll()
#include <sys/select.h>     // select()
```

---

# 3. Variables and Data Types

```cpp
int x = 5;
float pi = 3.14f;
double bigPi = 3.1415926535;
char letter = 'A';
bool flag = true;
std::string name = "Alice";
```

## Fixed Width Integers

```cpp
uint8_t a;
uint16_t b;
uint32_t c;
uint64_t d;
```

Useful in networking because packet fields have exact sizes.

---

# 4. Constants

```cpp
const int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;
```

---

# 5. Arrays

## C-style Array

```cpp
int arr[5] = {1, 2, 3, 4, 5};
```

## Character Buffer

```cpp
char buffer[1024];
```

## std::array

```cpp
std::array<int, 5> arr = {1,2,3,4,5};
```

---

# 6. Strings

## std::string

```cpp
std::string msg = "Hello";
msg += " World";
```

## C-style Strings

```cpp
char msg[] = "Hello";
```

## String to Integer

```cpp
int port = std::stoi("8080");
```

---

# 7. Pointers

## Pointer Basics

```cpp
int x = 10;
int* ptr = &x;

std::cout << *ptr << std::endl; // Dereference
```

## Null Pointer

```cpp
int* ptr = nullptr;
```

## Pointer to Array

```cpp
char buffer[1024];
char* ptr = buffer;
```

## Dynamic Allocation

```cpp
int* data = new int;
*data = 42;

delete data;
```

## Dynamic Arrays

```cpp
char* buffer = new char[1024];

delete[] buffer;
```

---

# 8. References

```cpp
int x = 5;
int& ref = x;

ref = 10;
```

Useful for function parameters.

---

# 9. Structures

## Define a Struct

```cpp
struct Packet {
    int id;
    char data[256];
};
```

## Using a Struct

```cpp
Packet p;
p.id = 1;
```

## Pointer to Struct

```cpp
Packet* ptr = &p;
ptr->id = 2;
```

---

# 10. Classes

```cpp
class Client {
public:
    int socketFd;

    void connect() {
        std::cout << "Connecting..." << std::endl;
    }
};
```

---

# 11. Functions

## Basic Function

```cpp
int add(int a, int b) {
    return a + b;
}
```

## Pass by Reference

```cpp
void increment(int& x) {
    x++;
}
```

## Pass Pointer

```cpp
void clearBuffer(char* buffer, int size) {
    memset(buffer, 0, size);
}
```

---

# 12. Loops

## For Loop

```cpp
for (int i = 0; i < 10; i++) {
    std::cout << i << std::endl;
}
```

## While Loop

```cpp
while (true) {
    // infinite loop
}
```

---

# 13. Conditional Statements

```cpp
if (x > 0) {
    std::cout << "Positive";
} else {
    std::cout << "Non-positive";
}
```

---

# 14. Memory Functions

## memset

```cpp
char buffer[1024];
memset(buffer, 0, sizeof(buffer));
```

## memcpy

```cpp
memcpy(dest, src, size);
```

---

# 15. Important Networking Concepts

## Endianness

Network byte order is Big Endian.

## Host to Network Conversion

```cpp
htons(port);      // host to network short
htonl(value);     // host to network long
ntohs(port);
ntohl(value);
```

---

# 16. Socket Basics

## Create Socket

```cpp
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
```

### Parameters

- AF_INET → IPv4
- SOCK_STREAM → TCP
- SOCK_DGRAM → UDP

---

# 17. sockaddr_in Structure

```cpp
sockaddr_in serverAddr;

serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(8080);
inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
```

## Clear Structure First

```cpp
memset(&serverAddr, 0, sizeof(serverAddr));
```

---

# 18. TCP Server Example

```cpp
int serverFd = socket(AF_INET, SOCK_STREAM, 0);

sockaddr_in addr;
memset(&addr, 0, sizeof(addr));

addr.sin_family = AF_INET;
addr.sin_port = htons(8080);
addr.sin_addr.s_addr = INADDR_ANY;

bind(serverFd, (sockaddr*)&addr, sizeof(addr));
listen(serverFd, 5);

int clientFd = accept(serverFd, nullptr, nullptr);
```

---

# 19. TCP Client Example

```cpp
int sockfd = socket(AF_INET, SOCK_STREAM, 0);

sockaddr_in serverAddr;
memset(&serverAddr, 0, sizeof(serverAddr));

serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(8080);
inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr));
```

---

# 20. Sending and Receiving Data

## send()

```cpp
send(sockfd, message, strlen(message), 0);
```

## recv()

```cpp
char buffer[1024];
int bytes = recv(sockfd, buffer, sizeof(buffer), 0);
```

---

# 21. UDP Example

## Create UDP Socket

```cpp
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
```

## Send UDP Packet

```cpp
sendto(sockfd,
       message,
       strlen(message),
       0,
       (sockaddr*)&destAddr,
       sizeof(destAddr));
```

## Receive UDP Packet

```cpp
recvfrom(sockfd,
         buffer,
         sizeof(buffer),
         0,
         (sockaddr*)&srcAddr,
         &addrLen);
```

---

# 22. Closing Sockets

```cpp
close(sockfd);
```

---

# 23. Error Handling

## perror

```cpp
if (sockfd < 0) {
    perror("Socket creation failed");
}
```

---

# 24. Common Socket Functions

| Function | Purpose |
|---|---|
| socket() | Create socket |
| bind() | Bind address |
| listen() | Listen for clients |
| accept() | Accept connection |
| connect() | Connect to server |
| send() | Send TCP data |
| recv() | Receive TCP data |
| sendto() | Send UDP data |
| recvfrom() | Receive UDP data |
| close() | Close socket |

---

# 25. select() Multiplexing

```cpp
fd_set readfds;
FD_ZERO(&readfds);
FD_SET(sockfd, &readfds);

select(sockfd + 1, &readfds, nullptr, nullptr, nullptr);
```

Useful for handling multiple clients.

---

# 26. Non-blocking Sockets

```cpp
fcntl(sockfd, F_SETFL, O_NONBLOCK);
```

---

# 27. Useful Macros

```cpp
#define BUFFER_SIZE 1024
#define PORT 8080
```

---

# 28. Enums

```cpp
enum MessageType {
    CONNECT,
    DISCONNECT,
    DATA
};
```

---

# 29. Typedef / Using

```cpp
typedef unsigned char byte;

using byte = unsigned char;
```

---

# 30. Vector

```cpp
std::vector<char> buffer(1024);
```

---

# 31. Smart Pointers

## unique_ptr

```cpp
#include <memory>

std::unique_ptr<int> ptr = std::make_unique<int>(5);
```

Preferred over raw pointers in modern C++.

---

# 32. Threads

```cpp
#include <thread>

void worker() {
    std::cout << "Thread running";
}

std::thread t(worker);
t.join();
```

Useful for concurrent clients.

---

# 33. Mutex

```cpp
#include <mutex>

std::mutex mtx;

mtx.lock();
mtx.unlock();
```

---

# 34. Common Compile Command

```bash
g++ main.cpp -o app
```

## With Threads

```bash
g++ main.cpp -pthread -o app
```

---

# 35. Debugging Tips

## Print Errors

```cpp
std::cerr << "Error" << std::endl;
```

## Use gdb

```bash
gdb ./app
```

---

# 36. Common Networking Constants

| Constant | Meaning |
|---|---|
| AF_INET | IPv4 |
| AF_INET6 | IPv6 |
| SOCK_STREAM | TCP |
| SOCK_DGRAM | UDP |
| INADDR_ANY | Any interface |

---

# 37. Common Mistakes

- Forgetting htons() / ntohs()
- Not clearing sockaddr_in with memset()
- Buffer overflow
- Forgetting close()
- Dereferencing nullptr
- Memory leaks with new/delete
- Assuming recv() receives all bytes at once

---

# 38. recv() and send() Important Note

TCP is a stream protocol.

```cpp
recv() != one complete message
```

You may need loops and message framing.

---

# 39. Packing Structures

Useful for protocol headers.

```cpp
#pragma pack(push, 1)
struct Header {
    uint16_t type;
    uint32_t length;
};
#pragma pack(pop)
```

---

# 40. Example Packet Header

```cpp
struct PacketHeader {
    uint16_t opcode;
    uint32_t payloadSize;
};
```

---

# 41. Basic Client Loop

```cpp
while (true) {
    char buffer[1024];

    int bytes = recv(sockfd, buffer, sizeof(buffer), 0);

    if (bytes <= 0) {
        break;
    }
}
```

---

# 42. Useful Commands

## Check Open Ports

```bash
netstat -tulnp
```

## Test Server

```bash
telnet 127.0.0.1 8080
```

## Test with netcat

```bash
nc 127.0.0.1 8080
```

---

# 43. IPv6 Example

```cpp
sockaddr_in6 addr6;
addr6.sin6_family = AF_INET6;
```

---

# 44. getaddrinfo()

Preferred modern address resolver.

```cpp
addrinfo hints{}, *res;

hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;

getaddrinfo("localhost", "8080", &hints, &res);
```

---

# 45. RAII Concept

Resource Acquisition Is Initialization.

Modern C++ uses objects to automatically release resources.

Example:

```cpp
std::vector<int> data;
```

No manual delete needed.

---

# 46. Recommended Modern C++ Practices

- Prefer std::string over char*
- Prefer std::vector over raw arrays
- Prefer smart pointers over raw pointers
- Use nullptr instead of NULL
- Use constexpr when possible
- Avoid global variables

---

# 47. Mini TCP Echo Server Skeleton

```cpp
int main() {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(serverFd, (sockaddr*)&addr, sizeof(addr));
    listen(serverFd, 5);

    while (true) {
        int clientFd = accept(serverFd, nullptr, nullptr);

        char buffer[1024];
        int bytes = recv(clientFd, buffer, sizeof(buffer), 0);

        send(clientFd, buffer, bytes, 0);

        close(clientFd);
    }
}
```

---

# 48. Quick Pointer Cheat Table

| Syntax | Meaning |
|---|---|
| int* p | Pointer to int |
| &x | Address of x |
| *p | Value pointed to by p |
| p->x | Access struct member through pointer |
| nullptr | Null pointer |

---

# 49. Quick Socket Workflow

## Server

```text
socket → bind → listen → accept → recv/send → close
```

## Client

```text
socket → connect → send/recv → close
```

---

# 50. Final Exam Tips

- Memorize sockaddr_in
- Understand pointers deeply
- Practice recv/send loops
- Know TCP vs UDP differences
- Understand byte order conversion
- Practice small client/server programs
- Learn memory ownership rules
- Always check return values

