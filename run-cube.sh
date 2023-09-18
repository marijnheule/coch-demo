CNF=cube$$.cnf
DIR=$2

if [ -z "$DIR" ]
then
  DIR=/tmp
fi

./build/6gon-final > $DIR/tmp$$.cnf
./build/gen-6gon-cube $1 > $DIR/tmp$$.cube
./build/apply.sh $DIR/tmp$$.cnf $DIR/tmp$$.cube 1 > $CNF
rm $DIR/tmp$$.cnf $DIR/tmp$$.cube
md5sum $DIR/$CNF | cut -c1-10 | awk '{print "MD5: "$1"\n"}'

mkfifo $DIR/pipe$$
./cake_lpr/cake_lpr $DIR/$CNF $DIR/pipe$$ &
./cadical/build/cadical $DIR/$CNF $DIR/pipe$$ --lrat --no-binary --sat
wait
rm $DIR/$CNF $DIR/pipe$$
