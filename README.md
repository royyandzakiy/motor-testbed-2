# Load Cell Firmware

[RFC Load Cell Software](https://docs.google.com/document/u/1/d/1FqSV9YbJ6J2EdJItCOYNZRxmBQPkKjuD6bdd6zNWwVM/edit#)

## Getting Strated
- Download & Install [VSCode](https://code.visualstudio.com/download)
- Clone repository `git clone git@bitbucket.org:efishery/loadcell-firmware.git`
- Open folder `loadcell-firmware`
- Add PlatformIO as extension
- Build & Upload

## Requirements:
1. Atur sampling rate
2. Atur durasi pengambilan data (buffer size)
3. Atur jumlah data yang dirata-rata
4. Atur mode upload data ke cloud (langsung kirim atau tunggu konfimasi)

Opsional:

- Bisa lihat kurvanya secara langsung (tanpa perlu post processing lagi ke spreadsheet).

## To Develop
- Implement Observer Pattern: loadCellDispatcher & loadCell as Pollable
- Implement Pollable of each Load Cell
    - Read then invoke to mqttClientHandler
- Implement Pollable of mqttClientHandler
- naming convention: loadcell01, loadcell02, loadcell03
- what mqtt topic?