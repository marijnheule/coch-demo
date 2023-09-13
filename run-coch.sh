CNF=$1

mkfifo pipe$$
./cake_lpr/cake_lpr $CNF pipe$$ &
./cadical/build/cadical $CNF pipe$$ --lrat --no-binary --sat
rm pipe$$
