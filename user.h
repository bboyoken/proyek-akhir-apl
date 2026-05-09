#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <limits>
#include <mysql.h>
#include <cmath>
#include "tabulate/table.hpp"
#include "helper.h"

using namespace std;
using namespace tabulate;

extern string user;
extern int currentUserId;

inline void readKatalogGizi(MYSQL* conn) {
    system("cls");
    cout << "\n====== KATALOG GIZI MAKANAN ======\n\n";
    DataMakanan arr[100];
    int n = fetchMakananToArray(conn, arr, 100);
    tabelMakanan(arr, n);
}

inline void reqGiziMakanan(MYSQL* conn) {
    system("cls");
    cout << "\n====== REQUEST GIZI MAKANAN ======\n\n";
    string namaReq;

    cout << "Masukkan nama makanan yang ingin di-request: ";
    getline(cin, namaReq);

    if (namaReq.empty()) {
        cout << "Nama makanan tidak boleh kosong!\n";
        return;
    }

    string query = "INSERT INTO request_user (id_user, nama_makanan_req, status_request) VALUES (" 
                   + to_string(currentUserId) + ", '" + namaReq + "', 'Pending')";

    if (mysql_query(conn, query.c_str())) {
        cout << "Gagal mengirim request: " << mysql_error(conn) << endl;
    } else {
        cout << "\n[BERHASIL] Request makanan '" << namaReq << "' telah dikirim ke Admin.\n";
        cout << "Silakan cek status secara berkala.\n";
    }
}

inline void kalkulatorKalori() {
    system("cls");
    cout << "\n====== KALKULATOR KALORI HARIAN (BMR) ======\n\n";
    try {
        char gender; float berat, tinggi; int umur;
        cout << "Pilih Jenis Kelamin (L/P): "; cin >> gender;
        if (toupper(gender) != 'L' && toupper(gender) != 'P') throw runtime_error("Jenis kelamin tidak valid!");

        cout << "Masukkan Berat Badan (kg) : "; cin >> berat;
        cout << "Masukkan Tinggi Badan (cm): "; cin >> tinggi;
        cout << "Masukkan Umur (tahun)     : "; cin >> umur;

        if (cin.fail() || berat <= 0 || tinggi <= 0 || umur <= 0) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw runtime_error("Input harus berupa angka positif!");
        }

        float bmr = (toupper(gender) == 'L') 
            ? (88.362 + (13.397 * berat) + (4.799 * tinggi) - (5.677 * umur))
            : (447.593 + (9.247 * berat) + (3.098 * tinggi) - (4.330 * umur));

        cout << "\n============================================\n";
        cout << "Kalori Dasar (BMR) Anda: " << bmr << " Kkal/hari.\n";
    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << endl;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

inline void kalkulatorBMI() {
    system("cls");
    cout << "\n====== KALKULATOR BMI (Body Mass Index) ======\n\n";
    try {
        float berat, tinggiCm;
        cout << "Masukkan Berat Badan (kg) : "; cin >> berat;
        cout << "Masukkan Tinggi Badan (cm): "; cin >> tinggiCm;

        if (cin.fail() || berat <= 0 || tinggiCm <= 0) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw runtime_error("Input harus berupa angka positif!");
        }

        float tinggiM = tinggiCm / 100.0;
        float bmi = berat / (tinggiM * tinggiM);

        cout << "\n============================================\n";
        cout << "Skor BMI Anda: " << round(bmi * 10.0) / 10.0 << "\nKategori     : ";
        if (bmi < 18.5) cout << "Kekurangan Berat Badan (Underweight)\n";
        else if (bmi >= 18.5 && bmi <= 24.9) cout << "Normal (Ideal)\n";
        else if (bmi >= 25.0 && bmi <= 29.9) cout << "Kelebihan Berat Badan (Overweight)\n";
        else cout << "Obesitas\n";

    } catch (const exception& e) {
        cout << "\n[ERROR] " << e.what() << endl;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

inline void menuSorting(MYSQL* conn) {
    system("cls");
    cout << "\n====== SORTING MAKANAN (QUICK SORT) ======\n\n";
    DataMakanan arr[100];
    int n = fetchMakananToArray(conn, arr, 100);

    if (n == 0) { cout << "Tidak ada data untuk disortir.\n"; return; }

    cout << "1. Berdasarkan Jenis/Kategori (A-Z)\n2. Berdasarkan Kalori Tertinggi-Terendah\n3. Berdasarkan Makronutrisi Tertinggi-Terendah\nPilih opsi: ";
    string pilihan;
    getline(cin, pilihan);

    if (pilihan == "1") {
        quickSortKategori(arr, 0, n - 1);
        cout << "\n>> Hasil Sorting Berdasarkan Jenis/Kategori:\n";
        tabelMakanan(arr, n);
    } else if (pilihan == "2") {
        quickSortKalori(arr, 0, n - 1);
        cout << "\n>> Hasil Sorting Berdasarkan Kalori Tertinggi:\n";
        tabelMakanan(arr, n);
    } else if (pilihan == "3") {
        cout << "\n   Pilih Makronutrisi:\n   a. Protein\n   b. Karbohidrat\n   c. Lemak\n   Pilihan: ";
        string pilMakro; getline(cin, pilMakro);
        
        if (pilMakro == "a" || pilMakro == "A") {
            quickSortMakro(arr, 0, n - 1, 1); cout << "\n>> Hasil Sorting Protein Tertinggi:\n";
        } else if (pilMakro == "b" || pilMakro == "B") {
            quickSortMakro(arr, 0, n - 1, 2); cout << "\n>> Hasil Sorting Karbohidrat Tertinggi:\n";
        } else if (pilMakro == "c" || pilMakro == "C") {
            quickSortMakro(arr, 0, n - 1, 3); cout << "\n>> Hasil Sorting Lemak Tertinggi:\n";
        } else {
            cout << "Pilihan makronutrisi tidak valid.\n"; return;
        }
        tabelMakanan(arr, n);
    } else {
        cout << "Pilihan tidak valid!\n";
    }
}

inline void menuSearching(MYSQL* conn) {
    system("cls");
    cout << "\n====== SEARCHING MAKANAN (BINARY SEARCH) ======\n\n";
    DataMakanan arr[100];
    int n = fetchMakananToArray(conn, arr, 100);

    if (n == 0) { cout << "Tidak ada data untuk dicari.\n"; return; }

    cout << "1. Cari Berdasarkan Nama Makanan\n2. Cari Berdasarkan Jumlah Kalori\nPilih opsi pencarian: ";
    string pilihan; getline(cin, pilihan);

    if (pilihan == "1") {
        cout << "\nMasukkan Nama Makanan (Perhatikan Huruf Besar/Kecil): ";
        string key; getline(cin, key);
        quickSortNama(arr, 0, n - 1); 
        int index = binarySearchNama(arr, 0, n - 1, key);

        if (index != -1) {
            cout << "\n[DATA DITEMUKAN]\n";
            DataMakanan hasil[1] = {arr[index]}; 
            tabelMakanan(hasil, 1);
        } else { cout << "\nMakanan dengan nama '" << key << "' tidak ada di database.\n"; }
    } else if (pilihan == "2") {
        cout << "\nMasukkan Jumlah Kalori yang dicari (contoh: 130): ";
        float keyKalori; cin >> keyKalori;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        quickSortKalori(arr, 0, n - 1);
        int index = binarySearchKalori(arr, 0, n - 1, keyKalori);

        if (index != -1) {
            cout << "\n[DATA DITEMUKAN]\n";
            DataMakanan hasil[1] = {arr[index]}; 
            tabelMakanan(hasil, 1);
        } else { cout << "\nMakanan dengan kalori " << keyKalori << " kcal tidak ada.\n"; }
    } else { cout << "Pilihan tidak valid!\n"; }
}

inline void menuUser(MYSQL* conn) {
    string pilihan;
    while (true) {
        system("cls");
        cout << "\n====== DASHBOARD GIZI (USER: " << user << ") ======\n\n";
        Table userMenu;
        userMenu.add_row({"No", "Menu User"});
        userMenu.add_row({"1", "Read Katalog Gizi"}); userMenu.add_row({"2", "Req Gizi Makanan"});
        userMenu.add_row({"3", "Kalkulator Kalori"}); userMenu.add_row({"4", "Kalkulator BMI"});
        userMenu.add_row({"5", "Sorting Makanan"}); userMenu.add_row({"6", "Searching Makanan"});
        userMenu.add_row({"0", "Logout"});

        userMenu[0].format().font_align(FontAlign::center);
        for (size_t i = 1; i <= 7; ++i) userMenu[i][0].format().font_align(FontAlign::center);
        cout << userMenu << "\nSilakan pilih menu yang tersedia : ";
        
        getline(cin, pilihan);
        if(pilihan.empty()) continue; // Skip jika kepencet enter 2 kali

        if (pilihan == "1") { readKatalogGizi(conn); 
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "2") { reqGiziMakanan(conn); 
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m"; cin.get(); } 
        else if (pilihan == "3") { kalkulatorKalori(); 
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "4") { kalkulatorBMI(); 
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "5") { menuSorting(conn); 
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "6") { menuSearching(conn); 
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "0") {
            extern bool isTerdaftar; 
            extern string userRole;
            isTerdaftar = false; user = ""; userRole = "";
            cout << "\n\033[1;32mLogout berhasil. Tekan enter...\033[0m"; 
            cin.get(); 
            break;
        } else { cout << "\033[1;33mPilihan tidak valid, silakan tekan enter...\033[0m"; 
            cin.get(); }
    }
}
#endif