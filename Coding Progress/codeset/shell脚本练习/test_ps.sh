#!/bin/sh
REPEAT_TIME=3

check_proc_state() {
	echo $1
	num="0"
	for i in $(seq 1 $REPEAT_TIME) 
	do
		num_t=`ps  | grep $1 | wc -l`
		echo $num_t
		if [ $num_t -gt $num ]; then
			num=$num_t
		fi
	done
	
	return $num
}
echo $1

check_proc_state loggatherd
if [ $? -le "1" ]; then
	echo le
else
	echo ge
fi
	
echo xxx 
echo $?
