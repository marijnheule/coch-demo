CNF=cube$$.cnf

./build/6gon-final > tmp$$.cnf
./build/gen-6gon-cube $1 > tmp$$.cube
./build/apply.sh tmp$$.cnf tmp$$.cube 1 > $CNF
rm tmp$$.cnf tmp$$.cube
md5sum $CNF | cut -c1-10 | awk '{print "MD5: "$1"\n"}'

mkfifo pipe$$
./cake_lpr/cake_lpr $CNF pipe$$ &
./cadical/build/cadical $CNF pipe$$ --lrat --no-binary --sat
wait
rm $CNF pipe$$
