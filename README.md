# Final Project Robocon

Repository ini berisi kode dan dokumentasi untuk proyek robot beroda dua berbasis Arduino Uno. Robot ini dilengkapi dengan kendali Bluetooth, sistem PID, dan sensor jarak untuk navigasi otomatis.

## Komponen Proyek

### 1. Program Kontrol Robot
- Menggunakan modul Bluetooth HC-06 untuk kendali nirkabel
- Arduino Uno sebagai kontroler utama
- Konfigurasi robot beroda dua
- Memungkinkan kendali jarak jauh melalui komunikasi Bluetooth

### 2. Implementasi PID
- Penerapan sistem kontrol PID (Proportional-Integral-Derivative)
- Dilengkapi parameter pada serial monitor untuk performa optimal

### 3. Program Sensor
- Menggunakan sensor ultrasonik HC-SR04 untuk deteksi halangan
- Fitur navigasi otomatis:
  - Sensor mendeteksi objek di depan robot
  - Jika terdeteksi halangan, robot akan berbelok ke kanan
  - Proses ini berlanjut hingga menemukan jalur yang bebas hambatan
- Memungkinkan robot bergerak secara otonom menghindari tabrakan

## Kebutuhan Perangkat Keras
- Arduino Uno
- Modul Bluetooth HC-06
- Motor DC (2 buah)
- Driver Motor
- Roda (2 buah)
- Sensor HC-SR04
- Baterai
- Kabel jumper dan komponen pendukung lainnya

## Struktur Program
Proyek ini terbagi dalam tiga direktori utama:
- `Control_Robot/`: Berisi implementasi kontrol Bluetooth
- `PID_Program/`: Berisi kode sistem kontrol PID
- `Sensor_Robot_Final/`: Berisi kode integrasi sensor HC-SR04 dan logika navigasi

## Panduan Pemasangan
1. Clone repository ini
2. Upload kode yang sesuai ke Arduino Uno
3. Rangkai komponen hardware sesuai dengan skema rangkaian
4. Lakukan pairing perangkat dengan modul Bluetooth HC-06
5. Gunakan antarmuka kontrol untuk mengoperasikan robot

## Cara Penggunaan
### Mode Manual (Bluetooth)
1. Hubungkan smartphone dengan modul HC-06
2. Gunakan aplikasi Arduino Bluetooth Control untuk mengendalikan robot
3. Robot akan bergerak sesuai perintah yang diberikan

### Mode Otomatis (Sensor)
1. Aktifkan mode sensor
2. Robot akan mulai bergerak otomatis
3. Jika sensor mendeteksi halangan dalam jarak tertentu:
   - Robot akan berhenti
   - Berbelok ke kanan
   - Melanjutkan gerakan maju
   - Proses berulang jika menemui halangan baru

## Kontribusi
Silakan fork repository ini dan ajukan pull request untuk pengembangan lebih lanjut.

## Pembuat
- Raditya-0
