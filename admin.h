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
    string nama, kategori; 
    float kalori, protein, karbohidrat, lemak;

    while (true) {
        try {
            cout << "Nama Makanan : "; 
            getline(cin, nama);
            if (nama.empty()) throw invalid_argument("Nama makanan tidak boleh kosong");
            
            if (nama.find_first_of("0123456789") != string::npos) {
                throw invalid_argument("Nama makanan tidak boleh mengandung angka!");
            }
            break; 
        } catch (const invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\nSilakan isi kembali.\n\n";
        }
    }

    while (true) {
        try {
            cout << "\nPilih Kategori Makanan:\n";
            
            Table tabelKategori;
            tabelKategori.add_row({"No", "Kategori Makanan"});
            tabelKategori.add_row({"1", "Lauk pauk"});
            tabelKategori.add_row({"2", "Cemilan"});
            tabelKategori.add_row({"3", "Makanan utama"});
            tabelKategori.add_row({"4", "Buah"});
            tabelKategori.add_row({"5", "Appetizer"});

            tabelKategori[0].format().font_align(FontAlign::center);
            for (size_t i = 1; i <= 5; ++i) {
                tabelKategori[i][0].format().font_align(FontAlign::center);
            }
            cout << tabelKategori << endl;
            cout << "Masukkan pilihan (1-5): "; 
            
            string pilKat;
            getline(cin, pilKat);
            
            if (pilKat == "1") { kategori = "Lauk pauk"; 
                break; }
            else if (pilKat == "2") { kategori = "Cemilan"; 
                break; }
            else if (pilKat == "3") { kategori = "Makanan utama"; 
                break; }
            else if (pilKat == "4") { kategori = "Buah"; 
                break; }
            else if (pilKat == "5") { kategori = "Appetizer"; 
                break; }
            else {
                throw invalid_argument("Pilihan kategori tidak valid. Pilih harus angka 1-5.");
            }
        } catch (const invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\nSilakan isi kembali.\n";
        }
    }

    auto inputFloat = [](const string& prompt) -> float {
        string inputStr;
        while(true) {
            try {
                cout << prompt;
                getline(cin, inputStr);
                if (inputStr.empty()) throw invalid_argument("Input tidak boleh kosong");
                
                size_t pos;
                float val = stof(inputStr, &pos); 
                
                if (pos != inputStr.length()) throw invalid_argument("Input harus berupa angka murni");
                return val; 
                
            } catch (const invalid_argument& e) {
                string msg = e.what();
                if (msg == "stof") cout << "[ERROR] Input tidak valid. Harus berupa angka.\n\n";
                else cout << "[ERROR] " << msg << "\n\n";
            } catch (...) {
                cout << "[ERROR] Input tidak valid atau angka terlalu besar\n\n";
            }
        }
    };

    cout << endl; 
    kalori = inputFloat("Jumlah Kalori : ");
    protein = inputFloat("Jumlah Protein (g) : ");
    karbohidrat = inputFloat("Jumlah Karbohidrat (g) : ");
    lemak = inputFloat("Jumlah Lemak (g) : ");

    string query = "INSERT INTO makanan (nama_makanan, kategori, kalori, protein, karbohidrat, lemak) VALUES ('" + 
                   nama + "', '" + kategori + "', " + formatFloat(kalori) + ", " + formatFloat(protein) + ", " + 
                   formatFloat(karbohidrat) + ", " + formatFloat(lemak) + ")";

    if (mysql_query(conn, query.c_str())) cout << "Gagal menyimpan data: " << mysql_error(conn) << endl;
    else cout << "\nData Makanan berhasil ditambahkan ke database" << endl;
}

inline void editData(MYSQL* conn) {
    readData(conn);
    string targetId; 

    while (true) {
        try {
            cout << "ps : Jika tidak ingin mengubah data makanan tertentu, cukup tekan enter";
            cout << "\nMasukkan ID Makanan yang akan diedit (ketik 0 untuk batal): "; 
            getline(cin, targetId);
            
            if (targetId == "0") 
            return; 
            if (targetId.empty()) throw invalid_argument("ID Makanan tidak boleh kosong!");

            if (targetId.find_first_not_of("0123456789") != string::npos) {
                throw invalid_argument("ID Makanan tidak valid. Harus berupa angka murni");
            }

            string checkQ = "SELECT * FROM makanan WHERE id_makanan = " + targetId;
            mysql_query(conn, checkQ.c_str());
            MYSQL_RES* res = mysql_store_result(conn);
            
            if (mysql_num_rows(res) == 0) { 
                mysql_free_result(res); 
                throw invalid_argument("Data dengan ID tersebut tidak ditemukan!");
            }
            
            mysql_free_result(res);
            break; 
            
        } catch (const invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\n";
        } catch (...) {
            cout << "[ERROR] Terjadi kesalahan saat membaca ID.\n";
        }
    }

    string namaBaru, kategoriBaru, kaloriBaru, proteinBaru, karboBaru, lemakBaru; 
    
    while (true) {
        try {
            cout << "\nNama makanan : "; 
            getline(cin, namaBaru);
            if (!namaBaru.empty() && namaBaru.find_first_of("0123456789") != string::npos) {
                throw invalid_argument("Nama makanan tidak boleh mengandung angka");
            }
            break;
        } catch (const invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\n";
        }
    }

    while (true) {
        try {
            cout << "\nUbah Kategori Makanan:\n";
            
            Table tabelKategori;
            tabelKategori.add_row({"No", "Kategori Makanan"});
            tabelKategori.add_row({"1", "Lauk pauk"});
            tabelKategori.add_row({"2", "Cemilan"});
            tabelKategori.add_row({"3", "Makanan utama"});
            tabelKategori.add_row({"4", "Buah"});
            tabelKategori.add_row({"5", "Appetizer"});

            tabelKategori[0].format().font_align(FontAlign::center);
            for (size_t i = 1; i <= 5; 
                ++i) {
                tabelKategori[i][0].format().font_align(FontAlign::center);
            }

            cout << tabelKategori << endl;
            cout << "Pilih menu diantara 1-5 : "; 
            
            string pilKat;
            getline(cin, pilKat);
            
            if (pilKat.empty()) { kategoriBaru = ""; 
                break; } 
            else if (pilKat == "1") { kategoriBaru = "Lauk pauk"; 
                break; }
            else if (pilKat == "2") { kategoriBaru = "Cemilan"; 
                break; }
            else if (pilKat == "3") { kategoriBaru = "Makanan utama"; 
                break; }
            else if (pilKat == "4") { kategoriBaru = "Buah"; 
                break; }
            else if (pilKat == "5") { kategoriBaru = "Appetizer"; 
                break; }
            else {
                throw invalid_argument("Pilihan kategori tidak valid! Masukkan angka 1-5 atau kosongkan.");
            }
        } catch (const invalid_argument& e) {
            cout << "\n[ERROR] " << e.what() << "\n";
        }
    }

    auto editFloatOpsional = [](const string& prompt, string& varRef) {
        while (true) {
            try {
                cout << prompt;
                getline(cin, varRef);
                
                if (varRef.empty()) break; 
                
                size_t pos;
                stof(varRef, &pos); 
                
                if (pos != varRef.length()) throw invalid_argument("Input harus angka murni!");
                break; 
                
            } catch (...) {
                cout << "[ERROR] Input harus berupa angka! Silakan isi kembali.\n";
            }
        }
    };

    cout << endl;
    editFloatOpsional("Kalori baru : ", kaloriBaru);
    editFloatOpsional("Protein baru (g) : ", proteinBaru);
    editFloatOpsional("Karbohidrat baru (g) : ", karboBaru);
    editFloatOpsional("Lemak baru (g) : ", lemakBaru);

    string updateQuery = "UPDATE makanan SET "; bool isUpdate = false;
    if (!namaBaru.empty()) { updateQuery += "nama_makanan = '" + namaBaru + "'"; isUpdate = true; }
    if (!kategoriBaru.empty()) { 
        if (isUpdate) updateQuery += ", "; updateQuery += "kategori = '" + kategoriBaru + "'"; isUpdate = true; }
    if (!kaloriBaru.empty()) { 
        if (isUpdate) updateQuery += ", "; updateQuery += "kalori = " + kaloriBaru; isUpdate = true; }
    if (!proteinBaru.empty()) { 
        if (isUpdate) updateQuery += ", "; updateQuery += "protein = " + proteinBaru; isUpdate = true; }
    if (!karboBaru.empty()) { 
        if (isUpdate) updateQuery += ", "; updateQuery += "karbohidrat = " + karboBaru; isUpdate = true; }
    if (!lemakBaru.empty()) { 
        if (isUpdate) updateQuery += ", "; updateQuery += "lemak = " + lemakBaru; isUpdate = true; }

    updateQuery += " WHERE id_makanan = " + targetId;
    if (isUpdate) {
        if (mysql_query(conn, updateQuery.c_str())) cout << "Gagal mengupdate: " << mysql_error(conn) << endl;
        else cout << "\nData berhasil diupdate" << endl;
    } else { 
        cout << "\nTidak ada data yang diubah." << endl; 
    }
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
    
    string query = "SELECT r.id_request, r.id_user, u.username, r.nama_makanan_req, r.status_request "
                   "FROM request_user r "
                   "JOIN users u ON r.id_user = u.id "
                   "WHERE r.status_request = 'Pending'";

    mysql_query(conn, query.c_str());
    MYSQL_RES* res = mysql_store_result(conn);

    if (mysql_num_rows(res) == 0) { 
        cout << "Tidak ada request pending.\n"; 
        mysql_free_result(res); 
        return; 
    }

    Table tbl; 
    tbl.add_row({"ID Request", "ID User", "Username", "Nama Makanan", "Status"});
    for (size_t i = 0; i < 5; ++i) tbl[0][i].format().font_align(FontAlign::center);

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        tbl.add_row({row[0], row[1], row[2], row[3], row[4]});
    }
    cout << tbl << "\n"; 
    mysql_free_result(res);

    string idReq, namaMakananReq, aksi;

    while (true) {
        try {
            cout << "Masukkan ID Request (0 = batal): "; 
            getline(cin, idReq);
            
            if (idReq == "0") 
            return; 
            if (idReq.empty()) throw invalid_argument("ID Request tidak boleh kosong!");

            if (idReq.find_first_not_of("0123456789") != string::npos) {
                throw invalid_argument("ID Request harus berupa angka murni!");
            }

            string qNama = "SELECT nama_makanan_req FROM request_user WHERE id_request = " + idReq + " AND status_request = 'Pending'";
            mysql_query(conn, qNama.c_str());
            MYSQL_RES* resNama = mysql_store_result(conn);
            
            if (mysql_num_rows(resNama) == 0) {
                mysql_free_result(resNama);
                throw invalid_argument("ID Request tidak ditemukan atau statusnya bukan Pending");
            }
            
            MYSQL_ROW rowNama = mysql_fetch_row(resNama);
            namaMakananReq = rowNama[0];
            mysql_free_result(resNama);
            break; 
            
        } catch (const invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\nSilakan coba lagi.\n\n";
        }
    }

    while (true) {
        try {
            cout << "\n1. Terima | 2. Tolak \nPilihan: "; 
            getline(cin, aksi);
            
            if (aksi == "1" || aksi == "2") {
                break; 
            } else {
                throw invalid_argument("Pilihan tidak valid, Masukkan angka 1 atau 2.");
            }
        } catch (const invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\n";
        }
    }

    if (aksi == "1") {
        cout << "\n--- MELENGKAPI DATA GIZI UNTUK '" << namaMakananReq << "' ---\n";
        string kategori;
        float kalori, protein, karbohidrat, lemak;

        while (true) {
            try {
                cout << "\nPilih Kategori Makanan:\n";
                Table tabelKategori;
                tabelKategori.add_row({"No", "Kategori Makanan"});
                tabelKategori.add_row({"1", "Lauk pauk"});
                tabelKategori.add_row({"2", "Cemilan"});
                tabelKategori.add_row({"3", "Makanan utama"});
                tabelKategori.add_row({"4", "Buah"});
                tabelKategori.add_row({"5", "Appetizer"});
                tabelKategori[0].format().font_align(FontAlign::center);
                for (size_t i = 1; i <= 5; ++i) tabelKategori[i][0].format().font_align(FontAlign::center);
                
                cout << tabelKategori << endl;
                cout << "Masukkan pilihan (1-5): "; 
                string pilKat;
                getline(cin, pilKat);
                
                if (pilKat == "1") { kategori = "Lauk pauk"; 
                    break; }
                else if (pilKat == "2") { kategori = "Cemilan"; 
                    break; }
                else if (pilKat == "3") { kategori = "Makanan utama"; 
                    break; }
                else if (pilKat == "4") { kategori = "Buah"; 
                    break; }
                else if (pilKat == "5") { kategori = "Appetizer"; 
                    break; }
                else throw invalid_argument("Pilihan kategori tidak valid Harus angka 1-5.");
            } catch (const invalid_argument& e) {
                cout << "[ERROR] " << e.what() << "\n";
            }
        }

        auto inputFloat = [](const string& prompt) -> float {
            string inputStr;
            while(true) {
                try {
                    cout << prompt; getline(cin, inputStr);
                    if (inputStr.empty()) throw invalid_argument("Input tidak boleh kosong!");
                    size_t pos; float val = stof(inputStr, &pos); 
                    if (pos != inputStr.length()) throw invalid_argument("Input harus berupa angka murni!");
                    return val; 
                } catch (...) {
                    cout << "[ERROR] Input tidak valid! Harus berupa angka.\n";
                }
            }
        };

        kalori = inputFloat("Jumlah Kalori : ");
        protein = inputFloat("Jumlah Protein (g) : ");
        karbohidrat = inputFloat("Jumlah Karbohidrat (g) : ");
        lemak = inputFloat("Jumlah Lemak (g) : ");

        string qInsert = "INSERT INTO makanan (nama_makanan, kategori, kalori, protein, karbohidrat, lemak) VALUES ('" + 
                         namaMakananReq + "', '" + kategori + "', " + formatFloat(kalori) + ", " + 
                         formatFloat(protein) + ", " + formatFloat(karbohidrat) + ", " + formatFloat(lemak) + ")";
        
        if (mysql_query(conn, qInsert.c_str())) {
            cout << "\nGagal menyimpan ke katalog makanan: " << mysql_error(conn) << "\n";
            return; 
        } else {
            cout << "\n[BERHASIL] '" << namaMakananReq << "' telah ditambahkan ke katalog gizi.\n";
        }
    }

    string stat = (aksi == "1") ? "Diterima" : "Ditolak";
    string qUpdate = "UPDATE request_user SET status_request = '" + stat + "' WHERE id_request = " + idReq;
    
    if (mysql_query(conn, qUpdate.c_str())) {
        cout << "Gagal memperbarui status request: " << mysql_error(conn) << endl;
    } else {
        cout << "Status Request berhasil diubah menjadi: " << stat << ".\n";
    }
}

inline void readLog(MYSQL* conn) {
    system("cls");
    cout << "\n====== LOG AKTIVITAS USER ======\n\n";
    mysql_query(conn, "SELECT id_log, id_user, aktivitas, waktu FROM log_user ORDER BY waktu DESC");
    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) { 
        cout << "Belum ada riwayat.\n"; 
        mysql_free_result(res); 
        return; }

    Table tbl; tbl.add_row({"ID Log", "ID User", "Aktivitas", "Waktu"});
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) tbl.add_row({row[0], row[1], row[2], row[3]});
    cout << tbl << endl; mysql_free_result(res);
}

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
        while (j >= 0 && arr[j].id_request > key.id_request) { 
            arr[j + 1] = arr[j]; j = j - 1;
        }
        arr[j + 1] = key;
    }
}

inline void menuSortingAdmin(MYSQL* conn) {
    string pilihan;

    while (true) {
        system("cls");
        cout << "\n====== SORTING DATA ADMIN ======\n\n";

        Table sortMenu;
        sortMenu.add_row({"No", "Menu Sorting"});
        sortMenu.add_row({"1", "Sorting Data Makanan Berdasarkan Alfabet (A-Z) - Quick Sort"});
        sortMenu.add_row({"2", "Sorting Data Makanan Berdasarkan Kalori (Tertinggi-Terendah) - Quick Sort"});
        sortMenu.add_row({"3", "Sorting Request User (Konsep FIFO) - Insertion Sort"});
        sortMenu.add_row({"0", "Kembali ke menu sebelumnya"});

        sortMenu[0].format().font_align(FontAlign::center);
        for (size_t i = 1; i <= 4; ++i) {
            sortMenu[i][0].format().font_align(FontAlign::center);
        }
        
        cout << sortMenu << endl;
        
        try {
            cout << "Pilih opsi: ";
            getline(cin, pilihan);

            if (pilihan.empty()) throw invalid_argument("Input tidak boleh kosong!");
            if (pilihan.find_first_not_of("0123456789") != string::npos) throw invalid_argument("Input harus berupa angka!");
            if (pilihan != "0" && pilihan != "1" && pilihan != "2" && pilihan != "3") {
                throw invalid_argument("Pilihan tidak valid! Silakan masukkan angka 0 - 3.");
            }

            if (pilihan == "0") {
                return;
            } 
            else if (pilihan == "1" || pilihan == "2") {
                DataMakanan arr[100];
                int n = fetchMakananToArray(conn, arr, 100);
                if (n == 0) { 
                    cout << "Tidak ada data makanan.\n"; 
                } else {
                    if (pilihan == "1") {
                        quickSortNama(arr, 0, n - 1);
                        cout << "\n>> Hasil Sorting Alfabet (A-Z):\n"; 
                        tabelMakanan(arr, n);
                    } else {
                        quickSortKalori(arr, 0, n - 1);
                        cout << "\n>> Hasil Sorting Kalori Tertinggi-Terendah:\n"; 
                        tabelMakanan(arr, n);
                    }
                }
                cout << "\nTekan enter untuk kembali ke menu sorting...";
                cin.get();
            } 
            else if (pilihan == "3") {
                DataRequest arrReq[100];
                int n = fetchRequestToArray(conn, arrReq, 100);
                if (n == 0) { 
                    cout << "Tidak ada request user.\n"; 
                } else {
                    insertionSortRequestFIFO(arrReq, n);
                    cout << "\n>> Hasil Sorting Request User (FIFO):\n";
                    Table tbl; 
                    tbl.add_row({"ID Req", "ID User", "Nama Makanan", "Status"});
                    for (int i = 0; i < n; i++) {
                        tbl.add_row({to_string(arrReq[i].id_request), to_string(arrReq[i].id_user), arrReq[i].nama_makanan_req, arrReq[i].status_request});
                    }
                    cout << tbl << endl;
                }
                cout << "\nTekan enter untuk kembali ke menu sorting...";
                cin.get();
            }

        } catch (const invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\nTekan enter untuk mengulang...";
            cin.get();
        }
    }
}

inline void searchingMenu(MYSQL* conn) {
    string sub;
    
    while (true) {
        system("cls");
        cout << "\n====== MENU SEARCHING ======\n\n";
        
        Table searchMenu;
        searchMenu.add_row({"No", "Menu Searching"});
        searchMenu.add_row({"1", "Cari Nama Bahan Makanan (Binary Search)"});
        searchMenu.add_row({"2", "Cari Nama Akun (Linear Search)"});
        searchMenu.add_row({"3", "Cari Log Aktivitas (Linear Search)"});
        searchMenu.add_row({"0", "Kembali ke menu sebelumnya"});

        searchMenu[0].format().font_align(FontAlign::center);
        for (size_t i = 1; i <= 4; ++i) {
            searchMenu[i][0].format().font_align(FontAlign::center);
        }
        
        cout << searchMenu << endl;
        
        try {
            cout << "Pilih opsi: ";
            getline(cin, sub);

            // Validasi Input
            if (sub.empty()) throw invalid_argument("Input tidak boleh kosong!");
            if (sub.find_first_not_of("0123456789") != string::npos) throw invalid_argument("Input harus berupa angka!");
            if (sub != "0" && sub != "1" && sub != "2" && sub != "3") {
                throw invalid_argument("Pilihan tidak valid! Silakan masukkan angka 0 - 3.");
            }

            if (sub == "0") {
                return; 
            } 
            else if (sub == "1") {
                DataMakanan arr[100];
                int n = fetchMakananToArray(conn, arr, 100);
                if (n == 0) { 
                    cout << "Belum ada data makanan.\n"; 
                } else {
                    cout << "\nMasukkan nama bahan makanan: "; 
                    string cari; 
                    getline(cin, cari);

                    quickSortNama(arr, 0, n - 1); 

                    int index = binarySearchNama(arr, 0, n - 1, toLower(cari));

                    system("cls");
                    if (index != -1) {
                        cout << "\n[DITEMUKAN]\n";
                        DataMakanan hasil[1] = {arr[index]};
                        tabelMakanan(hasil, 1);
                    } else { 
                        cout << "\nBahan '" << cari << "' tidak ditemukan.\n"; 
                    }
                }
                cout << "\nTekan enter untuk kembali ke menu searching...";
                cin.get();
            } 
            else if (sub == "2") {
                cout << "Masukkan nama akun: "; 
                string cari; 
                getline(cin, cari);

                string q = "SELECT username FROM users WHERE LOWER(username) = LOWER('" + cari + "')";
                mysql_query(conn, q.c_str()); 
                MYSQL_RES* res = mysql_store_result(conn);
                
                if (mysql_num_rows(res) > 0) cout << "Akun ditemukan.\n"; 
                else cout << "Akun tidak ditemukan.\n";
                
                mysql_free_result(res);
                cout << "\nTekan enter untuk kembali ke menu searching...";
                cin.get();
            } 
            else if (sub == "3") {
                cout << "Masukkan kata kunci log: "; 
                string cari; 
                getline(cin, cari);
            
                string q = "SELECT id_user, aktivitas FROM log_user WHERE LOWER(aktivitas) LIKE LOWER('%" + cari + "%')";
                mysql_query(conn, q.c_str()); 
                MYSQL_RES* res = mysql_store_result(conn);
                
                if (mysql_num_rows(res) > 0) {
                    Table tbl; 
                    tbl.add_row({"ID User", "Aktivitas"}); 
                    MYSQL_ROW row;
                    while((row = mysql_fetch_row(res))) {
                        tbl.add_row({row[0], row[1]});
                    }
                    cout << "\n" << tbl << endl;
                } else { 
                    cout << "Log tidak ditemukan.\n"; 
                }
                
                mysql_free_result(res);
                cout << "\nTekan enter untuk kembali ke menu searching...";
                cin.get();
            }

        } catch (const invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\nTekan enter untuk mengulang...";
            cin.get();
        }
    }
}

void manajemen_rekomendasi(MYSQL* conn) {
    string pil;
    while (true) {
        system("cls");
        cout << "\n====== MANAJEMEN REKOMENDASI DIET ======\n\n";
        cout << "1. Read Semua Rekomendasi\n";
        cout << "2. Create rekomendasi\n";
        cout << "3. Update Rekomendasi\n";
        cout << "4. Hapus Rekomendasi\n";
        cout << "0. Kembali\n";
        cout << "Pilih menu yang tersedia: ";
        getline(cin, pil);

        if (pil == "1") {
            mysql_query(conn, "SELECT * FROM manajemen_rekomendasi");
            MYSQL_RES* res = mysql_store_result(conn);
            if (mysql_num_rows(res) == 0) {
                cout << "\nBelum ada data rekomendasi diet.\n";
            } else {
                Table tbl;
                tbl.add_row({"ID", "Kategori BMI", "Saran Diet"});
                tbl.row(0).format().font_align(FontAlign::center);
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
            if (kategori.empty()) { 
                cout << "Kategori tidak boleh kosong.\n"; 
                cin.get(); 
                continue; }
            cout << "Saran Diet: ";
            getline(cin, saran);
            if (saran.empty()) { 
                cout << "Saran diet tidak boleh kosong.\n"; 
                cin.get(); 
                continue; }
            string query = "INSERT INTO manajemen_rekomendasi (kategori_bmi, saran_diet) VALUES ('" +
                           kategori + "', '" + saran + "')";
            if (mysql_query(conn, query.c_str())) 
            {
                cout << "Gagal tambah: " << mysql_error(conn) << endl;
            }
            else 
            {
                cout << "\nRekomendasi berhasil ditambahkan\n";
            }
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
            if (saranBaru.empty()) { 
                cout << "Saran tidak boleh kosong.\n"; 
                cin.get(); 
                continue; }

            string query = "UPDATE manajemen_rekomendasi SET saran_diet = '" + saranBaru +
                           "' WHERE id_rekomendasi = " + id;
            if (mysql_query(conn, query.c_str())) 
            {
                cout << "Gagal update: " << mysql_error(conn) << endl;
            }
            else 
            {
                cout << "\nRekomendasi berhasil diupdate\n";
            }
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
            if (id == "0") 
            {
                continue;
            }

            string konfirm;
            cout << "Yakin hapus ID " << id << "? (y/n): ";
            getline(cin, konfirm);
            if (konfirm != "y" && konfirm != "Y") { 
                cout << "Penghapusan dibatalkan.\n"; 
                cin.get(); 
                continue; }

            string query = "DELETE FROM manajemen_rekomendasi WHERE id_rekomendasi = " + id;
            if (mysql_query(conn, query.c_str())) 
            {
                cout << "Gagal hapus: " << mysql_error(conn) << endl;
            }
            else {
                if (mysql_affected_rows(conn) > 0) {
                    cout << "\nRekomendasi berhasil dihapus!\n";}
                else {
                    cout << "ID tidak ditemukan\n";
                }
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

inline void menuAdmin(MYSQL* conn) {
    string pilihan;
    while (isTerdaftar && userRole == "admin") {
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

        for (size_t i = 1; i <= 10; ++i) adminMenu[i][0].format().font_align(FontAlign::center);
        cout << adminMenu << endl;
        
        cout << "Silakan pilih menu admin: ";
        getline(cin, pilihan);
        if(pilihan.empty()) 
        continue; 

        if (pilihan == "1") { 
            readData(conn); 
            cout << "\nTekan enter untuk melanjutkan..."; 
            cin.get(); } 
        else if (pilihan == "2") { 
            createData(conn); 
            cout << "\nTekan enter untuk melanjutkan..."; 
            cin.get(); } 
        else if (pilihan == "3") { 
            editData(conn); 
            cout << "\nTekan enter untuk melanjutkan..."; 
            cin.get(); } 
        else if (pilihan == "4") { 
            deleteData(conn); 
            cout << "\nTekan enter untuk melanjutkan..."; 
            cin.get(); } 
        else if (pilihan == "5") { 
            konfirmasiRequest(conn); 
            cout << "\nTekan enter untuk melanjutkan..."; 
            cin.get(); } 
        else if (pilihan == "6") { 
            readLog(conn); 
            cout << "\nTekan enter untuk melanjutkan..."; 
            cin.get(); } 
        else if (pilihan == "7") { 
            menuSortingAdmin(conn); 
            cout << "\nTekan enter untuk melanjutkan..."; 
            cin.get(); } 
        else if (pilihan == "8") { 
            searchingMenu(conn); 
            cout << "\nTekan enter untuk melanjutkan..."; 
            cin.get(); } 
        else if (pilihan == "9") { 
            manajemen_rekomendasi(conn); 
            cout << "\nTekan enter untuk melanjutkan..."; 
            cin.get(); }
        else if (pilihan == "0") {
            isTerdaftar = false; 
            user = ""; 
            userRole = "";
            cout << "\nTekan enter untuk melanjutkan..."; 
            cin.get(); 
            break;} 
        else { cout << "\nPilihan tidak valid. Silakan tekan enter..."; 
            cin.get(); }
    }
}
#endif