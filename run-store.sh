CNF=$1

./cadical/build/cadical $CNF proof.lrat --lrat --no-binary --sat
