#include <Servo.h>

const int trigPin = 4;  // Pin untuk sensor ultrasonik luar (TRIG)
const int echoPin = 3;  // Pin untuk sensor ultrasonik luar (ECHO)
const int trigPin2 = 6; // Pin untuk sensor ultrasonik dalam (TRIG)
const int echoPin2 = 5; // Pin untuk sensor ultrasonik dalam (ECHO)

long duration, duration2;
int distance, distance2;

Servo myservo;

void setup() {
  myservo.attach(8);  // Menghubungkan servo motor ke pin digital 8
  myservo.write(0);   // Posisi awal servo (tutup)

  // Set up sensor ultrasonic luar
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);  

  // Set up sensor ultrasonic dalam
  pinMode(trigPin2, OUTPUT); 
  pinMode(echoPin2, INPUT);  

  // Set up LED merah dan hijau
  pinMode(11, OUTPUT); // LED hijau
  pinMode(12, OUTPUT); // LED merah

  // Matikan kedua LED di awal
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);

  // Set up serial monitor untuk debugging
  Serial.begin(9600);
}

void loop() {
  // Mengukur jarak menggunakan sensor ultrasonik luar (untuk kontrol servo)
  distance = getDistance(trigPin, echoPin);
  Serial.print("Jarak luar: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Mengukur jarak menggunakan sensor ultrasonik dalam (untuk deteksi penuh/kosong)
  distance2 = getDistance(trigPin2, echoPin2);
  Serial.print("Jarak dalam: ");
  Serial.print(distance2);
  Serial.println(" cm");

  // Logika untuk sensor dalam: menunjukkan status penuh/kosong tempat sampah
  bool isFull = false; // Variabel untuk memeriksa apakah tempat sampah penuh
  if (distance2 <= 5) {  // Tempat sampah penuh
    digitalWrite(12, HIGH); // LED merah menyala
    digitalWrite(11, LOW);  // Matikan LED hijau
    isFull = true;          // Menandakan tempat sampah penuh
  } else { // Tempat sampah kosong
    digitalWrite(12, LOW);  // LED merah mati
    isFull = false;
  }

  // Logika untuk sensor luar: kontrol servo jika tempat sampah tidak penuh
  if (!isFull) {
    if (distance <= 20) { // Sampah terdeteksi di luar
      myservo.write(120);      // Buka penutup
      digitalWrite(11, HIGH);  // LED hijau menyala
      delay(2000);             // Tunggu 2 detik
    } else {
      myservo.write(0);        // Tutup penutup
      digitalWrite(11, LOW);   // Matikan LED hijau
    }
  } else {
    myservo.write(0);          // Tutup penutup (jika penuh)
  }

  delay(500);  // Delay untuk siklus loop
}

// Fungsi untuk menghitung jarak dengan sensor ultrasonik
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH); // Waktu gelombang ultrasonik
  int distance = duration * 0.034 / 2;    // Hitung jarak (cm)
  return distance;
}
