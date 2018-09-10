cd audio
sudo rmmod audio
make clean
make
sudo insmod audio.ko
cd ..
