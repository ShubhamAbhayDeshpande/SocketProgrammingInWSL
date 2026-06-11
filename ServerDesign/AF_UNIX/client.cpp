/*
This is the client side functionality of the process. 

It has system calls such as connect() which will initicalize a file descriptor for the connection to server
The server side functionality is in the file server.

*/
#include<iostream>
#include<cstring> // For using the memset() and strcpy()
#include<cstdlib> // Used to define EXIT_FAILURE. It means that the program failed to execute and exited with error code 1. 
#include<sys/socket.h> // Used for socket() and connect()
#include<sys/un.h> // Used to define sockaddr_un which holds all the information about the socket address. It will be used as a blueprint to create structure 'addr'
#include<unistd.h> // For using the close(), read() and write() system calls

// Define the socket name and buffer size
#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 1024

using namespace std;

int main()
{
    // ===================================================
    // 1. Creating Socket
    // ===================================================
    int sockfd;
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    // Raise error if the socket connection failed
    if (sockfd < 0)
    {
        cerr<<"Socket creation failed"<<endl;
        return EXIT_FAILURE;
    }

    // If the socket is created successfully
    cout<<"Socket created succeessfully"<<endl;

    // ===================================================
    // 2. Creating address structure
    // ===================================================

    // The following is an example of the named structure. For infomration about what are named structures and how to use them, refer the link:https://www.w3schools.com/cpp/cpp_structs.asp
    struct sockaddr_un addr; // This creates memeory to store all the information about the socket address. 

    // Clear structure memory
    memset(&addr, 0, sizeof(addr)); // Clear the entire structure by replacing any values stored in the structure with 0. 

    // Specify address family. 
    addr.sun_family = AF_UNIX; // Since, this is communication between processes, the family will be AF_UNIX

    // Specify socket path
    strcpy(addr.sun_path, SOCKET_NAME);

    // ===================================================
    // 3. Connect to server
    // ===================================================

    int result;

    // Using the connect() system call. For more information read https://man7.org/linux/man-pages/man2/connect.2.html#:~:text=The%20connect()%20system%20call,(2)%20for%20further%20details.
    result = connect(
        sockfd, // File descriptor. In this case, the value should be SOCK_STREAM to emulate TCP. To emulate UDP, the value is SOCK_DGRAM 
        (struct sockaddr*)&addr, // Socket address, already specified in the step 2
         sizeof(addr) // Length of the socket address
        ); 

    // Check if the connection to the socket was successfull. If not, raise error. 
    if (result < 0)
    {
        cerr<<"Connection to the server socket failed"<<endl;
    }

    cout<<"Connection to the server socket successfull"<<endl;

    // ===================================================
    // 4. Send integers to server one at a time
    // ===================================================

    int nums[] = {10, 20, 30, 40, 0};

    // Count how many number to send to the server
    int numCount = sizeof(nums) / sizeof(nums[0]);

    // Recursively send the numbers to the server
    for (int i =0 ; i< numCount; i++)
    {
        write(sockfd, &nums[i], sizeof(int));
        cout<<"Sent: "<<nums[i]<< endl;
    }
    cout<<"Message sent to the server"<<endl;

    // ===================================================
    // 5. Read response from the server
    // ===================================================

    // The server will send back the sum of the integers as a result
    int serverResult;

    int byteRead;
    byteRead = read(sockfd, &serverResult, sizeof(int));

    if (byteRead <0)
    {
        cerr<<"Could not read server response. Closing socket"<<endl;
        close(sockfd); // Closing socket
        return EXIT_FAILURE;
    }


    cout<<"The server says sum is: "<<serverResult<<endl;

    // ===================================================
    // 6. Closing the socket at the end
    // ===================================================

    close(sockfd);
    cout<<"Socket closed"<<endl;
    
    return 0;
}