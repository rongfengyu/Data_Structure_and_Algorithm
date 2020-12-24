#!/bin/sh

n=10
while true; do
    echo -n -e "\r还剩 $((n--)) 秒关机 ....  "
    read -rs -n1 -t1 && break
    [ $n -eq 0 ] && echo -e "\n正在关机"&& break
done
