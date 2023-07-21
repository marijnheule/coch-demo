CNF=$1

./cadical/build/cadical $CNF proof.lrat --lrat --no-binary --sat
time ./cake_lpr/cake_lpr $CNF proof.lrat
rm proof.lrat
