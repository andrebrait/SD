#!/bin/bash

server_name="echoHybridServer"

gcc ${server_name}.c -o serverSD

pkill serverSD

./serverSD bambam_ipsum_100k