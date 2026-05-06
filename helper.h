#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>
#include <mysql.h>
#include "tabulate/table.hpp"
#include <sstream>

using namespace std;
using namespace tabulate;

// 1. Struktur Data Terpusat (Harus ada di paling atas)
struct DataMakanan {
    int id;
    string nama;
    string kategori;
    float kalori;
    float protein;
    float karbohidrat;
    float lemak;
};

// 2. Fungsi Bantuan (Gunakan inline)
inline void swapData(DataMakanan* a, DataMakanan* b) {
    DataMakanan temp = *a;
    *a = *b;
    *b = temp;
}

inline string formatFloat(float val) {
    stringstream ss;
    ss << val;
    return ss.str();
}

inline int fetchMakananToArray(MYSQL* conn, DataMakanan arr[], int max_size) {
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

inline void tabelMakanan(DataMakanan arr[], int n) {
    if (n == 0) {
        cout << "Tidak ada data makanan.\n";
        return;
    }
    Table tbl;
    
    tbl.add_row({"No", "ID", "Nama Makanan", "Kategori", "Kalori (kcal)", "Protein (g)", "Karbo (g)", "Lemak (g)"});
    tbl.row(0).format().font_align(FontAlign::center);

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

    for (size_t i = 1; i <= (size_t)n; ++i) {
        tbl[i][0].format().font_align(FontAlign::center); 
        tbl[i][1].format().font_align(FontAlign::center); 
        tbl[i][4].format().font_align(FontAlign::center); 
        tbl[i][5].format().font_align(FontAlign::center); 
        tbl[i][6].format().font_align(FontAlign::center); 
        tbl[i][7].format().font_align(FontAlign::center); 
    }

    cout << tbl << endl;
}

#endif