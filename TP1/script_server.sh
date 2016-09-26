#!/bin/bash

server_name="echoIteractiveServer"

gcc ${server_name}.c -o serverSD

pkill serverSD

./serverSD bambam_ipsum_100M
