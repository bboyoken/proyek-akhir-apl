#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include <mysql.h>
#include "tabulate/table.hpp"

using namespace std;
using namespace tabulate;

extern bool isTerdaftar;
extern string user;
extern string userRole;

void readData(MYSQL* conn) {
    system("cls");
    cout << "\n====== DATA MAKANAN ======\n\n";

    Table tbl;
    tbl.add_row({"No", "ID", "Nama Makanan", "Kategori", "Kalori (kcal)", "Protein (g)", "Karbohidrat (g)", "Lemak (g)"});
    tbl.row(0).format().font_align(FontAlign::center).font_style({FontStyle::bold});

    string query = "SELECT * FROM makanan";
    if (mysql_query(conn, query.c_str())) {
        cout << "Error ambil data: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;

    if (mysql_num_rows(res) == 0) {
        cout << "Belum ada data makanan di database\n";
    } else {
        int no = 1; 
        while ((row = mysql_fetch_row(res))) {
            tbl.add_row({to_string(no++), row[0], row[1], row[2], row[3], row[4], row[5], row[6]});
        }

        for (size_t i = 1; i <= mysql_num_rows(res); ++i) {
            tbl[i][0].format().font_align(FontAlign::center); 
            tbl[i][1].format().font_align(FontAlign::center); 
            tbl[i][4].format().font_align(FontAlign::center); 
            tbl[i][5].format().font_align(FontAlign::center); 
            tbl[i][6].format().font_align(FontAlign::center); 
            tbl[i][7].format().font_align(FontAlign::center); 
        }
        cout << tbl << endl;
    }
    mysql_free_result(res);
}

string formatAngka(float angka) {
    string teks = to_string(angka);
    for (size_t i = 0; i < teks.length(); i++) {
        if (teks[i] == ',') {
            teks[i] = '.'; 
        }
    }
    return teks;
}

void createData(MYSQL* conn) {
    system("cls");
    cout << "\n====== TAMBAH DATA MAKANAN ======\n\n";

    string nama, kategori;
    float kalori, protein, karbohidrat, lemak;

    while (true) {
        try {
            cout << "Nama Makanan : ";
            getline(cin, nama);
            
            if (nama.empty()) {
                throw invalid_argument("Nama makanan tidak boleh kosong");
            }
            
            bool isHuruf = true;
            for (char c : nama) {
                if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
                    isHuruf = false; break;
                }
            }
            if (!isHuruf) throw invalid_argument("Silakan masukkan nama makanan berupa huruf");
            break;
        } catch(const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    while (true) {
        try {
            cout << "Kategori (contoh: Buah, Lauk) : ";
            getline(cin, kategori);
            
            if (kategori.empty()) {
                throw invalid_argument("Kategori tidak boleh kosong");
            }
            
            bool isHuruf = true;
            for (char c : kategori) {
                if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
                    isHuruf = false; break;
                }
            }
            if (!isHuruf) throw invalid_argument("Silakan masukkan kategori berupa huruf");
            break;
        } catch(const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    while (true) {
        try {
            cout << "Jumlah Kalori : ";
            if (cin.peek() == '\n') cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> kalori;
            if (cin.fail()) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Input harus berupa angka");
            }
            if (kalori < 0) throw runtime_error("Angka tidak boleh negatif");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        } catch(const exception& e) { cout << "Error: " << e.what() << endl; }
    }

    while (true) {
        try {
            cout << "Jumlah Protein (g) : ";
            if (cin.peek() == '\n') cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> protein;
            if (cin.fail()) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Input harus berupa angka");
            }
            if (protein < 0) throw runtime_error("Angka tidak boleh negatif");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        } catch(const exception& e) { cout << "Error: " << e.what() << endl; }
    }

    while (true) {
        try {
            cout << "Jumlah Karbohidrat (g) : ";
            if (cin.peek() == '\n') cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> karbohidrat;
            if (cin.fail()) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Input harus berupa angka");
            }
            if (karbohidrat < 0) throw runtime_error("Angka tidak boleh negatif");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        } catch(const exception& e) { cout << "Error: " << e.what() << endl; }
    }

    while (true) {
        try {
            cout << "Jumlah Lemak (g) : ";
            if (cin.peek() == '\n') cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> lemak;
            if (cin.fail()) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Input harus berupa angka");
            }
            if (lemak < 0) throw runtime_error("Angka tidak boleh negatif");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        } catch(const exception& e) { cout << "Error: " << e.what() << endl; }
    }

    string query = "INSERT INTO makanan (nama_makanan, kategori, kalori, protein, karbohidrat, lemak) VALUES ('" + 
                   nama + "', '" + kategori + "', " + formatAngka(kalori) + ", " + formatAngka(protein) + ", " + 
                   formatAngka(karbohidrat) + ", " + formatAngka(lemak) + ")";

    if (mysql_query(conn, query.c_str())) {
        cout << "Gagal menyimpan data: " << mysql_error(conn) << endl;
    } else {
        cout << "\nData Makanan berhasil ditambahkan ke database" << endl;
    }
}

void editData(MYSQL* conn) {
    readData(conn);
    
    string targetId;
    cout << "\nMasukkan ID Makanan yang akan diedit: ";
    getline(cin, targetId);

    string checkQuery = "SELECT * FROM makanan WHERE id_makanan = " + targetId;
    mysql_query(conn, checkQuery.c_str());
    MYSQL_RES* res = mysql_store_result(conn);

    if (mysql_num_rows(res) == 0) {
        cout << "Data dengan ID " << targetId << " tidak ditemukan.\n";
        mysql_free_result(res);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(res);
    string oldNama = row[1];
    string oldKalori = row[3];
    string oldProtein = row[4];
    string oldKarbo = row[5];
    string oldLemak = row[6];
    mysql_free_result(res);

    string namaBaru, kaloriBaru, proteinBaru, karbohidratBaru, lemakBaru; 

    while (true) {
        cout << "Nama produk [" << oldNama << "] : ";
        getline(cin, namaBaru);
        if (namaBaru.empty()) 
        break; 
        
        bool isHuruf = true;
        for (char c : namaBaru) {
            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
                isHuruf = false; 
                break;
            }
        }
        if (!isHuruf) {
            cout << "Silakan masukkan huruf saja\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Kalori baru [" << oldKalori << "] : ";
        getline(cin, kaloriBaru);
        if (kaloriBaru.empty()) break;

        bool isAngka = true; int dotCount = 0;
        for (char &c : kaloriBaru) {
            if (c == '.') {
                dotCount++;
                if (dotCount > 1) isAngka = false;
            } else if (c < '0' || c > '9') {
                isAngka = false; 
                break;
            }
        }
        if (!isAngka) {
            cout << "Error: Input harus berupa angka valid!\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Protein baru (g) [" << oldProtein << "] : ";
        getline(cin, proteinBaru);
        if (proteinBaru.empty()) 
        break;

        bool isAngka = true; int dotCount = 0;
        for (char &c : proteinBaru) {
            if (c == ',') c = '.'; 
            if (c == '.') { dotCount++; if (dotCount > 1) isAngka = false; } 
            else if (c < '0' || c > '9') { isAngka = false; 
                break; }
        }
        if (!isAngka) { cout << "Error: Input harus berupa angka valid!\n"; 
            continue; }
        break;
    }

    while (true) {
        cout << "Karbohidrat baru (g) [" << oldKarbo << "] : ";
        getline(cin, karbohidratBaru);      
        if (karbohidratBaru.empty()) 
        break;

        bool isAngka = true; int dotCount = 0;
        for (char &c : karbohidratBaru) {
            if (c == ',') c = '.'; 
            if (c == '.') { dotCount++; if (dotCount > 1) isAngka = false; } 
            else if (c < '0' || c > '9') { isAngka = false; 
                break; }
        }
        if (!isAngka) { cout << "Error: Input harus berupa angka valid!\n"; 
            continue; }
        break;
    }

    while (true) {
        cout << "Lemak baru (g) [" << oldLemak << "] : ";
        getline(cin, lemakBaru);
        if (lemakBaru.empty()) 
        break;

        bool isAngka = true; int dotCount = 0;
        for (char &c : lemakBaru) {
            if (c == ',') c = '.'; 
            if (c == '.') { dotCount++; if (dotCount > 1) isAngka = false; } 
            else if (c < '0' || c > '9') { isAngka = false; 
                break; }
        }
        if (!isAngka) { cout << "Error: Input harus berupa angka valid\n"; 
            continue; }
        break;
    }

    string updateQuery = "UPDATE makanan SET ";
    bool isUpdate = false;

    if (!namaBaru.empty()) {
        updateQuery += "nama_makanan = '" + namaBaru + "'";
        isUpdate = true;
    }
    if (!kaloriBaru.empty()) {
        if (isUpdate) updateQuery += ", ";
        updateQuery += "kalori = " + kaloriBaru;
        isUpdate = true;
    }
    if (!proteinBaru.empty()) {
        if (isUpdate) updateQuery += ", ";
        updateQuery += "protein = " + proteinBaru;
        isUpdate = true;
    }
    if (!karbohidratBaru.empty()) {
        if (isUpdate) updateQuery += ", ";
        updateQuery += "karbohidrat = " + karbohidratBaru;
        isUpdate = true;
    }
    if (!lemakBaru.empty()) {
        if (isUpdate) updateQuery += ", ";
        updateQuery += "lemak = " + lemakBaru;
        isUpdate = true;
    }

    updateQuery += " WHERE id_makanan = " + targetId;

    if (isUpdate) {
        if (mysql_query(conn, updateQuery.c_str())) {
            cout << "Gagal mengupdate data: " << mysql_error(conn) << endl;
        } else {
            cout << "\nData berhasil diupdate!" << endl;
        }
    } else {
        cout << "\nTidak ada data yang diubah." << endl;
    }
}

void deleteData(MYSQL* conn) {
    readData(conn);

    string targetId;
    cout << "\nMasukkan ID Makanan yang akan dihapus: ";
    getline(cin, targetId);

    string deleteQuery = "DELETE FROM makanan WHERE id_makanan = " + targetId;
    
    if (mysql_query(conn, deleteQuery.c_str())) {
        cout << "Gagal menghapus data: " << mysql_error(conn) << endl;
    } else {
        if (mysql_affected_rows(conn) > 0) {
            cout << "Data Makanan dengan ID " << targetId << " berhasil dihapus." << endl;
        } else {
            cout << "Data dengan ID tersebut tidak ditemukan." << endl;
        }
    }
}

void tabelMenuAdmin() {
    system("cls");
    cout << "\n====== Menu Admin ======\n\n";

    Table adminMenu;
    adminMenu.add_row({"No", "Menu"});
    adminMenu.add_row({"1", "Read (Lihat data makanan)"});
    adminMenu.add_row({"2", "Create (Tambah data makanan)"});
    adminMenu.add_row({"3", "Update (Update data makanan)"});
    adminMenu.add_row({"4", "Delete (Hapus data makanan)"});
    adminMenu.add_row({"5", "Konfirmasi request user"});
    adminMenu.add_row({"6", "Log user"});
    adminMenu.add_row({"7", "Menu sorting"});
    adminMenu.add_row({"8", "Menu searching"});
    adminMenu.add_row({"9", "Manajemen rekomendasi diet"});
    adminMenu.add_row({"0", "Kembali ke menu sebelumnya"});

    adminMenu[0].format().font_align(FontAlign::center);
    for (size_t i = 1; i <= 10; ++i) {
        adminMenu[i][0].format().font_align(FontAlign::center);
    }
    cout << adminMenu << endl;
}

void menuAdmin(MYSQL* conn) {
    string pilihan;
    
    while (isTerdaftar && userRole == "admin") {
        tabelMenuAdmin();
        cout << "Silakan pilih menu admin: ";
        getline(cin, pilihan);

        if (pilihan == "1") {
            readData(conn);
            cout << "\nTekan enter untuk kembali..."; cin.get();
        } else if (pilihan == "2") {
            createData(conn);
            cout << "\nTekan enter untuk kembali..."; cin.get();
        } else if (pilihan == "3") {
            editData(conn);
            cout << "\nTekan enter untuk kembali..."; cin.get();
        } else if (pilihan == "4") {
            deleteData(conn);
            cout << "\nTekan enter untuk kembali..."; cin.get();
        } else if (pilihan == "5") {
            cout << "\non progress..." << endl;
            cin.get();
        } else if (pilihan == "6") {
            cout << "\non progress..." << endl;
            cin.get();
        } else if (pilihan == "7") {
            cout << "\non progress..." << endl;
            cin.get();
        } else if (pilihan == "8") {
            cout << "\non progress..." << endl;
            cin.get();
        } else if (pilihan == "9") {
            cout << "\non progress..." << endl;
            cin.get();
        } else if (pilihan == "0") {
            isTerdaftar = false;
            user = "";
            userRole = "";
            cout << "\nTekan enter untuk melanjutkan..." << endl;
            cin.get();
            break;
        } else {
            cout << "\nPilihan tidak valid, silakan coba lagi" << endl;
            cout << "\nTekan enter untuk melanjutkan" << endl;
            cin.get();
        }
    }
}

#endif