/*
Assignment 2 - CS5060: ACN
Socket Programming

Name     : Manan Bharatkumar Darji
Roll No. : CS22MTECH14004

Question : 

a. Create a server (file server) program. It listens on a TCP socket. The
server is meant to send specific file(s) (.txt/.png/.pdf) to its client
depending on the file requested (file path).

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

using namespace std;

class FileServerSocket {
    int PORT;
    int flags = 0, DataSent = 0;
    int MySocket, MyNewSocket;
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

    string GetFilesOnServer() {
        // This Function is used for getting all files on server [inside ./storage/server folder] 
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
        // This Function is used to create socket
        MySocket = socket(AF_INET, SOCK_STREAM, 0);
        if (MySocket == 0) {
            cout << "||Server Error Occured|| : Socket Creation Failed! :(";
            exit(EXIT_FAILURE);
        }
        cout << "||Server Log|| : Socket Created Successfully.  :)" << endl;
    }

    vector < string > FileNameTokenizer(string InFileLs, char Sep) {
        // This Function is used for seperating the string by some char which i used to seprate the list of files
        stringstream InFileLsStream(InFileLs);
        string File;
        vector < string > v;
        while (!InFileLsStream.eof()) {
            getline(InFileLsStream, File, Sep);
            v.push_back(File);
        }
        return v;
    }

    void BindServerSocket() {
        // This Function is used for bind operation
        if (bind(MySocket, (struct sockaddr * ) & address, sizeof(address)) < 0) {
            cout << "||Server Error Occured|| : Bind Failed!";
            exit(EXIT_FAILURE);
        }
        else{
            cout << "||Server Log|| : Successfully Binded." << endl;
        }
    }

    void ServerListen() {
        // This Function is used for listing/waiting for client to arrive
        if (listen(MySocket, 3) < 0) {
            cout << "||Server Error Occured|| : Listen Failed!";
            exit(EXIT_FAILURE);
        }
        else{
            cout << "||Server Log|| : Listening on port " << PORT << "..." << endl;
        }
    }

    void AcceptClientConnection() {
        // This Function is used for accepting the client's request
        MyNewSocket = accept(MySocket, (struct sockaddr * ) & address, (socklen_t * ) & address_length);
        if (MyNewSocket < 0) {
            cout << "||Server Error Occured|| : Accept Connection Issue!";
            exit(EXIT_FAILURE);
        }
        else{
            cout << "||Server Log|| : Connected to Client Successfully." << endl;
        }
    }
    string WaitForMessage() {
        // This Function is used for recving the message/command from server
        char MessageBuf[ByteSentAtOnce];
        memset(MessageBuf, '\0', sizeof(MessageBuf));
        int data = recv(MyNewSocket, & MessageBuf, sizeof(MessageBuf), 0);
        cout << "||Server Log|| : DATA Received - " << data << " " << MessageBuf << endl;
        return MessageBuf;
    }
    void SendMessage(string msg) {
        // This Function is used for sending the message/command to server
        cin.clear();
        cout << "||Server Log|| : Command Sent - " << msg.c_str() << endl;
        int response = send(MyNewSocket, msg.c_str(), strlen(msg.c_str()), 0);
        cout << "||Server Log|| : Response - " << response << endl;
    }
    int StrToInt(string n) {
        // This Function is used for converting string to int
        stringstream ss;
        ss << n;
        int i;
        ss >> i;
        return i;
    }
    string IntToStr(int n) {
        // This Function is used for converting the int to string
        stringstream ss;
        ss << n;
        string s;
        ss >> s;
        return s;
    }

    void SendSelectedFile(string FileName) {
        // This Function is used for sending selected file to the client
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
            this_thread::sleep_for(chrono::milliseconds(10));
        }
        cout << endl;
        cout << "||Server Log|| : Sent " << DataSent << " Bytes of Data" << endl;
        cout << "||Server Log|| : Data Not Sent " << max(0LL, size - DataSent) << " Bytes" << endl;
        cin.clear();
        fclose(file);
    }
};

int main() {
    FileServerSocket FS;
    vector < string > v;
    string files = FS.GetFilesOnServer();
    v = FS.FileNameTokenizer(files, '/');
    for (auto a: v) {
        cout << a << endl;
    }

    FS.BindServerSocket();
    FS.ServerListen();
    FS.AcceptClientConnection();

    
    // This is main infinite loop
    for (;;) {
        string rev = FS.WaitForMessage();
        cout << rev << endl;
        if (rev == "GETFL") {
            cout << files << endl;
            FS.SendMessage(files);
        } else if (rev == "GET") {
            string index = FS.WaitForMessage();
            cout << "index-" << index << endl;
            int ind = FS.StrToInt(index);
            cout << "ind" << ind;
            cout << v.at(ind) << endl;
            FS.SendSelectedFile(v.at(ind));
        } else if (rev == "BYE") {
            cout<<"||Server Log|| : Client Disconnected!" << endl;
            break;
        } else if( rev != "" ){
            cout << "||Server Log|| : No Such Command : " << rev << endl;
            FS.SendMessage("No Such Command! - "+rev);
        }else{
            // cout<<"||Server Log|| : Null Data "<< endl;
        }
    }

    return 0;
}