#!/bin/bash

IP=$(grep -E '^IP' "./conf/httpd.conf" | awk -F: '{print $2}')
echo $IP
