cd ~/ECEN_427/lab06/pit_driver/
sudo rmmod pit
sudo insmod pit.ko

echo "finish with pit mod"

cd ~/ECEN_427/lab06/space_invaders/audio/
sudo rmmod audio
sudo insmod audio.ko

cd ~/ECEN_427/lab06/space_invaders/

echo "finish with audio mod"
