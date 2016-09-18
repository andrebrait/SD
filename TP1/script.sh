#!/bin/bash

cc echoClient.c -o echoClient
cc echoConcurrentServer.c -o echoConcurrentServer
cc echoHybridServer.c -o echoHybridServer
cc echoIteractiveServer.c -o echoIteractiveServer

./echoIteractiveServer bambam_ipsum_1M > server_out &

for i in `seq 1 10`
do
    ./echoClient 127.0.0.1
done
