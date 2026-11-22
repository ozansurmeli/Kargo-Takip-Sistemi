#include "kargo.hpp"
#include <iostream>
#include <sstream>

using namespace kargo;

std::string oku(const char* soru) {
    std::cout << soru << " ";
    std::string satir;
    if (!std::getline(std::cin, satir)) throw std::ios_base::failure("Girdi sona erdi.");
    return satir;
}

int sayi(const char* soru, int alt, int ust) {
    std::istringstream girdi(oku(soru));
    int deger;
    char fazla;
    if (!(girdi >> deger) || (girdi >> fazla) || deger < alt || deger > ust)
        throw std::invalid_argument("Geçerli aralıkta bir tam sayı girin.");
    return deger;
}

Gonderi yeniGonderi(int no) {
    GonderiBuilder builder;
    builder.gonderici(oku("Gönderici:")).alici(oku("Alıcı:")).adres(oku("Teslimat adresi:"));
    auto icerik = oku("Paket içeriği:");
    builder.paket(icerik, sayi("Ağırlık (1-1000 kg):", 1, 1000));
    std::unique_ptr<Tarife> tarife;
    if (sayi("1 Standart | 2 Ekspres:", 1, 2) == 1) tarife = std::make_unique<Standart>();
    else tarife = std::make_unique<Ekspres>();
    if (sayi("Sigorta? 1 Evet | 0 Hayır:", 0, 1))
        tarife = std::make_unique<Sigorta>(std::move(tarife));
    if (sayi("Hediye paketi? 1 Evet | 0 Hayır:", 0, 1))
        tarife = std::make_unique<HediyePaketi>(std::move(tarife));
    return builder.olustur(no, *tarife);
}

int main() {
    TakipEkrani ekran;
    BildirimKaydi kayit;
    std::map<int, Gonderi> gonderiler;
    int siradaki = 1001;
    while (true) {
        try {
            int secim = sayi("\n1 Gönderi oluştur | 2 Takip | 3 Durumu ilerlet | 0 Çıkış\nSeçim:", 0, 3);
            if (secim == 0) return 0;
            if (secim == 1) {
                auto gonderi = yeniGonderi(siradaki);
                gonderi.aboneOl(ekran);
                gonderi.aboneOl(kayit);
                std::cout << "Takip no: " << siradaki << " | Toplam: " << gonderi.toplam() << " TL\n";
                gonderiler.emplace(siradaki++, std::move(gonderi));
                continue;
            }
            int no = sayi("Takip numarası:", 1, 2147483647);
            auto it = gonderiler.find(no);
            if (it == gonderiler.end()) throw std::invalid_argument("Gönderi bulunamadı.");
            auto& gonderi = it->second;
            if (secim == 3) gonderi.ilerlet();
            const auto& b = gonderi.bilgiler();
            std::cout << b.gonderici << " -> " << b.alici << " | " << b.adres << "\n"
                      << b.icerik << " | " << b.kg << " kg | " << gonderi.toplam() << " TL\n"
                      << "Durum: " << ekran.sonDurum.at(no) << "\nBildirimler:\n";
            for (const auto& olay : kayit.gecmis.at(no)) std::cout << "  " << olay << "\n";
        } catch (const std::ios_base::failure&) {
            return 0;
        } catch (const std::logic_error& hata) {
            std::cout << "Hata: " << hata.what() << "\n";
        }
    }
}
