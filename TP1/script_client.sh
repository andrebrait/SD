#!/bin/bash

client_name="echoClient"

gcc ${client_name}.c -o clientSD

pkill clientSD &>/dev/null
rm fwrite* &>/dev/null
rm client_out_* &>/dev/null

for i in `seq 1 20`
do
    (./clientSD 127.0.0.1 ${i} 2>/dev/null; rm fwrite${i} &>/dev/null;)&
done

