Kargo Takip Sistemi
Ozan Sürmeli

1. Kullanıcı gönderici, alıcı, adres ve paket bilgilerini girerek gönderi oluşturur.
2. Standart ve ekspres teslimat seçenekleri vardır. Sigorta ve hediye paketi ücrete eklenebilir.
3. Gönderi, takip numarasıyla sorgulanır. Kayıtlar program açık olduğu sürece saklanır.
4. Builder kalıbında GonderiBuilder, girilen bilgileri toplar ve zorunlu alanları kontrol eder.
5. Strategy kalıbında Standart ve Ekspres sınıfları, seçilen teslimat türüne göre ücreti hesaplar.
6. Decorator kalıbında Sigorta ve HediyePaketi sınıfları, temel ücrete kendi hizmet bedellerini ekler.
7. Observer kalıbında Gonderi sınıfı, durum değişikliğini TakipEkrani ve BildirimKaydi sınıflarına iletir.
8. Kalıplar src klasöründeki kargo.hpp dosyasında, kullanıcı menüsü main.cpp dosyasındadır.
