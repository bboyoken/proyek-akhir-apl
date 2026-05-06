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
#include "helper.h" 

using namespace std;
using namespace tabulate;

extern bool isTerdaftar;
extern string user;
extern string userRole;

// Struktur khusus untuk Admin melihat Request
struct DataRequest {
    int id_request;
    int id_user;
    string nama_makanan_req;
    string status_request;
};

inline void readData(MYSQL* conn) {
    system("cls");
    cout << "\n====== DATA MAKANAN ======\n\n";
    DataMakanan arr[100];
    int n = fetchMakananToArray(conn, arr, 100);
    tabelMakanan(arr, n);
}

inline void createData(MYSQL* conn) {
    system("cls");
    cout << "\n====== TAMBAH DATA MAKANAN ======\n\n";
    string nama, kategori; float kalori, protein, karbohidrat, lemak;

    cout << "Nama Makanan : "; getline(cin, nama);
    cout << "Kategori (contoh: Buah, Lauk) : "; getline(cin, kategori);
    
    cout << "Jumlah Kalori : "; cin >> kalori;
    cout << "Jumlah Protein (g) : "; cin >> protein;
    cout << "Jumlah Karbohidrat (g) : "; cin >> karbohidrat;
    cout << "Jumlah Lemak (g) : "; cin >> lemak;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string query = "INSERT INTO makanan (nama_makanan, kategori, kalori, protein, karbohidrat, lemak) VALUES ('" + 
                   nama + "', '" + kategori + "', " + formatFloat(kalori) + ", " + formatFloat(protein) + ", " + 
                   formatFloat(karbohidrat) + ", " + formatFloat(lemak) + ")";

    if (mysql_query(conn, query.c_str())) cout << "Gagal menyimpan data: " << mysql_error(conn) << endl;
    else cout << "\nData Makanan berhasil ditambahkan ke database" << endl;
}

inline void editData(MYSQL* conn) {
    readData(conn);
    string targetId; cout << "\nMasukkan ID Makanan yang akan diedit: "; getline(cin, targetId);

    string checkQ = "SELECT * FROM makanan WHERE id_makanan = " + targetId;
    mysql_query(conn, checkQ.c_str());
    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) { cout << "Data tidak ditemukan.\n"; mysql_free_result(res); return; }
    mysql_free_result(res);

    string namaBaru, kaloriBaru, proteinBaru, karboBaru, lemakBaru; 
    cout << "Nama produk baru (Kosongkan jika tak diubah) : "; getline(cin, namaBaru);
    cout << "Kalori baru : "; getline(cin, kaloriBaru);
    cout << "Protein baru (g) : "; getline(cin, proteinBaru);
    cout << "Karbohidrat baru (g) : "; getline(cin, karboBaru);
    cout << "Lemak baru (g) : "; getline(cin, lemakBaru);

    string updateQuery = "UPDATE makanan SET "; bool isUpdate = false;
    if (!namaBaru.empty()) { updateQuery += "nama_makanan = '" + namaBaru + "'"; isUpdate = true; }
    if (!kaloriBaru.empty()) { if (isUpdate) updateQuery += ", "; updateQuery += "kalori = " + kaloriBaru; isUpdate = true; }
    if (!proteinBaru.empty()) { if (isUpdate) updateQuery += ", "; updateQuery += "protein = " + proteinBaru; isUpdate = true; }
    if (!karboBaru.empty()) { if (isUpdate) updateQuery += ", "; updateQuery += "karbohidrat = " + karboBaru; isUpdate = true; }
    if (!lemakBaru.empty()) { if (isUpdate) updateQuery += ", "; updateQuery += "lemak = " + lemakBaru; isUpdate = true; }

    updateQuery += " WHERE id_makanan = " + targetId;
    if (isUpdate) {
        if (mysql_query(conn, updateQuery.c_str())) cout << "Gagal mengupdate: " << mysql_error(conn) << endl;
        else cout << "\nData berhasil diupdate!" << endl;
    } else { cout << "\nTidak ada data yang diubah." << endl; }
}

inline void deleteData(MYSQL* conn) {
    readData(conn);
    string targetId; cout << "\nMasukkan ID Makanan yang akan dihapus: "; getline(cin, targetId);
    string query = "DELETE FROM makanan WHERE id_makanan = " + targetId;
    if (mysql_query(conn, query.c_str())) cout << "Gagal menghapus: " << mysql_error(conn) << endl;
    else cout << "Data berhasil dihapus.\n";
}

inline void konfirmasiRequest(MYSQL* conn) {
    system("cls");
    cout << "\n====== KONFIRMASI REQUEST USER ======\n\n";
    mysql_query(conn, "SELECT id_request, id_user, nama_makanan_req, status_request FROM request_user WHERE status_request = 'Pending'");
    MYSQL_RES* res = mysql_store_result(conn);

    if (mysql_num_rows(res) == 0) { cout << "Tidak ada request pending.\n"; mysql_free_result(res); return; }

    Table tbl; tbl.add_row({"ID Request", "ID User", "Nama Makanan", "Status"});
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) tbl.add_row({row[0], row[1], row[2], row[3]});
    cout << tbl << "\n"; mysql_free_result(res);

    string idReq, aksi;
    cout << "Masukkan ID Request (0 = batal): "; getline(cin, idReq);
    if (idReq == "0") return;

    cout << "1. Terima | 2. Tolak \nPilihan: "; getline(cin, aksi);
    string stat = (aksi == "1") ? "Diterima" : "Ditolak";
    string q = "UPDATE request_user SET status_request = '" + stat + "' WHERE id_request = " + idReq;
    if (mysql_query(conn, q.c_str())) cout << "Gagal.\n";
    else cout << "Request " << stat << ".\n";
}

inline void readLog(MYSQL* conn) {
    system("cls");
    cout << "\n====== LOG AKTIVITAS USER ======\n\n";
    mysql_query(conn, "SELECT id_log, id_user, aktivitas, waktu FROM log_user ORDER BY waktu DESC");
    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) { cout << "Belum ada riwayat.\n"; mysql_free_result(res); return; }

    Table tbl; tbl.add_row({"ID Log", "ID User", "Aktivitas", "Waktu"});
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) tbl.add_row({row[0], row[1], row[2], row[3]});
    cout << tbl << endl; mysql_free_result(res);
}

// FUNGSI SORTING KHUSUS ADMIN (FIFO REQUEST)
inline int fetchRequestToArray(MYSQL* conn, DataRequest arr[], int max_size) {
    string query = "SELECT id_request, id_user, nama_makanan_req, status_request FROM request_user";
    if (mysql_query(conn, query.c_str())) return 0;
    MYSQL_RES* res = mysql_store_result(conn); MYSQL_ROW row;
    int count = 0;
    while ((row = mysql_fetch_row(res)) && count < max_size) {
        arr[count].id_request = row[0] ? stoi(row[0]) : 0;
        arr[count].id_user = row[1] ? stoi(row[1]) : 0;
        arr[count].nama_makanan_req = row[2] ? row[2] : "";
        arr[count].status_request = row[3] ? row[3] : "";
        count++;
    }
    mysql_free_result(res); return count;
}

inline void insertionSortRequestFIFO(DataRequest arr[], int n) {
    for (int i = 1; i < n; i++) {
        DataRequest key = arr[i]; int j = i - 1;
        while (j >= 0 && arr[j].id_request > key.id_request) { // FIFO = Urutan ID kecil ke besar
            arr[j + 1] = arr[j]; j = j - 1;
        }
        arr[j + 1] = key;
    }
}

inline void menuSortingAdmin(MYSQL* conn) {
    system("cls");
    cout << "\n====== SORTING DATA ADMIN ======\n\n";
    cout << "1. Sorting Data Makanan Berdasarkan Alfabet (A-Z) - Quick Sort\n";
    cout << "2. Sorting Data Makanan Berdasarkan Kalori (Tertinggi-Terendah) - Quick Sort\n";
    cout << "3. Sorting Request User (Konsep FIFO) - Insertion Sort\n";
    cout << "Pilih opsi: ";
    string pilihan; getline(cin, pilihan);

    if (pilihan == "1" || pilihan == "2") {
        DataMakanan arr[100];
        int n = fetchMakananToArray(conn, arr, 100);
        if (n == 0) { cout << "Tidak ada data makanan.\n"; return; }

        if (pilihan == "1") {
            quickSortNama(arr, 0, n - 1);
            cout << "\n>> Hasil Sorting Alfabet (A-Z):\n"; tabelMakanan(arr, n);
        } else {
            quickSortKalori(arr, 0, n - 1);
            cout << "\n>> Hasil Sorting Kalori Tertinggi-Terendah:\n"; tabelMakanan(arr, n);
        }
    } else if (pilihan == "3") {
        DataRequest arrReq[100];
        int n = fetchRequestToArray(conn, arrReq, 100);
        if (n == 0) { cout << "Tidak ada request user.\n"; return; }

        insertionSortRequestFIFO(arrReq, n);
        cout << "\n>> Hasil Sorting Request User (FIFO):\n";
        Table tbl; tbl.add_row({"ID Req", "ID User", "Nama Makanan", "Status"});
        for (int i = 0; i < n; i++) tbl.add_row({to_string(arrReq[i].id_request), to_string(arrReq[i].id_user), arrReq[i].nama_makanan_req, arrReq[i].status_request});
        cout << tbl << endl;
    } else { cout << "Pilihan tidak valid!\n"; }
}

inline void searchingMenu(MYSQL* conn) {
    system("cls");
    cout << "\n====== MENU SEARCHING ======\n\n";
    cout << "1. Cari Nama Bahan Makanan (Binary Search)\n2. Cari Nama Akun (Linear Search)\n3. Cari Log Aktivitas (Linear Search)\nPilih: ";
    string sub; getline(cin, sub);

    if (sub == "1") {
        DataMakanan arr[100];
        int n = fetchMakananToArray(conn, arr, 100);
        if (n == 0) { cout << "Belum ada data makanan.\n"; return; }

        cout << "\nMasukkan nama bahan makanan (Perhatikan huruf besar/kecil): ";
        string cari; getline(cin, cari);

        quickSortNama(arr, 0, n - 1); // Wajib di-sort sebelum Binary Search
        int index = binarySearchNama(arr, 0, n - 1, cari);

        if (index != -1) {
            cout << "\n[DITEMUKAN]\n";
            DataMakanan hasil[1] = {arr[index]};
            tabelMakanan(hasil, 1);
        } else { cout << "\nBahan '" << cari << "' tidak ditemukan.\n"; }
    } else if (sub == "2") {
        cout << "Masukkan nama akun: "; string cari; getline(cin, cari);
        string q = "SELECT username FROM users WHERE username = '" + cari + "'";
        mysql_query(conn, q.c_str()); MYSQL_RES* res = mysql_store_result(conn);
        if (mysql_num_rows(res) > 0) cout << "Akun ditemukan!\n"; else cout << "Tidak ditemukan.\n";
        mysql_free_result(res);
    } else if (sub == "3") {
        cout << "Masukkan kata kunci log: "; string cari; getline(cin, cari);
        string q = "SELECT id_user, aktivitas FROM log_user WHERE aktivitas LIKE '%" + cari + "%'";
        mysql_query(conn, q.c_str()); MYSQL_RES* res = mysql_store_result(conn);
        if (mysql_num_rows(res) > 0) {
            Table tbl; tbl.add_row({"ID User", "Aktivitas"}); MYSQL_ROW row;
            while((row = mysql_fetch_row(res))) tbl.add_row({row[0], row[1]});
            cout << "\n" << tbl << endl;
        } else { cout << "Log tidak ditemukan.\n"; }
        mysql_free_result(res);
    }
}

inline void manajemenDiet(MYSQL* conn) {
    // FUNGSI INI TETAP SAMA SEPERTI MILIKMU SEBELUMNYA
    cout << "\nFitur Manajemen Diet...\n";
}

inline void menuAdmin(MYSQL* conn) {
    string pilihan;
    while (isTerdaftar && userRole == "admin") {
        system("cls");
        cout << "\n====== Menu Admin ======\n\n";
        Table adminMenu;
        adminMenu.add_row({"No", "Menu"});
        adminMenu.add_row({"1", "Read (Lihat data makanan)"}); adminMenu.add_row({"2", "Create (Tambah data makanan)"});
        adminMenu.add_row({"3", "Update (Update data makanan)"}); adminMenu.add_row({"4", "Delete (Hapus data makanan)"});
        adminMenu.add_row({"5", "Konfirmasi request user"}); adminMenu.add_row({"6", "Log user"});
        adminMenu.add_row({"7", "Menu sorting"}); adminMenu.add_row({"8", "Menu searching"});
        adminMenu.add_row({"9", "Manajemen rekomendasi diet"}); adminMenu.add_row({"0", "Kembali ke menu sebelumnya"});
        adminMenu[0].format().font_align(FontAlign::center);
        for (size_t i = 1; i <= 10; ++i) adminMenu[i][0].format().font_align(FontAlign::center);
        cout << adminMenu << endl;
        
        cout << "Silakan pilih menu admin: ";
        getline(cin, pilihan);
        if(pilihan.empty()) continue; 

        if (pilihan == "1") { readData(conn); cout << "\nTekan enter..."; cin.get(); } 
        else if (pilihan == "2") { createData(conn); cout << "\nTekan enter..."; cin.get(); } 
        else if (pilihan == "3") { editData(conn); cout << "\nTekan enter..."; cin.get(); } 
        else if (pilihan == "4") { deleteData(conn); cout << "\nTekan enter..."; cin.get(); } 
        else if (pilihan == "5") { konfirmasiRequest(conn); cout << "\nTekan enter..."; cin.get(); } 
        else if (pilihan == "6") { readLog(conn); cout << "\nTekan enter..."; cin.get(); } 
        else if (pilihan == "7") { menuSortingAdmin(conn); cout << "\nTekan enter..."; cin.get(); } 
        else if (pilihan == "8") { searchingMenu(conn); } 
        else if (pilihan == "9") { manajemenDiet(conn); } 
        else if (pilihan == "0") {
            isTerdaftar = false; user = ""; userRole = "";
            cout << "\nTekan enter untuk melanjutkan..."; cin.get(); break;
        } else { cout << "\nPilihan tidak valid. Tekan enter..."; cin.get(); }
    }
}
#endif