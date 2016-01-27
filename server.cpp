#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         1000
#define QUEUE_SIZE          5
#define NAME_SIZE           255
#define CONTENT_TYPE_SIZE   10
#define HTTP_OK             "HTTP/1.1 200 OK"
#define CONTENT_TYPE        "Content-Type:"

int main(int argc, char *argv[])
{
    int hSocket, hServerSocket;  /* handle to socket */
    struct hostent *pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address; /* Internet socket address stuct */
    int nAddressSize = sizeof(struct sockaddr_in);
    char pBuffer[BUFFER_SIZE];
    int nHostPort;
    char startingDirectory[NAME_SIZE];
    char filePath[NAME_SIZE];

    if (argc < 3)
    {
        printf("\nUsage: server host-port startingDirectory\n");
        return 0;
    }
    else
    {
        nHostPort = atoi(argv[1]);
        strcpy(startingDirectory, argv[2]);
        strcpy(filePath, startingDirectory);
    }

    printf("\nStarting server on port %d in startingDirectory %s", nHostPort, startingDirectory);

    printf("\nMaking socket");
    /* make a socket */
    hServerSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (hServerSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return 0;
    }

    /* fill address struct */
    Address.sin_addr.s_addr = INADDR_ANY;
    Address.sin_port = htons(nHostPort);
    Address.sin_family = AF_INET;

    printf("\nBinding to port %d\n", nHostPort);

    /* bind to a port */
    if (bind(hServerSocket, (struct sockaddr *) &Address, sizeof(Address))
        == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }
    /*  get port number */
    getsockname(hServerSocket, (struct sockaddr *) &Address, (socklen_t * ) & nAddressSize);
    printf("opened socket as fd (%d) on port (%d) for stream i/o\n", hServerSocket, ntohs(Address.sin_port));

    printf("Server\n\
              sin_family        = %d\n\
              sin_addr.s_addr   = %d\n\
              sin_port          = %d\n", Address.sin_family, (int) Address.sin_addr.s_addr, ntohs(Address.sin_port)
    );

    printf("\nMaking a listen queue of %d elements", QUEUE_SIZE);
    /* establish listen queue */
    if (listen(hServerSocket, QUEUE_SIZE) == SOCKET_ERROR)
    {
        printf("\nCould not listen\n");
        return 0;
    }
    int optval = 1;
    setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    for (; ;)
    {
        printf("\nWaiting for a connection\n");
        /* get the connected socket */
        hSocket = accept(hServerSocket, (struct sockaddr *) &Address, (socklen_t * ) & nAddressSize);

        printf("\nGot a connection from %X (%d)\n",
               Address.sin_addr.s_addr,
               ntohs(Address.sin_port));
        memset(pBuffer, 0, sizeof(pBuffer));
        int bytesRead = read(hSocket, pBuffer, BUFFER_SIZE);
        printf("Got from browser %d\n%s\n", bytesRead, pBuffer);
        char requestedFile[NAME_SIZE];
        sscanf(pBuffer, "GET %s HTTP/1.1", requestedFile);
        strcat(filePath, requestedFile);
        /* analyse given directory */

        struct stat fileStat;
        char contentType[CONTENT_TYPE_SIZE];


        sprintf(pBuffer, "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\n\r\n\r\n"
                "<html>"
                "<ul>"
                "<li> <a>file.html</a></li>"
                "</ul>"
                "Hello"
                "</html>\n");
        write(hSocket, pBuffer, strlen(pBuffer));
        linger lin;
        unsigned int y = sizeof(lin);
        lin.l_onoff = 1;
        lin.l_linger = 10;
        setsockopt(hSocket, SOL_SOCKET, SO_LINGER, &lin, sizeof(lin));
        shutdown(hSocket, SHUT_RDWR);
        printf("\nClosing the socket");
        /* close socket */
        if (close(hSocket) == SOCKET_ERROR)
        {
            printf("\nCould not close socket\n");
            return 0;
        }
    }
}
