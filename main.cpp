#include <winsock.h>
#include <iostream>
#include <mysql.h>
#include <conio.h>
#include <string>
#include "tabulate/table.hpp"
#include "autentikasi.h"
#include "admin.h"

using namespace std;
using namespace tabulate;

int main() {
    MYSQL* conn = mysql_init(0);

    unsigned int ssl_mode = SSL_MODE_DISABLED;
    mysql_options(conn, MYSQL_OPT_SSL_MODE, &ssl_mode);
<<<<<<< HEAD

    mysql_options(conn, MYSQL_PLUGIN_DIR, "./plugin");
=======
    
    // Sesuaikan path plugin jika diperlukan
    mysql_options(conn, MYSQL_PLUGIN_DIR, "C:/laragon/bin/mysql/mysql-8.0.30-winx64/lib/plugin");
>>>>>>> a0d542daca9ed3bb2269ddb2ac4fda5cbe81d587

    if (!mysql_real_connect(conn, "127.0.0.1", "root", "", "db_gizi", 3306, NULL, 0)) {
        cout << "Koneksi Database Gagal: " << mysql_error(conn) << endl;
        return 1;
    }

    string pilihan;

    while (true) {
        system("cls");

        if (!isTerdaftar) {
            cout << "\n========= SELAMAT DATANG DI SIGIZI =========\n\n";

            Table mainMenu;
            mainMenu.add_row({"No", "Menu Utama"});
            mainMenu.add_row({"1", "Login"});
            mainMenu.add_row({"2", "Register"});
            mainMenu.add_row({"0", "Logout"});

            mainMenu[0].format().font_align(FontAlign::center);
            for (size_t i = 1; i <= 3; ++i) {
                mainMenu[i][0].format().font_align(FontAlign::center);
            }

            cout << mainMenu << endl;
            cout << "Silakan pilih menu yang tersedia : ";
            getline(cin, pilihan);

            if (pilihan == "1") {
                login(conn, 3);
            } else if (pilihan == "2") {
                registerUser(conn);
            } else if (pilihan == "0") {
                cout << "Terima kasih telah menggunakan aplikasi ini" << endl;
                break;
            } else {
                cout << "Pilihan tidak valid, silakan tekan enter...";
                cin.get();
            }
        } 

        else {
            if (userRole == "admin") {
                menuAdmin(conn); 
            } 
            else if (userRole == "user") {
                cout << "\n====== DASHBOARD GIZI ======\n";
                cout << "Selamat datang, " << user << endl;
                cout << "otw kak" << endl;
            }
        }
    }

    mysql_close(conn);
    return 0;
}