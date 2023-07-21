# coch-demo
Concurrent Proof Checking Demo

INSTALL 

  ./init.sh

This will install the CaDiCaL SAT solver. 
Since version 1.7.0 it can produce LRAT files
directly using the --lrat commandline option.

This will also install the cake_lpr proof 
checking tool. 


TEST

  ./run-coch.sh cnf/happy-6gon-17.cnf

This will run CaDiCaL on the CNF formula,
while producing a proof to pipe. The pipe
will be read concurrently by the cake_lpr
checker. More specifically, I will run the
following commands:

  mkfifo pipe
  ./cake_lpr/cake_lpr $CNF pipe &
  ./cadical/build/cadical $CNF pipe --lrat --no-binary

By default, CaDiCaL produces binary LRAT
proofs, but this is not supported by the
cake_lpr checker yet. 
