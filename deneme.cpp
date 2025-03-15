#include <iostream>
#include <cmath>
#include <algorithm>

int main() {
    // Sabitler
    const double timerClock = 36e6;            // Timer clock frekansı (36 MHz)
    const unsigned int maxARR = (1 << 16) - 1;   // Maksimum ARR değeri (65535)

    // Kullanıcıdan istenen frekans değeri alınır
    double f_desired;
    std::cout << "Istenen frekans degerini giriniz (Hz): ";
    std::cin >> f_desired;

    if (f_desired <= 0) {
        std::cerr << "Frekans 0 veya negatif olamaz!" << std::endl;
        return 1;
    }

    // AŞAMA 1: Analitik Tahmin
    unsigned int psc0 = static_cast<unsigned int>(std::ceil(timerClock / ((maxARR + 1.0) * f_desired))) - 1;
    unsigned int arr0 = static_cast<unsigned int>(std::floor(timerClock / ((psc0 + 1) * f_desired) - 1));
    
    // Eğer hesaplanan ARR değeri maxARR'yi aşıyorsa, uygun değeri bulana kadar PSC'yi arttırıyoruz
    while (arr0 > maxARR) {
        psc0++;
        arr0 = static_cast<unsigned int>(std::floor(timerClock / ((psc0 + 1) * f_desired) - 1));
    }
    
    double f_calc0 = timerClock / ((psc0 + 1.0) * (arr0 + 1.0));
    double error_best = std::abs(f_calc0 - f_desired);
    unsigned int psc_best = psc0;
    unsigned int arr_best = arr0;

    // AŞAMA 2: Yerel İyileştirme (psc0 ±20 aralığında arama)
    int psc_candidate_min = std::max(0, static_cast<int>(psc0) - 20);
    int psc_candidate_max = static_cast<int>(psc0) + 20;
    for (int psc_candidate = psc_candidate_min; psc_candidate <= psc_candidate_max; psc_candidate++) {
        int arr_candidate = static_cast<int>(std::round(timerClock / ((psc_candidate + 1) * f_desired) - 1));
        
        // ARR değeri sınır kontrolü
        if (arr_candidate > static_cast<int>(maxARR))
            arr_candidate = maxARR;
        else if (arr_candidate < 0)
            arr_candidate = 0;
        
        double f_candidate = timerClock / ((psc_candidate + 1.0) * (arr_candidate + 1.0));
        double error_candidate = std::abs(f_candidate - f_desired);
        
        if (error_candidate < error_best) {
            psc_best = static_cast<unsigned int>(psc_candidate);
            arr_best = static_cast<unsigned int>(arr_candidate);
            error_best = error_candidate;
        }
    }
    
    // Sonuçları yalnızca en son bulunan en düşük hatalı değerler olarak bastırıyoruz
    std::cout << "Istenen Frekans: " << f_desired << " Hz" << std::endl;
    std::cout << "PSC: " << psc_best << std::endl;
    std::cout << "ARR: " << arr_best << std::endl;
    std::cout << "Hesaplanan Frekans: " 
              << timerClock / ((psc_best + 1.0) * (arr_best + 1.0)) << " Hz" 
              << std::endl;
              
    return 0;
}
