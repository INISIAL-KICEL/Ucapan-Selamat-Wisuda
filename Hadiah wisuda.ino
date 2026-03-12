#include <Wire.h> 
#include <LiquidCrystal_I2C.h> // Unduh library LiquidCrystal I2C oleh Frank de Brabander di LIBRARY MANAGER
#include <DS3231.h> // Unduh zip library DS3231

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Atur alamat I2C di antara 0x27 atau 0x3F (hanya salah satu)
DS3231 rtc(SDA, SCL); // Inisialisasi penggunaan I2C untuk DS3231

int speakerPin = 7; // Pin Buzzer
int length = 28; // Jumlah nada
char notes[] = "GGAGcB GGAGdc GGxecBA yyecdc";
int beats[] = {2,2,8,8,8,16,1,2,2,8,8,8,16,1,2,2,8,8,8,8,16,1,2,2,8,8,8,16};
int tempo = 200; // Jeda antar nada 
int playCount = 0; // Variabel berapa kali lagu diputar
bool songFinished = false; // Penanda lagu selesai diputar

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = {'C', 'D', 'E', 'F', 'G',  'A', 'B',           
                   'c', 'd', 'e', 'f', 'g', 'a', 'b',
                   'x', 'y' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275,  1136, 1014,
                   956,  834,  765,  593,  468,  346,  224,
                   655 , 715 };
  int SPEE = 5;

  // Mainkan nada yang sesuai dengan nama not
  for (int i = 0; i < 17; i++) {
    if (names[i] ==  note) {
      int newduration = duration / SPEE;
      playTone(tones[i], newduration);
    }
  }
}

void fadeDisplay() {
  for (int brightness = 0; brightness < 256; ++brightness) {
    lcd.setBacklight(brightness);
    delay(10);
  }
}

void setup() {
  rtc.begin(); // Inisialisasi DS3231
  lcd.init();  // Inisialisasi LCD 
  lcd.backlight(); // Menyalakan backlight
 //Pengaturan waktu pada jam
 //rtc.setDate(07, 06, 2024);   //Mengatur tanggal, bulan, dan tahun
 //rtc.setTime(15, 22, 00);     //Mengatur jam, menit, dan detik

  // Menampilkan Teks pada baris pertama. Muncul hurufnya satu per satu.
  lcd.setCursor(0, 0);
  String barissatu = " Happy Birthday";
  for (int i = 0; i < barissatu.length(); i++) {
    lcd.print(barissatu.charAt(i));
    delay(200);  // Jeda antara menampilkan huruf
  }

  delay(500); // Delay dalam milidetik

  // Menampilkan teks pada baris kedua. Muncul hurufnya satu per satu.
  lcd.setCursor(0, 1);
  String barisdua = "    TO YOU";
  for (int i = 0; i < barisdua.length(); i++) {
    lcd.print(barisdua.charAt(i));
    delay(200);  // Jeda antara menampilkan huruf
  }
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  if (!songFinished) {
    // Memutar Lagu
    for (int i = 0; i < length; i++) {
      if (notes[i] == ' ') {
        delay(beats[i] * tempo); // Jeda antar nada
      } else {
        playNote(notes[i],  beats[i] * tempo);
      }
      // Jeda waktu antar nada
      delay(tempo);
    }
    songFinished = true; // Setel penanda bahwa lagu telah selesai diputar
  }

  // Matikan backlight LCD saat lagu berhenti
  lcd.noBacklight();
  
  // Tunggu 5 detik sebelum menampilkan jam digital
  delay(5000);

  // Nyalakan backlight LCD sebelum menampilkan jam digital
  lcd.backlight();
  
  // Tampilkan jam digital
  while (true) {
    Time t = rtc.getTime();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tgl: "); 
    lcd.print(rtc.getDateStr());
    lcd.setCursor(0, 1);
    lcd.print("Jam: ");
    lcd.print(rtc.getTimeStr());
    delay(1000); // Delay untuk memperbarui jam setiap detik
  }
}


