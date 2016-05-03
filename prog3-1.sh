#!/bin/bash

echo "Assignment #3-1, Carlo Alejandro Salazar, masc1415"

password=`grep "YOKO JAMIKA" $1 | cut -d "," -f3`

expect -c "
	log_user 0
	spawn scp $3@$2:encryptor .
	expect password: { send $password\r }
	sleep 1
	exit"
	
expect -c "
	log_user 0
	spawn scp $3@$2:*.enc .
	expect password: { send $password\r }
	sleep 1
	exit"

for i in encryptor *.enc
do
	echo $i
done


