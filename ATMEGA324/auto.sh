PROGRAM=so_test_prog

if [ $# -eq 2 ]
  then
    PROGRAM=$2
fi

if [ $1 == 'comp' ];
then
	cd theo
	make
	cd ..
elif [ $1 == 'upload' ]
then
	sudo ./bootloadHID theo/main.hex
else
	echo Invalid option.""
fi
