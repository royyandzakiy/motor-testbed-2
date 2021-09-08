# Load Cell Tester

## Requirements
Secara umum iya, yang disimpen di cloud cuma itu aja (data adc dari masing-masing load cell), tapi sampling rate nya beda-beda untuk tiap eksperimen.
Kalau di code yang saya bikin tu, ada proses sampling 50 data dengan periode 200 us, terus dirata-rata (sebagai lowpass filter). Pakai ini, berarti 1 paket data terdiri atas 3 bacaan adc perlu waktu 10ms. Berarti sampling rate maksimalnya 100 SPS.
Eksperimen pertama tentang pengaruh kemiringan, sampling rate gak perlu terlalu tinggi. 10 SPS juga dah lebih dari cukup.
Eksperimen kedua tentang beban kontinu. Ini sampling rate nya rendah juga gak apa (~ 1 SPS). Tapi ini bakal lama, pengambilan datanya bisa berhari-hari.
Eksperimen ketiga uji beban kejut. Ini perlu baca beban impuls, jadi sampling rate perlu dibuat besar. Averaging yang semula pake 50 data dikurangi jadi 5 data aja (bisa dapet sampling rate 1000 SPS). Untuk uji beban kejut ini durasi pengambilan datanya singkat (orde detik), tapi sampling rate tinggi.


Jadi dari saya keperluannya itu bisa:
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