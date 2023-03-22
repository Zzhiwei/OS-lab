#!/bin/bash

srun --pty bash
make
./monitor myshell

