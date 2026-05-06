#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include <mysql.h>
#include <vector>
#include <algorithm>
#include "tabulate/table.hpp"
#include "helper.h" // Memanggil struktur data dari helper

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
                   nama + "', '" + kategori + "', " + formatFloat(kalori) + ", " + formatFloat(protein) + ", " + 
                   formatFloat(karbohidrat) + ", " + formatFloat(lemak) + ")";

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

// =============================================
// MENU 5: KONFIRMASI REQUEST USER
// =============================================
void konfirmasiRequest(MYSQL* conn) {
    system("cls");
    cout << "\n====== KONFIRMASI REQUEST USER ======\n\n";

    string querySelect = "SELECT id_request, id_user, nama_makanan_req, status_request FROM request_user WHERE status_request = 'Pending'";
    mysql_query(conn, querySelect.c_str());
    MYSQL_RES* res = mysql_store_result(conn);

    if (mysql_num_rows(res) == 0) {
        cout << "Tidak ada request user yang perlu dikonfirmasi.\n";
        mysql_free_result(res);
        return;
    }

    Table tbl;
    tbl.add_row({"No", "ID Request", "ID User", "Nama Makanan Diminta", "Status"});
    tbl.row(0).format().font_align(FontAlign::center).font_style({FontStyle::bold});

    MYSQL_ROW row;
    int no = 1;
    while ((row = mysql_fetch_row(res))) {
        tbl.add_row({to_string(no++), row[0], row[1], row[2], row[3]});
    }
    cout << tbl << "\n";
    mysql_free_result(res);

    string idReq;
    cout << "Masukkan ID Request yang ingin diproses (0 = batal): ";
    getline(cin, idReq);
    if (idReq == "0") return;

    string cekQuery = "SELECT id_request FROM request_user WHERE id_request = " + idReq + " AND status_request = 'Pending'";
    mysql_query(conn, cekQuery.c_str());
    MYSQL_RES* cekRes = mysql_store_result(conn);
    if (mysql_num_rows(cekRes) == 0) {
        cout << "ID Request tidak ditemukan atau sudah diproses.\n";
        mysql_free_result(cekRes);
        return;
    }
    mysql_free_result(cekRes);

    string aksi;
    cout << "Pilih aksi:\n";
    cout << "  1. Terima\n";
    cout << "  2. Tolak\n";
    cout << "Pilihan: ";
    getline(cin, aksi);

    if (aksi != "1" && aksi != "2") {
        cout << "Pilihan tidak valid.\n";
        return;
    }

    string statusFinal = (aksi == "1") ? "Diterima" : "Ditolak";
    string queryUpdate = "UPDATE request_user SET status_request = '" + statusFinal +
                         "' WHERE id_request = " + idReq;

    if (mysql_query(conn, queryUpdate.c_str())) {
        cout << "Gagal memperbarui status: " << mysql_error(conn) << endl;
    } else {
        cout << "\nRequest ID " << idReq << " berhasil di" << statusFinal << "!\n";
        string logQuery = "INSERT INTO log_user (id_user, aktivitas) VALUES ('" +
                          user + "', 'Admin " + statusFinal + " request ID " + idReq + "')";
        mysql_query(conn, logQuery.c_str());
    }
}

// =============================================
// MENU 6: LOG USER
// =============================================
void readLog(MYSQL* conn) {
    system("cls");
    cout << "\n====== LOG AKTIVITAS USER ======\n\n";

    string query = "SELECT id_log, id_user, aktivitas, waktu FROM log_user ORDER BY waktu DESC";
    if (mysql_query(conn, query.c_str())) {
        cout << "Error ambil log: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;

    if (mysql_num_rows(res) == 0) {
        cout << "Belum ada riwayat aktivitas.\n";
        mysql_free_result(res);
        return;
    }

    Table tblLog;
    tblLog.add_row({"No", "ID Log", "ID User", "Aktivitas", "Waktu"});
    tblLog.row(0).format().font_align(FontAlign::center).font_style({FontStyle::bold});

    int no = 1;
    while ((row = mysql_fetch_row(res))) {
        tblLog.add_row({to_string(no), row[0], row[1], row[2], row[3]});
        tblLog[no][0].format().font_align(FontAlign::center);
        no++;
    }
    cout << tblLog << endl;
    mysql_free_result(res);
}

// =============================================
// MENU 8: SEARCHING
// =============================================
int binarySearchBahan(const vector<DataMakanan>& arr, const string& x) {
    int low = 0, high = (int)arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid].nama == x) return mid;
        if (arr[mid].nama < x) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int linearSearchAkun(const vector<string>& arr, const string& x) {
    for (int i = 0; i < (int)arr.size(); i++) {
        if (arr[i] == x) return i;
    }
    return -1;
}

void linearSearchLog(const vector<pair<string,string>>& logs, const string& x) {
    bool found = false;
    Table tbl;
    tbl.add_row({"ID User", "Aktivitas"});
    tbl.row(0).format().font_style({FontStyle::bold});

    for (const auto& log : logs) {
        string aktivitasLower = log.second;
        string xLower = x;
        transform(aktivitasLower.begin(), aktivitasLower.end(), aktivitasLower.begin(), ::tolower);
        transform(xLower.begin(), xLower.end(), xLower.begin(), ::tolower);
        if (aktivitasLower.find(xLower) != string::npos) {
            tbl.add_row({log.first, log.second});
            found = true;
        }
    }

    if (found) {
        cout << "\nHasil Pencarian Log '" << x << "':\n";
        cout << tbl << endl;
    } else {
        cout << "\nData log dengan aktivitas '" << x << "' tidak ditemukan.\n";
    }
}

void searchingMenu(MYSQL* conn) {
    system("cls");
    cout << "\n====== MENU SEARCHING ======\n\n";
    cout << "1. Cari Nama Bahan Makanan (Binary Search)\n";
    cout << "2. Cari Nama Akun (Linear Search)\n";
    cout << "3. Cari Log Aktivitas (Linear Search)\n";
    cout << "0. Kembali\n";
    cout << "Pilih: ";
    string sub;
    getline(cin, sub);

    if (sub == "1") {
        vector<DataMakanan> listData;
        mysql_query(conn, "SELECT nama_makanan, kalori FROM makanan ORDER BY nama_makanan ASC");
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            
            DataMakanan dm;
            dm.nama = row[0];
            dm.kalori = stof(row[1]);
            listData.push_back(dm);
            
        }
        mysql_free_result(res);

        if (listData.empty()) {
            cout << "Belum ada data makanan di database.\n";
            return;
        }

        cout << "\nMasukkan nama bahan makanan yang dicari (harus persis): ";
        string cari;
        getline(cin, cari);

        int hasil = binarySearchBahan(listData, cari);
        if (hasil != -1) {
            cout << "\n[DITEMUKAN]\n";
            Table tbl;
            tbl.add_row({"Nama Makanan", "Kalori (kcal)"});
            tbl.add_row({listData[hasil].nama, formatFloat(listData[hasil].kalori)});
            cout << tbl << endl;
        } else {
            cout << "\nBahan '" << cari << "' tidak ditemukan.\n";
            cout << "Catatan: Binary Search memerlukan nama yang persis sama.\n";
        }

    } else if (sub == "2") {
        vector<string> listAkun;
        mysql_query(conn, "SELECT username FROM users");
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) listAkun.push_back(row[0]);
        mysql_free_result(res);

        if (listAkun.empty()) {
            cout << "Belum ada data akun di database.\n";
            return;
        }

        cout << "\nMasukkan nama akun yang dicari: ";
        string cari;
        getline(cin, cari);

        int hasil = linearSearchAkun(listAkun, cari);
        if (hasil != -1) cout << "\n[DITEMUKAN] Akun '" << cari << "' terdaftar di sistem.\n";
        else cout << "\nAkun '" << cari << "' tidak ditemukan.\n";

    } else if (sub == "3") {
        vector<pair<string,string>> logs;
        mysql_query(conn, "SELECT id_user, aktivitas FROM log_user ORDER BY waktu DESC");
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            logs.push_back({row[0], row[1]});
        }
        mysql_free_result(res);

        if (logs.empty()) {
            cout << "Belum ada log aktivitas.\n";
            return;
        }

        cout << "\nMasukkan kata kunci aktivitas yang dicari: ";
        string cari;
        getline(cin, cari);
        linearSearchLog(logs, cari);

    } else if (sub == "0") {
        return;
    } else {
        cout << "Pilihan tidak valid.\n";
    }

    cout << "\nTekan enter untuk kembali...";
    cin.get();
}

// =============================================
// MENU 9: MANAJEMEN REKOMENDASI DIET
// =============================================
void manajemenDiet(MYSQL* conn) {
    string pil;
    while (true) {
        system("cls");
        cout << "\n====== MANAJEMEN REKOMENDASI DIET ======\n\n";
        cout << "1. Lihat Semua Rekomendasi\n";
        cout << "2. Tambah Rekomendasi\n";
        cout << "3. Update Rekomendasi\n";
        cout << "4. Hapus Rekomendasi\n";
        cout << "0. Kembali\n";
        cout << "Pilih: ";
        getline(cin, pil);

        if (pil == "1") {
            mysql_query(conn, "SELECT * FROM manajemen_rekomendasi");
            MYSQL_RES* res = mysql_store_result(conn);
            if (mysql_num_rows(res) == 0) {
                cout << "\nBelum ada data rekomendasi diet.\n";
            } else {
                Table tbl;
                tbl.add_row({"ID", "Kategori BMI", "Saran Diet"});
                tbl.row(0).format().font_align(FontAlign::center).font_style({FontStyle::bold});
                MYSQL_ROW row;
                int i = 1;
                while ((row = mysql_fetch_row(res))) {
                    tbl.add_row({row[0], row[1], row[2]});
                    tbl[i][0].format().font_align(FontAlign::center);
                    i++;
                }
                cout << "\n" << tbl << endl;
            }
            mysql_free_result(res);
            cout << "\nTekan enter untuk kembali...";
            cin.get();

        } else if (pil == "2") {
            string kategori, saran;
            cout << "\nKategori BMI (misal: Underweight, Normal, Overweight, Obesitas): ";
            getline(cin, kategori);
            if (kategori.empty()) { cout << "Kategori tidak boleh kosong.\n"; cin.get(); continue; }
            cout << "Saran Diet: ";
            getline(cin, saran);
            if (saran.empty()) { cout << "Saran diet tidak boleh kosong.\n"; cin.get(); continue; }
            string query = "INSERT INTO manajemen_rekomendasi (kategori_bmi, saran_diet) VALUES ('" +
                           kategori + "', '" + saran + "')";
            if (mysql_query(conn, query.c_str())) cout << "Gagal tambah: " << mysql_error(conn) << endl;
            else cout << "\nRekomendasi berhasil ditambahkan!\n";
            cout << "Tekan enter untuk kembali...";
            cin.get();

        } else if (pil == "3") {
            mysql_query(conn, "SELECT id_rekomendasi, kategori_bmi, saran_diet FROM manajemen_rekomendasi");
            MYSQL_RES* res = mysql_store_result(conn);
            if (mysql_num_rows(res) == 0) {
                cout << "\nBelum ada data rekomendasi.\n";
                mysql_free_result(res);
                cin.get();
                continue;
            }
            Table tbl;
            tbl.add_row({"ID", "Kategori BMI", "Saran Diet"});
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) tbl.add_row({row[0], row[1], row[2]});
            mysql_free_result(res);
            cout << "\n" << tbl << endl;

            string id, saranBaru;
            cout << "Masukkan ID Rekomendasi yang ingin diupdate (0 = batal): ";
            getline(cin, id);
            if (id == "0") continue;

            string cekQ = "SELECT id_rekomendasi FROM manajemen_rekomendasi WHERE id_rekomendasi = " + id;
            mysql_query(conn, cekQ.c_str());
            MYSQL_RES* cekRes = mysql_store_result(conn);
            if (mysql_num_rows(cekRes) == 0) {
                cout << "ID tidak ditemukan.\n";
                mysql_free_result(cekRes);
                cin.get();
                continue;
            }
            mysql_free_result(cekRes);

            cout << "Masukkan Saran Diet Baru: ";
            getline(cin, saranBaru);
            if (saranBaru.empty()) { cout << "Saran tidak boleh kosong.\n"; cin.get(); continue; }

            string query = "UPDATE manajemen_rekomendasi SET saran_diet = '" + saranBaru +
                           "' WHERE id_rekomendasi = " + id;
            if (mysql_query(conn, query.c_str())) cout << "Gagal update: " << mysql_error(conn) << endl;
            else cout << "\nRekomendasi berhasil diupdate!\n";
            cout << "Tekan enter untuk kembali...";
            cin.get();

        } else if (pil == "4") {
            mysql_query(conn, "SELECT id_rekomendasi, kategori_bmi, saran_diet FROM manajemen_rekomendasi");
            MYSQL_RES* res = mysql_store_result(conn);
            if (mysql_num_rows(res) == 0) {
                cout << "\nBelum ada data rekomendasi.\n";
                mysql_free_result(res);
                cin.get();
                continue;
            }
            Table tbl;
            tbl.add_row({"ID", "Kategori BMI", "Saran Diet"});
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) tbl.add_row({row[0], row[1], row[2]});
            mysql_free_result(res);
            cout << "\n" << tbl << endl;

            string id;
            cout << "Masukkan ID Rekomendasi yang akan dihapus (0 = batal): ";
            getline(cin, id);
            if (id == "0") continue;

            string konfirm;
            cout << "Yakin hapus ID " << id << "? (y/n): ";
            getline(cin, konfirm);
            if (konfirm != "y" && konfirm != "Y") { cout << "Penghapusan dibatalkan.\n"; cin.get(); continue; }

            string query = "DELETE FROM manajemen_rekomendasi WHERE id_rekomendasi = " + id;
            if (mysql_query(conn, query.c_str())) cout << "Gagal hapus: " << mysql_error(conn) << endl;
            else {
                if (mysql_affected_rows(conn) > 0) cout << "\nRekomendasi berhasil dihapus!\n";
                else cout << "ID tidak ditemukan.\n";
            }
            cout << "Tekan enter untuk kembali...";
            cin.get();

        } else if (pil == "0") {
            break;
        } else {
            cout << "Pilihan tidak valid.\n";
            cin.get();
        }
    }
}

// =============================================
// TABEL MENU ADMIN
// =============================================
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

// =============================================
// MENU ADMIN UTAMA
// =============================================
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
            cout << "\nTekan enter untuk kembali..."; cin.get();
        } else if (pilihan == "6") {
            readLog(conn);
            cout << "\nTekan enter untuk kembali..."; cin.get();
        } else if (pilihan == "7") {
            cout << "\nFitur sorting sedang dikerjakan...\n";
            cin.get();
        } else if (pilihan == "8") {
            searchingMenu(conn);
        } else if (pilihan == "9") {
            manajemenDiet(conn);
        } else if (pilihan == "0") {
            isTerdaftar = false;
            user = "";
            userRole = "";
            cout << "\nTekan enter untuk melanjutkan..."; cin.get();
            break;
        } else {
            cout << "\nPilihan tidak valid, silakan coba lagi\n";
            cout << "Tekan enter untuk melanjutkan..."; cin.get();
        }
    }
}

#endif