CNF=$1

rm -f pipe$$ output$$
mkfifo pipe$$
./cake_lpr/cake_lpr $CNF pipe$$ > output &
./cadical/build/cadical $CNF pipe$$ --lrat --no-binary --sat
wait
cat output$$
rm -f pipe$$ output$$
