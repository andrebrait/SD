#!/bin/bash

client_name="echoClient"

gcc ${client_name}.c -o clientSD

pkill clientSD
rm fwrite*
rm client_out_*

for i in `seq 1 100`
do
    (./clientSD 127.0.0.1 ${i}; rm fwrite${i})&
done
