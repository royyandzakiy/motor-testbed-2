# Load Cell Firmware

[RFC Load Cell Software](https://docs.google.com/document/u/1/d/1FqSV9YbJ6J2EdJItCOYNZRxmBQPkKjuD6bdd6zNWwVM/edit#)

## Getting Strated
- Download & Install [VSCode](https://code.visualstudio.com/download)
- Add PlatformIO as extension
- Clone repository `git clone git@bitbucket.org:efishery/loadcell-firmware.git`
- Open in VSCode folder `loadcell-firmware`
- Build & Upload

## How to Use
- Setelah muncul tulisan "[commandTask] started", bisa mulai memberikan input ke Serial Monitor

## Flow
- input volt motor dc

## Commands List
```
// General
AT                  --> usage "AT:"
samplingStartDemo   --> usage "samplingStartDemo:"

// Loadcell Sampler
startLs             --> usage "startLs:"
stopLs              --> usage "stopLs:"
stateLs             --> usage "stateLs:"
printLs             --> usage "printLs:"
resetLs             --> usage "resetLs:"
setLs               --> usage "setLs:<key>=<value>;"

// Pump Out
startPo             --> usage "startPo:"
stopPo              --> usage "stopPo:"
statePo             --> usage "statePo:"
printPo             --> usage "printPo:"
setPo               --> usage "setPo:<key>=<value>;"

// Pump In
startPi             --> usage "startPi:"
stopPi              --> usage "stopPi:"
statePi             --> usage "statePi:"
printPi             --> usage "printPi:"
setPi               --> usage "setPi:<key>=<value>;"

// example inputs
startPi;
stopPi;
setLs:avgSamplingBufferSize=60;
setLs:totalSamplingStopMode=buffer;
setLs:totalSamplingBufferSize=100;
startLs:
stopLs:
```

## Requirements:
1. Atur sampling rate
2. Atur durasi pengambilan data (buffer size)
3. Atur jumlah data yang dirata-rata
4. Atur mode upload data ke cloud (langsung kirim atau tunggu konfimasi)

Opsional:

- Bisa lihat kurvanya secara langsung (tanpa perlu post processing lagi ke spreadsheet).

---

## To Do Next:

## To Develop
- Sampler: Remove load cell logic from inside, use dependency injection for load cell logic
    - Use for all other sensors
v Sensor: create sensor generic class
v SensorTask: create loadsensor task
v implement config, test
v change Sensor and Sampler to accomodate deviceConfig
v move name to sensor, not sampler
- implement serialCommand
- implement global, to add macro for printing type