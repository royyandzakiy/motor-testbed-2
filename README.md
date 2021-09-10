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

## Commands List
```
// General
AT                  --> sample usage "AT:"
samplingStartDemo   --> sample usage "samplingStartDemo:"

// Loadcell Sampler
startLs             --> sample usage "startLs:"
stopLs              --> sample usage "stopLs:"
stateLs             --> sample usage "stateLs:"
printLs             --> sample usage "printLs:"
resetLs             --> sample usage "resetLs:"
setLs               --> sample usage "setLs:key=value;"

// Pump Out
startPo             --> sample usage "startPo:"
stopPo              --> sample usage "stopPo:"
statePo             --> sample usage "statePo:"
printPo             --> sample usage "printPo:"
setPo               --> sample usage "setPo:key=value;"

// Pump In
startPi             --> sample usage "startPi:"
stopPi              --> sample usage "stopPi:"
statePi             --> sample usage "statePi:"
printPi             --> sample usage "printPi:"
setPi               --> sample usage "setPi:key=value;"
```

## Requirements:
1. Atur sampling rate
2. Atur durasi pengambilan data (buffer size)
3. Atur jumlah data yang dirata-rata
4. Atur mode upload data ke cloud (langsung kirim atau tunggu konfimasi)

Opsional:

- Bisa lihat kurvanya secara langsung (tanpa perlu post processing lagi ke spreadsheet).

## To Develop
- Code Review
    - PI implementation
    - Debit count
    - demo doing "samplingStart()"
- Sampler
    - insert threshold inside sampler as private object
- Use DataSaver
    - Save to SdCard after every successfull reading
- Implement Uploader
    - Upload last sampled data