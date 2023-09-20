
git clone https://github.com/arminbiere/cadical
cd cadical
./configure
make
cd ..
#git clone https://github.com/tanyongkiam/cake_lpr
cd cake_lpr
make
cd ..

mkdir build
cd src
make
cp 6gon-final     ../build
cp 6hole-final    ../build
cp gen-6gon-cube  ../build
cp gen-6hole-cube ../build
cd ..
