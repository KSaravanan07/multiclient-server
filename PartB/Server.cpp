/*
Assignment 2 - CS5060: ACN
Socket Programming

Name     : Manan Bharatkumar Darji
Roll No. : CS22MTECH14004

Question : 

a. Extend the file server program in Part A to serve multiple clients in
parallel. i.e. if two different clients ask for a file, the server shall satisfy
both the clients in parallel without making any client wait. Maximum
number of clients to honor can be set at the server.

*/
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <dirent.h>
#include <sys/types.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

class FileServerSocket {
    // This is class we used for creating connection with server 
    int PORT;
    int flags = 0, DataSent = 0;
    int MySocket;
    int address_length;
    int ByteSentAtOnce = 1024 * 50;

    struct sockaddr_in address;
    FILE * file;

    public:
        FileServerSocket() {
            CreateServerSocket();
            PORT = 8000;
            cout << "Please Enter PORT Number to Start Server On: ";
            cin >> PORT;
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(PORT);
            address_length = sizeof(address);

        }

    void CreateServerSocket() {
        // This Function is used for creating socket 
        MySocket = socket(AF_INET, SOCK_STREAM, 0);
        if (MySocket == 0) {
            cout << "||Server Side Error|| : Socket Creation Failed! :(";
            exit(EXIT_FAILURE);
        }
        cout << "||Server Log|| : Socket Created Successfully.  :)" << endl;
    }

    void BindServerSocket() {
        // This Function is used for bind operation
        if (bind(MySocket, (struct sockaddr * ) & address, sizeof(address)) < 0) {
            cout << "||Server Side Error|| : Bind Failed!";
            exit(EXIT_FAILURE);
        }
        cout << "||Server Log|| : Successfully Binded." << endl;
    }

    void ServerListen() {
        // This Function is used for listning/waiting for client to arrive
        if (listen(MySocket, 3) < 0) {
            cout << "||Server Side Error|| : Listen Failed!";
            exit(EXIT_FAILURE);
        }
        cout << "||Server Log|| : Listening on port " << PORT << "..." << endl;
    }

    int AcceptClientConnection() {
        // This Function is used for accepting a client request and assigning new socket id for new client
        int MyNewSocket = accept(MySocket, (struct sockaddr * ) & address, (socklen_t * ) & address_length);
        if (MyNewSocket < 0) {
            cout << "||Server Side Error|| : Accept Connection Issue!";
            exit(EXIT_FAILURE);
        }
        cout << "||Server Log|| : Connected to New Client Successfully." << endl;
        return MyNewSocket;
    }
};

class FileServerClientConnSocket {
    // This is class we use to create seprate socket for each of the clients
    int PORT;
    int flags = 0, DataSent = 0;
    int MySocket;
    int address_length;
    int ByteSentAtOnce = 1024 * 50;

    struct sockaddr_in address;
    FILE * file;

    public:
        string GetFilesOnServer() {
            DIR * ServerDir;
            string FileLs = "";
            struct dirent * obj;
            ServerDir = opendir(".//Storage//Server"); //open directory
            if (ServerDir) {
                while ((obj = readdir(ServerDir)) != NULL) {
                    FileLs += obj -> d_name;
                    FileLs += "/";
                }
                closedir(ServerDir);
            }
            return FileLs;
        }

    void CreateServerSocket() {
        // This ClientThreadRuntion is used for creating the main socket
        MySocket = socket(AF_INET, SOCK_STREAM, 0);
        if (MySocket == 0) {
            cout << "||Server Side Error|| : Socket Creation Failed! :(";
            exit(EXIT_FAILURE);
        }
        cout << "||Server Log|| : Socket Created Successfully.  :)" << endl;
    }

    vector < string > FileNameTokenizer(string InFileLs, char Sep) {
        // This ClientThreadRuntion is used for seperating the string by some char which i used to seprate the list of files
        stringstream InFileLsStream(InFileLs);
        string File;
        vector < string > v;
        while (!InFileLsStream.eof()) {
            getline(InFileLsStream, File, Sep);
            v.push_back(File);
        }
        return v;
    }

    string WaitForMessage(int MyNewSocket) {
        // This ClientThreadRuntion is used for creating the connection with the server
        char MessageBuf[ByteSentAtOnce];
        memset(MessageBuf, '\0', sizeof(MessageBuf));
        int data = recv(MyNewSocket, & MessageBuf, sizeof(MessageBuf), 0);
        cout << "||Server Log|| : DATA Received - " << data << " " << MessageBuf << endl;
        return MessageBuf;
    }
    void SendMessage(int MyNewSocket, string msg) {
        // This ClientThreadRuntion is used for sending the message/command to server
        cin.clear();
        cout << "msg.c_str()" << msg.c_str() << endl;
        cout << "sizeof(msg)" << sizeof(msg) << endl;
        int response = send(MyNewSocket, msg.c_str(), strlen(msg.c_str()), 0);
        cout << "||Server Log|| : Response - " << response << endl;
    }
    int StrToInt(string n) {
        // This ClientThreadRuntion is used for converting string to int
        stringstream ss;
        ss << n;
        int i;
        ss >> i;
        return i;
    }
    string IntToStr(int n) {
        // This ClientThreadRuntion is used for converting the int to string
        stringstream ss;
        ss << n;
        string s;
        ss >> s;
        return s;
    }

    void SendSelectedFile(int MyNewSocket, string FileName) {
        // This ClientThreadRuntion is used for sending selected file to the client
        string ServerPath = ".//Storage//Server//";
        ServerPath.append(FileName);
        file = fopen(ServerPath.c_str(), "rb");

        if (file == NULL) {
            cout << "||Server Log|| : File Error Occured! :( " << ServerPath << endl;
            exit(EXIT_FAILURE);
        }
        fseek(file, 0, SEEK_END);
        long long size = ftell(file);
        cout << "||Server Log|| : Size Of File to be Sent : " << size << endl;
        send(MyNewSocket, & size, sizeof(long long), 0);
        int temp = ByteSentAtOnce;
        if (ByteSentAtOnce > size) {
            temp = size;
        }
        char buffer[temp];
        rewind(file);
        int DataSent = 0, DataSentCur = 0;
        while (!feof(file)) {
            memset(buffer, '\0', sizeof(buffer));
            fread(buffer, temp, 1, file);
            DataSentCur = send(MyNewSocket, & buffer, sizeof(buffer), flags);
            DataSent += DataSentCur;
            cout << "||Server Log|| : Number of Bytes Sent : " << DataSentCur << " | Total Data Sent " << DataSent << endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        cout << endl;
        cout << "||Server Log|| : Sent " << DataSent << " Bytes of Data" << endl;
        cout << "||Server Log|| : Data Not Sent " << max(0LL, size - DataSent) << " Bytes" << endl;
        cin.clear();
        fclose(file);
    }
};

void * ClientThreadRun(void * sockId) {
    FileServerClientConnSocket FS;
    int MyNewSocket = * ((int * ) sockId);
    vector < string > v;
    string files = FS.GetFilesOnServer();
    v = FS.FileNameTokenizer(files, '/');

    //this is main loop for a perticular client to interact with server
    for (;;) {
        string rev = FS.WaitForMessage(MyNewSocket);
        cout << rev << endl;
        if (rev == "GETFL") {
            cout << files << endl;
            FS.SendMessage(MyNewSocket, files);
        } else if (rev == "GET") {
            string index = FS.WaitForMessage(MyNewSocket);
            cout << "index-" << index << endl;

            int ind = FS.StrToInt(index);
            cout << "ind" << ind;
            try {
                cout << v.at(ind) << endl;
                FS.SendSelectedFile(MyNewSocket, v.at(ind));
            } catch (out_of_range const& exc) {
                cout << "||Server Side Error|| : File Does not Exist! / wrong index" << exc.what() << '\n';
                cout << "||Server Side Error|| : Exiting The Server due to ..." << '\n';
                // FS.SendMessage(MyNewSocket,"File Does not Exist! / wrong index");
                break;
            }
            
            
        }  else if (rev == "BYE") {
            cout<<"||Server Log|| : Client Disconnected!" << endl;
            break;
        } else if( rev != "" ){
            cout << "||Server Log|| : No Such Command : " << rev << endl;
            FS.SendMessage(MyNewSocket,"No Such Command! - "+rev);
        }else{
            // cout<<"||Server Log|| : Null Data "<< endl;
        }
    }
}

int main() {
    FileServerSocket FS1;
    FS1.BindServerSocket();
    FS1.ServerListen();
    //this is a main loop for diffrent clients
    for (;;) {
        int MyNewSocket = FS1.AcceptClientConnection();
        if (!MyNewSocket) {
            cout << "Error in new client";
        }
        pthread_t MyThread;
        int * pc = NULL;
        pc = new int;
        * pc = MyNewSocket;
        pthread_create( & MyThread, NULL, ClientThreadRun, pc);
    }
    return 0;
}