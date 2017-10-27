#/usr/bin/env pyhton3 
import inspect, os, sys
from socket import *

def main():
    host, port = 'localhost', 5555

    with socket(AF_INET, SOCK_STREAM) as sfd:
        sfd.connect((host,port))
        print("Connected To:", sfd.getpeername(),
                  "From:", sfd.getsockname())
        err = 'ERR: '

        while True:
            data = sfd.recv(1024)
            if "output" in data.decode():
                print(data.decode())
                continue;
            if data.decode() == "quit": break
            text = input(data.decode())
            if text.lower() == "quit": break
            sfd.send(text.encode()) 
if __name__ == "__main__": main()
