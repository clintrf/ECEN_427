cd pit
sudo rmmod pit
make clean
make
sudo insmod pit.ko
cd ..
