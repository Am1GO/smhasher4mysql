#!/bin/bash

NAME="smhasher"
#CURDIR=/home/program/smhasher4mysql
MYSQLSOURCE=/usr/include/mysql
MYSQLPLUGINS=/usr/lib64/mysql/plugin

#cd $CURDIR

# compile the UDF as a shared object (I assume you have gcc)
echo -n "compiling "$NAME".cpp... "
gcc -O -fPIC -Wall -Ismhasher -I$MYSQLSOURCE -I/usr/include -c $NAME.cpp -o $NAME.o
rc=$?
if [[ $rc != 0 ]]; then
    echo "error, script stopped"
    exit $rc
fi
echo "done"
echo -n "compiling MurmurHash3.cpp... "
gcc -O -fPIC -Wall -Ismhasher -I$MYSQLSOURCE -I/usr/include -c smhasher/MurmurHash3.cpp -o murmurhash3.o
rc=$?
if [[ $rc != 0 ]]; then
    echo "error, script stopped"
    exit $rc
fi
echo "done"
echo -n "linking into "$NAME".so... "
g++ -shared -s $NAME.o murmurhash3.o -o $NAME.so
rc=$?
if [[ $rc != 0 ]]; then
    echo "error, script stopped"
    exit $rc
fi
echo "done"


# put the UDF somewhere where MySQL can find it
echo -n "copying "$NAME".so to "$MYSQLPLUGINS"... "
cp $NAME.so $MYSQLPLUGINS
rc=$?
if [[ $rc != 0 ]]; then
    echo "error, script stopped"
    exit $rc
fi
echo "done"


# run mysql and register the UDF's functions and input data types in MySQL
#CREATE FUNCTION my_hash64 RETURNS INTEGER SONAME 'smhasher.so';

# If you want to get rid of a UDF you have registered successfully, simply run mysql and type
#DROP FUNCTION my_hash64;


## these don't work
#CREATE FUNCTION my_hash64u (value VARCHAR(512))
#RETURNS BIGINT(21) UNSIGNED
#BEGIN
#    RETURN CAST(my_hash64(value) AS UNSIGNED);
#END;
