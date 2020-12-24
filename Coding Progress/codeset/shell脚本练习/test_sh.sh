#!/bin/sh
var=2
func1() {
	#do sth
	echo func1
	echo $var 
}
    
func2() {
	#do sth
	echo func2
	var2=45
}
        
main() {
	echo $1
	echo $2
	echo ${var2}
	func1
	func2
}
               
echo ${var2}
main "$@"
