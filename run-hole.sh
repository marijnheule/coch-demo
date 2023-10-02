CNF=cube$$.cnf
INS=$1
DIR=$2

echo "SEED: "$INS

if [ -z "$DIR" ]
then
  DIR=.
fi

./build/6hole-final > $DIR/tmp$$.cnf
./build/gen-6hole-cube $INS > $DIR/tmp$$.cube
./build/apply.sh $DIR/tmp$$.cnf $DIR/tmp$$.cube 1 > $CNF
md5sum $DIR/tmp$$.cube | cut -c1-10 | awk '{print "MD5: "$1}'
rm $DIR/tmp$$.cnf $DIR/tmp$$.cube
md5sum $DIR/$CNF | cut -c1-10 | awk '{print "MC5: "$1}'

mkfifo $DIR/pipe$$
./cake_lpr/cake_lpr $DIR/$CNF $DIR/pipe$$ &
./cadical/build/cadical $DIR/$CNF $DIR/pipe$$ --lrat --no-binary --sat --reducetarget=10
wait
rm $DIR/$CNF $DIR/pipe$$
