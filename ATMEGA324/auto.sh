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
elif [ $1 == 'compnema' ]
then
	cd nema17
	make
	cd ..
elif [ $1 == 'comp28byj' ]
then
	cd 28byj
	make
	cd ..
elif [ $1 == 'upload' ]
then
	sudo ./bootloadHID theo/main.hex
elif [ $1 == 'uploadnema' ]
then
	sudo ./bootloadHID nema17/main.hex
elif [ $1 == 'upload28byj' ]
then
	sudo ./bootloadHID 28byj/main.hex
elif [ $1 == 'clean' ]
then
	cd theo
	make clean
	cd ..
else
	echo Invalid option.""
fi
