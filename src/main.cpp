#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

// --- INTEGRATED FILE SERVER ---
// This starts a lightweight listener so your iPad can talk to the Razer
void StartFileBridge() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all network interfaces
    serverAddr.sin_port = htons(8080); // iPad connects to http://[RAZER_IP]:8080

    bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, SOMAXCONN);

    // This would run in a separate thread so it doesn't freeze your Browser UI
    // It handles "GET" (download to iPad) and "POST" (upload from iPad)
}
