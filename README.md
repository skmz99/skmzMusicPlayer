
# Introduction

Raspberry Pi Zero WH music player that plays .wav files within subdirectories of the ```Music``` folder through bluetooth headphones.

# Install dependencies

Make sure everything is up to date
```bash
sudo apt update
sudo apt upgrade
```

Install Pigpio
```bash
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install
```

install vlc
```bash
  sudo apt install -y vlc
```

## Run Locally

```sh
git clone "https://github.com/skmz99/skmzMusicPlayer.git"
```

Go to skmzMusicPlayer directory

```bash
cd skmzMusicPlayer
```
run skmzMusicPlayer
```bash
sh run.sh
```


