#!/bin/sh
set -e

cp ../build/client/client_demo ./client
cp ../build/server/server_demo ./server
podman compose up --build -d
