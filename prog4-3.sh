#!/bin/bash
#Assignment #4-3, Carlo Alejandro Salazar, masc1415

g++ prog4.cpp

if [ "$#" -eq 2 ];
then
	name=$1
	addhours=$2
	select=`grep -rne $name database.txt | cut -d ':' -f1`
	(( select -= 1 ))

./a.out database.txt << EOF > /dev/null
$select
$addhours
EOF
	
fi



if [ "$#" -eq 3 ];
then
	newname=$1
	newrate=$2
	newhours=$3
	select=`wc -l database.txt | cut -d " " -f1`
	
./a.out database.txt << EOF > /dev/null
$select
$newname
$newrate
$newhours
EOF

fi






