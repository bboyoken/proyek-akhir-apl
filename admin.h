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

struct Makanan {
    string id;
    string nama;
    string kategori;
    float kalori;
};

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

void readLog(MYSQL* conn) {
    system("cls");
    cout << "\n====== LOG AKTIVITAS USER ======\n\n";

    Table tblLog;
    tblLog.add_row({"No", "ID User", "Aktivitas", "Waktu"});

    string query = "SELECT id_user, aktivitas, waktu FROM log_user ORDER BY waktu DESC";
    if (mysql_query(conn, query.c_str())) {
        cout << "Error ambil log: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    int no = 1;

    while ((row = mysql_fetch_row(res))) {
        tblLog.add_row({to_string(no++), row[0], row[1], row[2]});
    }

    if (no == 1) {
        cout << "Belum ada riwayat aktivitas." << endl;
    } else {
        cout << tblLog << endl;
    }
    
    mysql_free_result(res);
}

void updateRekomendasi(MYSQL *conn) {
    cin.ignore();
    string id, saranBaru;
    cout << "Masukkan ID Rekomendasi yang ingin diubah: ";
    getline(cin, id);
    cout << "Masukkan Saran Diet Baru: ";
    getline(cin, saranBaru);

    string query = "UPDATE manajemen_rekomendasi SET saran_diet = '" + saranBaru + "' WHERE id_rekomendasi = " + id;
    
    if (mysql_query(conn, query.c_str())) {
        cout << "Gagal mengupdate data: " << mysql_error(conn) << endl;
    } else {
        cout << "Data berhasil diupdate!" << endl;
    }
}

void manajemenDiet(MYSQL *conn) {
    system("cls");
    cout << "\n====== MANAJEMEN REKOMENDASI DIET ======\n";
    cout << "1. Lihat Rekomendasi\n";
    cout << "2. Tambah Rekomendasi\n";
    cout << "3. Update Rekomendasi\n";
    cout << "4. Hapus Rekomendasi" << endl;
    cout << "Pilih: ";
    string pil; getline(cin, pil);

    if (pil == "1") {
        Table tbl;
        tbl.add_row({"ID", "Kategori BMI", "Saran Diet"});
        
        mysql_query(conn, "SELECT * FROM manajemen_rekomendasi");
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        
        while ((row = mysql_fetch_row(res))) {
            tbl.add_row({row[0], row[1], row[2]});
        }
        cout << "\n" << tbl << endl;
        mysql_free_result(res);
    } 
    else if (pil == "2") {
        string kategori, saran;
        cout << "Kategori BMI (misal: Overweight): "; getline(cin, kategori);
        cout << "Saran Diet: "; getline(cin, saran);
        
        string query = "INSERT INTO manajemen_rekomendasi (kategori_bmi, saran_diet) VALUES ('" + kategori + "', '" + saran + "')";
        
        if (mysql_query(conn, query.c_str())) cout << "Gagal tambah: " << mysql_error(conn);
        else cout << "Berhasil ditambah!";
    }
    else if (pil == "3") {
        string id;
        cout << "ID Rekomendasi yang dihapus: "; getline(cin, id);
        
        string query = "DELETE FROM manajemen_rekomendasi WHERE id_rekomendasi = " + id;
        
        if (mysql_query(conn, query.c_str())) cout << "Gagal hapus!";
        else cout << "Data terhapus (jika ID tersedia).";
    }
}

void konfirmasiRequest(MYSQL* conn) {
    system("cls");
    cout << "\n====== KONFIRMASI REQUEST USER ======\n\n";

    // 1. Tampilkan daftar request yang statusnya masih 'Pending'
    Table tbl;
    tbl.add_row({"ID Req", "ID User", "Nama Makanan", "Status"});
    
    string querySelect = "SELECT id_request, id_user, nama_makanan_req, status_request FROM request_user WHERE status_request = 'Pending'";
    mysql_query(conn, querySelect.c_str());
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;

    int jmlData = mysql_num_rows(res);
    if (jmlData == 0) {
        cout << "Tidak ada request user yang perlu dikonfirmasi.\n";
        mysql_free_result(res);
        return;
    }

    while ((row = mysql_fetch_row(res))) {
        tbl.add_row({row[0], row[1], row[2], row[3]});
    }
    cout << tbl << endl;
    mysql_free_result(res);

    // 2. Proses Konfirmasi
    string idReq, status;
    cout << "\nMasukkan ID Request yang ingin diproses: "; getline(cin, idReq);
    cout << "Pilih Status (1. Terima / 2. Tolak): "; getline(cin, status);

    string statusFinal = (status == "1") ? "Diterima" : "Ditolak";
    string queryUpdate = "UPDATE request_user SET status_request = '" + statusFinal + "' WHERE id_request = " + idReq;

    if (mysql_query(conn, queryUpdate.c_str())) {
        cout << "Gagal memperbarui status: " << mysql_error(conn) << endl;
    } else {
        cout << "Request ID " << idReq << " telah " << statusFinal << "!" << endl;
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
    adminMenu.add_row({"0", "Logout / Kembali"});

    adminMenu[0].format().font_align(FontAlign::center).font_style({FontStyle::bold});
    for (size_t i = 1; i <= 10; ++i) {
        adminMenu[i][0].format().font_align(FontAlign::center);
    }
    cout << adminMenu << endl;
}
void searchingMenu(MYSQL* conn) {
    system("cls");
    cout << "\n====== MENU SEARCHING (LINEAR SEARCH) ======\n\n";
    
    string cari;
    cout << "Masukkan Nama Makanan yang dicari: ";
    getline(cin, cari);

    string query = "SELECT nama_makanan, kategori, kalori FROM makanan";
    if (mysql_query(conn, query.c_str())) {
        cout << "Error: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    bool ditemukan = false;

    Table hasilCari;
    hasilCari.add_row({"Nama Makanan", "Kategori", "Kalori"});

    while ((row = mysql_fetch_row(res))) {
        string namaDiDB = row[0];
        // Linear search sederhana menggunakan find
        if (namaDiDB.find(cari) != string::npos) { 
            hasilCari.add_row({row[0], row[1], row[2]});
            ditemukan = true;
        }
    }

    if (ditemukan) {
        cout << "\nHasil Pencarian:\n" << hasilCari << endl;
    } else {
        cout << "\nData '" << cari << "' tidak ditemukan." << endl;
    }
    mysql_free_result(res);
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
            konfirmasiRequest(conn);
            cout << "\nTekan enter untuk kembali..." << endl;
            cin.get();
        } else if (pilihan == "6") {
            readLog(conn);
            cout << "\nTekan enter untuk kembali..." << endl;
            cin.get();
        } else if (pilihan == "7") {
            cout << "\non progress..." << endl;
            cin.get();
        } else if (pilihan == "8") {
            searchingMenu(conn);
            cout << "\nTekan enter untuk kembali..." << endl;
            cin.get();
        } else if (pilihan == "9") {
            manajemenDiet(conn);
            cout << "\noTekan enter untuk kembali..." << endl;
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