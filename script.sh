#!/bin/bash

# Check if duration argument is provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <duration>"
    exit 1
fi

# Launch C application in the background
./main &

# Sleep for the specified duration
sleep $1

# Send SIGINT signal to the C application
kill -SIGINT $(pgrep main)