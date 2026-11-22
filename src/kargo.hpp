#pragma once
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace kargo {


struct Tarife {
    virtual int hesapla(int kg) const = 0;
    virtual ~Tarife() = default;
};
struct Standart : Tarife {
    int hesapla(int kg) const override { return 40 + 10 * kg; }
};
struct Ekspres : Tarife {
    int hesapla(int kg) const override { return 70 + 15 * kg; }
};

class EkHizmet : public Tarife {
protected:
    std::unique_ptr<Tarife> temel;
public:
    explicit EkHizmet(std::unique_ptr<Tarife> tarife) : temel(std::move(tarife)) {
        if (!temel) throw std::invalid_argument("Temel tarife gerekli.");
    }
};
struct Sigorta : EkHizmet {
    using EkHizmet::EkHizmet;
    int hesapla(int kg) const override { return temel->hesapla(kg) + 25; }
};
struct HediyePaketi : EkHizmet {
    using EkHizmet::EkHizmet;
    int hesapla(int kg) const override { return temel->hesapla(kg) + 15; }
};

struct Gozlemci {
    virtual void guncelle(int no, const std::string& durum) = 0;
    virtual ~Gozlemci() = default;
};
struct TakipEkrani : Gozlemci {
    std::map<int, std::string> sonDurum;
    void guncelle(int no, const std::string& durum) override { sonDurum[no] = durum; }
};
struct BildirimKaydi : Gozlemci {
    std::map<int, std::vector<std::string>> gecmis;
    void guncelle(int no, const std::string& durum) override { gecmis[no].push_back(durum); }
};

struct Bilgiler {
    std::string gonderici, alici, adres, icerik;
    int kg = 0;
};

class Gonderi {
    friend class GonderiBuilder;
    int numara;
    Bilgiler bilgi;
    int ucret;
    std::size_t asama = 0;
    std::vector<std::reference_wrapper<Gozlemci>> gozlemciler;
    inline static const std::vector<std::string> durumlar{
        "Hazırlandı", "Yolda", "Dağıtımda", "Teslim edildi"
    };
    Gonderi(int no, Bilgiler b, int tutar) : numara(no), bilgi(std::move(b)), ucret(tutar) {}
public:
    const Bilgiler& bilgiler() const { return bilgi; }
    int toplam() const { return ucret; }
    void aboneOl(Gozlemci& hedef) {
        for (Gozlemci& kayit : gozlemciler) if (&kayit == &hedef) return;
        gozlemciler.push_back(hedef);
        hedef.guncelle(numara, durumlar[asama]);
    }
    void ilerlet() {
        if (asama + 1 == durumlar.size()) throw std::logic_error("Gönderi zaten teslim edildi.");
        ++asama;
        for (Gozlemci& hedef : gozlemciler) hedef.guncelle(numara, durumlar[asama]);
    }
};

class GonderiBuilder {
    Bilgiler bilgi;
public:
    GonderiBuilder& gonderici(std::string ad) { bilgi.gonderici = std::move(ad); return *this; }
    GonderiBuilder& alici(std::string ad) { bilgi.alici = std::move(ad); return *this; }
    GonderiBuilder& adres(std::string yer) { bilgi.adres = std::move(yer); return *this; }
    GonderiBuilder& paket(std::string icerik, int kg) {
        bilgi.icerik = std::move(icerik);
        bilgi.kg = kg;
        return *this;
    }
    Gonderi olustur(int no, const Tarife& tarife) const {
        for (const auto* alan : {&bilgi.gonderici, &bilgi.alici, &bilgi.adres, &bilgi.icerik})
            if (alan->find_first_not_of(" \t\r\n") == std::string::npos)
                throw std::invalid_argument("Gönderici, alıcı, adres ve içerik zorunludur.");
        if (bilgi.kg < 1 || bilgi.kg > 1000)
            throw std::invalid_argument("Ağırlık 1 ile 1000 kg arasında olmalıdır.");
        return Gonderi(no, bilgi, tarife.hesapla(bilgi.kg));
    }
};
}
