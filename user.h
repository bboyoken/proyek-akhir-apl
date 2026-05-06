#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <limits>
#include <mysql.h>
#include "tabulate/table.hpp"
#include <cmath>
#include <sstream>

using namespace std;
using namespace tabulate;

extern string user;

struct DataMakanan {
    int id;
    string nama;
    string kategori;
    float kalori;
    float protein;
    float karbohidrat;
    float lemak;
};

void swapData(DataMakanan* a, DataMakanan* b) {
    DataMakanan temp = *a;
    *a = *b;
    *b = temp;
}

int fetchMakananToArray(MYSQL* conn, DataMakanan arr[], int max_size) {
    string query = "SELECT * FROM makanan";
    if (mysql_query(conn, query.c_str())) {
        cout << "Error ambil data: " << mysql_error(conn) << endl;
        return 0;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    int count = 0;

    while ((row = mysql_fetch_row(res)) && count < max_size) {
        arr[count].id = stoi(row[0]);
        arr[count].nama = row[1];
        arr[count].kategori = row[2];
        arr[count].kalori = stof(row[3]);
        arr[count].protein = stof(row[4]);
        arr[count].karbohidrat = stof(row[5]);
        arr[count].lemak = stof(row[6]);
        count++;
    }
    mysql_free_result(res);
    return count;
}

string formatFloat(float val) {
    stringstream ss;
    ss << val;
    return ss.str();
}

void tampilkanTabelMakanan(DataMakanan arr[], int n) {
    if (n == 0) {
        cout << "Tidak ada data makanan.\n";
        return;
    }
    Table tbl;
    
    tbl.add_row({"No", "ID", "Nama Makanan", "Kategori", "Kalori (kcal)", "Protein (g)", "Karbo (g)", "Lemak (g)"});
    tbl.row(0).format().font_align(FontAlign::center).font_style({FontStyle::bold});

    int no = 1; 
    
    for (int i = 0; i < n; i++) {
        tbl.add_row({
            to_string(no++), 
            to_string(arr[i].id), 
            arr[i].nama, 
            arr[i].kategori,
            formatFloat(arr[i].kalori), 
            formatFloat(arr[i].protein),
            formatFloat(arr[i].karbohidrat), 
            formatFloat(arr[i].lemak)
        });
    }

    for (size_t i = 1; i <= n; ++i) {
        tbl[i][0].format().font_align(FontAlign::center); 
        tbl[i][1].format().font_align(FontAlign::center); 
        tbl[i][4].format().font_align(FontAlign::center); 
        tbl[i][5].format().font_align(FontAlign::center); 
        tbl[i][6].format().font_align(FontAlign::center); 
        tbl[i][7].format().font_align(FontAlign::center); 
    }

    cout << tbl << endl;
}

int partisiKategori(DataMakanan arr[], int low, int high) {
    string pivot = arr[high].kategori;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].kategori <= pivot) {
            i++;
            swapData(&arr[i], &arr[j]);
        }
    }
    swapData(&arr[i + 1], &arr[high]);
    return i + 1;
}
void quickSortKategori(DataMakanan arr[], int low, int high) {
    if (low < high) {
        int pi = partisiKategori(arr, low, high);
        quickSortKategori(arr, low, pi - 1);
        quickSortKategori(arr, pi + 1, high);
    }
}

int partisiKalori(DataMakanan arr[], int low, int high) {
    float pivot = arr[high].kalori;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].kalori >= pivot) {
            i++;
            swapData(&arr[i], &arr[j]);
        }
    }
    swapData(&arr[i + 1], &arr[high]);
    return i + 1;
}
void quickSortKalori(DataMakanan arr[], int low, int high) {
    if (low < high) {
        int pi = partisiKalori(arr, low, high);
        quickSortKalori(arr, low, pi - 1);
        quickSortKalori(arr, pi + 1, high);
    }
}

int partisiMakro(DataMakanan arr[], int low, int high, int mode) {
    float pivot;
    if (mode == 1) pivot = arr[high].protein;
    else if (mode == 2) pivot = arr[high].karbohidrat;
    else pivot = arr[high].lemak;

    int i = low - 1;
    for (int j = low; j < high; j++) {
        float compareValue;
        if (mode == 1) compareValue = arr[j].protein;
        else if (mode == 2) compareValue = arr[j].karbohidrat;
        else compareValue = arr[j].lemak;

        if (compareValue >= pivot) {
            i++;
            swapData(&arr[i], &arr[j]);
        }
    }
    swapData(&arr[i + 1], &arr[high]);
    return i + 1;
}
void quickSortMakro(DataMakanan arr[], int low, int high, int mode) {
    if (low < high) {
        int pi = partisiMakro(arr, low, high, mode);
        quickSortMakro(arr, low, pi - 1, mode);
        quickSortMakro(arr, pi + 1, high, mode);
    }
}

int partisiNama(DataMakanan arr[], int low, int high) {
    string pivot = arr[high].nama;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].nama <= pivot) {
            i++;
            swapData(&arr[i], &arr[j]);
        }
    }
    swapData(&arr[i + 1], &arr[high]);
    return i + 1;
}
void quickSortNama(DataMakanan arr[], int low, int high) {
    if (low < high) {
        int pi = partisiNama(arr, low, high);
        quickSortNama(arr, low, pi - 1);
        quickSortNama(arr, pi + 1, high);
    }
}

int binarySearchNama(DataMakanan arr[], int low, int high, string key) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        string namaTengah = arr[mid].nama;
        
        if (namaTengah == key) return mid;
        if (namaTengah < key) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// ================= MODUL 6: BINARY SEARCH =================
// (Fungsi binarySearchNama biarkan saja, taruh fungsi ini di bawahnya)

int binarySearchKalori(DataMakanan arr[], int low, int high, float key) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        // Cek apakah kalori di tengah sama dengan yang dicari
        if (arr[mid].kalori == key) return mid;
        
        // KARENA ARRAY DESCENDING (Besar ke Kecil):
        // Jika angka di mid LEBIH KECIL dari key, berarti key ada di sebelah KIRI (bagian angka besar)
        if (arr[mid].kalori < key) high = mid - 1;
        // Jika angka di mid LEBIH BESAR dari key, berarti key ada di sebelah KANAN (bagian angka kecil)
        else low = mid + 1;
    }
    return -1; // Tidak ditemukan
}

void readKatalogGizi(MYSQL* conn) {
    system("cls");
    cout << "\n====== KATALOG GIZI MAKANAN ======\n\n";
    DataMakanan arr[100];
    int n = fetchMakananToArray(conn, arr, 100);
    tampilkanTabelMakanan(arr, n);
}

void reqGiziMakanan(MYSQL* conn) {
    system("cls");
    cout << "\n====== REQUEST GIZI MAKANAN ======\n\n";
    string namaReq, detailReq;

    while (true) {
        cout << "Masukkan nama makanan yang ingin di-request: ";
        getline(cin, namaReq);
        if (namaReq.empty()) {
            cout << "[Peringatan] Nama makanan tidak boleh kosong!\n";
        } else {
            break;
        }
    }

    while (true) {
        cout << "Masukkan deskripsi/detail tambahan: ";
        getline(cin, detailReq);
        if (detailReq.empty()) {
            cout << "[Peringatan] Deskripsi/detail tidak boleh kosong!\n";
        } else {
            break;
        }
    }

    cout << "\n[BERHASIL] Request makanan '" << namaReq << "' telah dikirim ke Admin untuk ditinjau.\n";
}

void kalkulatorKalori() {
    system("cls");
    cout << "\n====== KALKULATOR KALORI HARIAN (BMR) ======\n\n";
    
    try {
        char gender;
        float berat, tinggi;
        int umur;

        cout << "Pilih Jenis Kelamin (L/P): ";
        cin >> gender;
        if (toupper(gender) != 'L' && toupper(gender) != 'P') throw runtime_error("Jenis kelamin tidak valid!");

        cout << "Masukkan Berat Badan (kg) : "; cin >> berat;
        cout << "Masukkan Tinggi Badan (cm): "; cin >> tinggi;
        cout << "Masukkan Umur (tahun)     : "; cin >> umur;

        if (cin.fail() || berat <= 0 || tinggi <= 0 || umur <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw runtime_error("Input harus berupa angka positif!");
        }

        float bmr = 0;
        if (toupper(gender) == 'L') {
            bmr = 88.362 + (13.397 * berat) + (4.799 * tinggi) - (5.677 * umur);
        } else {
            bmr = 447.593 + (9.247 * berat) + (3.098 * tinggi) - (4.330 * umur);
        }

        cout << "\n============================================\n";
        cout << "Kalori Dasar (BMR) Anda: " << bmr << " Kkal/hari.\n";
        cout << "*(Ini adalah kalori yang dibutuhkan tubuh Anda untuk bernapas & bertahan hidup tanpa aktivitas)\n";
        
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << endl;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void kalkulatorBMI() {
    system("cls");
    cout << "\n====== KALKULATOR BMI (Body Mass Index) ======\n\n";
    
    try {
        float berat, tinggiCm;
        cout << "Masukkan Berat Badan (kg) : "; cin >> berat;
        cout << "Masukkan Tinggi Badan (cm): "; cin >> tinggiCm;

        if (cin.fail() || berat <= 0 || tinggiCm <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw runtime_error("Input harus berupa angka positif!");
        }

        float tinggiM = tinggiCm / 100.0;
        float bmi = berat / (tinggiM * tinggiM);

        cout << "\n============================================\n";
        cout << "Skor BMI Anda: " << round(bmi * 10.0) / 10.0 << endl;
        cout << "Kategori     : ";

        if (bmi < 18.5) cout << "Kekurangan Berat Badan (Underweight)\n";
        else if (bmi >= 18.5 && bmi <= 24.9) cout << "Normal (Ideal)\n";
        else if (bmi >= 25.0 && bmi <= 29.9) cout << "Kelebihan Berat Badan (Overweight)\n";
        else cout << "Obesitas\n";

    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << endl;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void menuSorting(MYSQL* conn) {
    system("cls");
    cout << "\n====== SORTING MAKANAN (QUICK SORT) ======\n\n";
    
    DataMakanan arr[100];
    int n = fetchMakananToArray(conn, arr, 100);

    if (n == 0) {
        cout << "Tidak ada data untuk disortir.\n";
        return;
    }

    cout << "1. Berdasarkan Jenis/Kategori (A-Z)\n";
    cout << "2. Berdasarkan Kalori Tertinggi\n";
    cout << "3. Berdasarkan Makronutrisi Tertinggi\n";
    cout << "Pilih opsi sorting: ";
    
    string pilihan;
    getline(cin, pilihan);

    if (pilihan == "1") {
        quickSortKategori(arr, 0, n - 1);
        cout << "\n>> Hasil Sorting Berdasarkan Jenis/Kategori:\n";
        tampilkanTabelMakanan(arr, n);
    } 
    else if (pilihan == "2") {
        quickSortKalori(arr, 0, n - 1);
        cout << "\n>> Hasil Sorting Berdasarkan Kalori Tertinggi (Descending):\n";
        tampilkanTabelMakanan(arr, n);
    } 
    else if (pilihan == "3") {
        cout << "\n   Pilih Makronutrisi:\n";
        cout << "   a. Protein\n   b. Karbohidrat\n   c. Lemak\n   Pilihan: ";
        string pilMakro;
        getline(cin, pilMakro);
        
        if (pilMakro == "a" || pilMakro == "A") {
            quickSortMakro(arr, 0, n - 1, 1);
            cout << "\n>> Hasil Sorting Protein Tertinggi:\n";
        } else if (pilMakro == "b" || pilMakro == "B") {
            quickSortMakro(arr, 0, n - 1, 2);
            cout << "\n>> Hasil Sorting Karbohidrat Tertinggi:\n";
        } else if (pilMakro == "c" || pilMakro == "C") {
            quickSortMakro(arr, 0, n - 1, 3);
            cout << "\n>> Hasil Sorting Lemak Tertinggi:\n";
        } else {
            cout << "Pilihan makronutrisi tidak valid.\n";
            return;
        }
        tampilkanTabelMakanan(arr, n);
    } else {
        cout << "Pilihan tidak valid!\n";
    }
}

void menuSearching(MYSQL* conn) {
    system("cls");
    cout << "\n====== SEARCHING MAKANAN (BINARY SEARCH) ======\n\n";
    
    DataMakanan arr[100];
    int n = fetchMakananToArray(conn, arr, 100);

    if (n == 0) {
        cout << "Tidak ada data untuk dicari.\n";
        return;
    }

    cout << "1. Cari Berdasarkan Nama Makanan\n";
    cout << "2. Cari Berdasarkan Jumlah Kalori\n";
    cout << "Pilih opsi pencarian: ";
    
    string pilihan;
    getline(cin, pilihan);

    if (pilihan == "1") {
        cout << "\nMasukkan Nama Makanan (Perhatikan Huruf Besar/Kecil): ";
        string key;
        getline(cin, key);

        // WAJIB DISORTING DULU SEBELUM BINARY SEARCH
        quickSortNama(arr, 0, n - 1); 

        int index = binarySearchNama(arr, 0, n - 1, key);

        if (index != -1) {
            cout << "\n[DATA DITEMUKAN]\n";
            DataMakanan hasil[1] = {arr[index]}; 
            tampilkanTabelMakanan(hasil, 1);
        } else {
            cout << "\n[DATA TIDAK DITEMUKAN] Makanan dengan nama '" << key << "' tidak ada di database.\n";
        }
    } 
    else if (pilihan == "2") {
        cout << "\nMasukkan Jumlah Kalori yang dicari (contoh: 130): ";
        float keyKalori;
        cin >> keyKalori;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bersihkan buffer input

        // WAJIB DISORTING DULU. Kita pakai quickSortKalori (Descending)
        quickSortKalori(arr, 0, n - 1);

        int index = binarySearchKalori(arr, 0, n - 1, keyKalori);

        if (index != -1) {
            cout << "\n[DATA DITEMUKAN]\n";
            DataMakanan hasil[1] = {arr[index]}; 
            tampilkanTabelMakanan(hasil, 1);
        } else {
            cout << "\n[DATA TIDAK DITEMUKAN] Makanan dengan kalori " << keyKalori << " kcal tidak ada di database.\n";
        }
    } 
    else {
        cout << "Pilihan tidak valid!\n";
    }
}

void menuUser(MYSQL* conn) {
    string pilihan;

    while (true) {
        system("cls");
        cout << "\n====== DASHBOARD GIZI (USER: " << user << ") ======\n\n";

        Table userMenu;
        userMenu.add_row({"No", "Menu User"});
        userMenu.add_row({"1", "Read Katalog Gizi"});
        userMenu.add_row({"2", "Req Gizi Makanan"});
        userMenu.add_row({"3", "Kalkulator Kalori"});
        userMenu.add_row({"4", "Kalkulator BMI"});
        userMenu.add_row({"5", "Sorting Makanan"});
        userMenu.add_row({"6", "Searching Makanan"});
        userMenu.add_row({"0", "Logout"});

        userMenu[0].format().font_align(FontAlign::center).font_style({FontStyle::bold});
        for (size_t i = 1; i <= 7; ++i) {
            userMenu[i][0].format().font_align(FontAlign::center);
        }

        cout << userMenu << endl;
        cout << "Silakan pilih menu yang tersedia : ";
        getline(cin, pilihan);

        if (pilihan == "1") {
            readKatalogGizi(conn);
            cout << "\nTekan enter untuk kembali..."; cin.get();
        } else if (pilihan == "2") {
            reqGiziMakanan(conn);
            cout << "\nTekan enter untuk kembali..."; cin.get();
        } else if (pilihan == "3") {
            kalkulatorKalori();
            cout << "\nTekan enter untuk kembali..."; cin.get();
        } else if (pilihan == "4") {
            kalkulatorBMI();
            cout << "\nTekan enter untuk kembali..."; cin.get();
        } else if (pilihan == "5") {
            menuSorting(conn);
            cout << "\nTekan enter untuk kembali..."; cin.get();
        } else if (pilihan == "6") {
            menuSearching(conn);
            cout << "\nTekan enter untuk kembali..."; cin.get();
        } else if (pilihan == "0") {
            extern bool isTerdaftar;
            extern string userRole;
            isTerdaftar = false;
            user = "";
            userRole = "";
            cout << "\nLogout berhasil. Tekan enter untuk melanjutkan..." << endl;
            cin.get();
            break;
        } else {
            cout << "Pilihan tidak valid, silakan tekan enter...";
            cin.get();
        }
    }
}

#endif