#include <iostream>
#include <string>
#include <cmath>

using namespace std;

const int MAX_OBAT = 100;
int stokObat[MAX_OBAT];

const int MAX_USER = 100;
const int MAX_PURCHASE = 100;
const int MAX_UNDO = 100;
const int MAX_CHAR = 256; 


struct Obat {
    string nama;
    string deskripsi;
    int stok;
};

struct User {
    string username;
    string password;
    string role;
    string purchaseQueue[MAX_PURCHASE];
    int purchaseCount;
};


struct Pembelian
{
    string username;
    string namaObat;
    string deskripsi;
    int jumlah;
    string status;
};

Pembelian antrianPembelian[MAX_PURCHASE];
int pembelianCount = 0;

Obat daftarObat[MAX_OBAT];
int jumlahObat = 0;

User  userList[MAX_USER];
int jumlahUser  = 0;

bool isLoggedIn = false;
string currentRole;
User *currentUser = nullptr;

string recentlyAdded[MAX_OBAT];
int recentlyAddedCount = 0;

string undoStack[MAX_UNDO];
int undoCount = 0;

void menuUtama();
void menuAdmin();
void menuUser ();
void tambahObat();
void lihatListObat();
void beliObat();
void daftarPembelian();
void daftarPembelianUser ();
void registrasi();
void login();
void undo();
void ubahObat();
void hapusObat();
void cariDenganBoyerMoore();
void cariDenganFibonacci();
void cariDenganJumpSearch();
void mergeSort(int left, int right);
void shellSortByStockAsc();
void shellSortByStockDesc();
void quickSortByNameAsc(int low, int high);
void quickSortByNameDesc(int low, int high);


bool isValidUsername(const string& username) {
    if (username.length() < 6) {
        return false; 
    }
    for (char ch : username) {
        if (!isalnum(ch)) { 
            return false; 
        }
    }
    return true; 
}

// Fungsi untuk memvalidasi password
bool isValidPassword(const string& password) {
    if (password.length() < 6) {
        return false; 
    }

    bool hasUpper = false;
    bool hasLower = false;
    bool hasSymbol = false;

    for (char ch : password) {
        if (isupper(ch)) {
            hasUpper = true; // huruf besar
        } else if (islower(ch)) {
            hasLower = true; // huruf kecil
        } else if (ispunct(ch)) {
            hasSymbol = true; // simbol
        }
    }

    return hasUpper && hasLower && hasSymbol; 
}

void registrasi() {
    system("cls");
    string username, password;
    cout << "|================================================|\n";
    cout << "|                 Registrasi                     |\n";
    cout << "|================================================|\n\n";
    while (true) {
        cout << "Masukkan username (min 6 karakter): ";
        cin >> username;

        if (!isValidUsername(username)) {
            cout << "Username tidak valid! Pastikan username memiliki minimal 6 karakter atau tambahan angka."  << endl << endl;
            continue;
        }

       
        bool usernameExists = false;
        for (int i = 0; i < jumlahUser ; i++) {
            if (userList[i].username == username) {
                usernameExists = true;
                break;
            }
        }
        if (usernameExists) {
            cout << "Username sudah terdaftar!" << endl <<endl;
            continue; 
        }

        break; 
    }

    while (true) {
        cout << "Masukkan password (min 6 karakter, min 1 huruf besar, min 1 simbol): ";
        cin >> password;

        if (!isValidPassword(password)) {
            cout << "Password Tidak Valid!." << endl <<endl;
            continue; 
        }

        break; 
    }
    userList[jumlahUser ++] = {username, password, "user"};
    cout << "Registrasi berhasil!" << endl;
}


void login() {
    system("cls");
    string username, password;
    int percobaan = 0;

    cout << "|================================================|\n";
    cout << "|                    Login                       |\n";
    cout << "|================================================|\n\n";

    while (percobaan < 3) {
        cout << "Masukkan username: ";
        cin >> username;
        cout << "Masukkan password: ";
        cin >> password;

        for (int i = 0; i < jumlahUser ; i++) {
            if (userList[i].username == username && userList[i].password == password) {
                isLoggedIn = true;
                currentRole = userList[i].role;
                currentUser  = &userList[i];
                cout << "Login berhasil sebagai " << (currentRole == "admin" ? "Admin" : "User ") << "!" << endl << endl;

                if (currentRole == "admin")
                    menuAdmin();
                else
                    menuUser ();
                return;
            }
        }

        percobaan++;
        cout << "Username atau password salah! Percobaan ke-" << percobaan << " dari 3" << endl<<endl;
    }

    cout << "Anda telah salah memasukkan username atau password sebanyak 3 kali. Program akan berhenti" << endl;
    isLoggedIn = false;
}


Obat buatObat(string nama, string deskripsi, int stok) {
    Obat obat;
    obat.nama = nama;
    obat.deskripsi = deskripsi;
    obat.stok = stok;
    return obat;
}

bool isValidNama(const string& nama) {
    if (nama.length() > 20) {
        return false; 
    }
    for (char ch : nama) {
        if (!isalnum(ch) && ch != ' ') { 
        }
    }
    return true;
}

bool isValidDeskripsi(const string& deskripsi) {
    if (deskripsi.length() > 50) {
        return false; 
    }
    for (char ch : deskripsi) {
        if (!isalnum(ch) && ch != ' ') { 
            return false; 
        }
    }
    return true; 
}

void tambahObat() {
    string nama, deskripsi;
    int stok;

    while (true) {
        cout << "Masukkan nama obat (max 20 karakter): ";
        cin.ignore();
        getline(cin, nama);

        // Memeriksa apakah nama mengandung angka
        bool containsNumber = false;
        for (char c : nama) {
            if (c >= '0' && c <= '9') {
                containsNumber = true;
                break; // Jika ada angka, keluar dari loop
            }
        }

        if (containsNumber) {
            cout << "Nama obat tidak boleh mengandung angka." << endl << endl;
            continue; 
        }

        if (!isValidNama(nama)) {
            cout << "Pastikan tidak lebih dari 20 karakter dan hanya mengandung huruf, angka, atau spasi." << endl << endl;
            continue; 
        }

        bool obatAda = false;
        for (int i = 0; i < jumlahObat; i++) {
            if (daftarObat[i].nama == nama) {
                obatAda = true;
                cout << "Nama obat sudah ada. Apakah Anda ingin menambah stok? (ya/tidak): ";
                string pilihan;
                cin >> pilihan;

                if (pilihan == "ya") {
                    cout << "Masukkan jumlah stok yang ingin ditambahkan: ";
                    cin >> stok;
                    daftarObat[i].stok += stok;
                    cout << "Stok untuk " << nama << " berhasil ditambahkan. Stok sekarang: " << daftarObat[i].stok << endl;
                }
                return; 
            }
        }
        if (obatAda) {
            continue; // Kembali ke awal untuk memasukkan nama obat baru
        }
        break; // Jika obat tidak ada, lanjutkan untuk memasukkan deskripsi
    }

    while (true) {
        cout << "Masukkan deskripsi obat (max 50 karakter): ";
        getline(cin, deskripsi);

        if (!isValidDeskripsi(deskripsi)) {
            cout << "Pastikan tidak lebih dari 50 karakter dan hanya mengandung huruf, angka, atau spasi." << endl;
            continue;
        }
        break; 
    }

    while (true) {
        cout << "Masukkan jumlah stok (1-100): ";
        cin >> stok;

        if (cin.fail()) {
            cin.clear(); 
            cin.ignore();
            cout << "Input tidak valid! Harap masukkan angka yang benar." << endl << endl; 
            continue; 
        }

        if (stok <= 0) {
            cout << "Masukkan angka yang valid!" << endl << endl; 
            continue; 
        }

        if (stok > 100) {
            cout << "Stok maksimal 100!" << endl << endl; 
            continue; 
        }

        break; 
    }

    if (jumlahObat < MAX_OBAT) {
        daftarObat[jumlahObat++] = {nama, deskripsi, stok};  
        recentlyAdded[recentlyAddedCount++] = nama;

        if (recentlyAddedCount > MAX_OBAT) {
            recentlyAddedCount = MAX_OBAT; 
        }
    
        cout << "Obat berhasil ditambahkan." << endl;
    } else {
        cout << "Daftar obat sudah penuh!" << endl;
    }
}

void lihatListObat() {
    if (jumlahObat == 0) {
        cout << "Belum ada obat yang disimpan." << endl;
        return;
    }

    
    cout << "=================================================\n";
    cout << "                Daftar Stok Obat                 \n";
    cout << "=================================================\n";
    
    for (int i = 0; i < jumlahObat; i++) {
        cout 
        << " " << (i + 1) << "." << " Nama: " << daftarObat[i].nama 
        << "\n    Deskripsi: " << daftarObat[i].deskripsi 
        << "\n    Stok: " << daftarObat[i].stok <<endl;
    }
    cout << "=================================================\n";
    
    if (recentlyAddedCount > 0) 
    cout << "Obat yang baru ditambahkan :" << endl;
    for (int i = 0; i < recentlyAddedCount; i++) {
        cout << (i + 1) << ". " << recentlyAdded[i] << endl;
    }
}

void undo() {
    if (undoCount == 0) {
        cout << "Tidak ada tindakan yang dapat di-undo." << endl;
        return;
    }

    string lastDeletedName = undoStack[--undoCount];
    daftarObat[jumlahObat++] = buatObat(lastDeletedName, "Deskripsi Obat", 1);
    cout << "Obat " << lastDeletedName << " berhasil dikembalikan ke daftar." << endl;
}

//menggunkan queque untuk nyimpan antrian pembelian dan daftar pembelian pengguna
void daftarPembelian() {
    if (pembelianCount == 0) {
        cout << "Belum ada pembelian yang dilakukan." << endl;
    } else {
        cout << "|=================================================|\n";
        cout << "|                Daftar Pembelian                 |\n";
        cout << "|=================================================|\n";
        for (int i = 0; i < pembelianCount; i++) {
            Pembelian pembelian = antrianPembelian[i];
            cout << " Nomor Pembelian: " << (i + 1) << "\n";
            cout << " Obat: " << pembelian.namaObat << "\n";
            cout << " Deskripsi: " << pembelian.deskripsi << "\n";
            cout << " Jumlah: " << pembelian.jumlah << "\n";
            cout << " Username: " << pembelian.username << "\n";
            cout << " Status: " << pembelian.status << "\n";
            cout << "----------------------------------------------\n";
        }

        cout << "|=================================================|\n";
        cout << "Masukkan nomor pembelian yang ingin dikonfirmasi (0 untuk batal): ";
        int pilih;
        cin >> pilih;

        if (pilih > 0 && pilih <= pembelianCount) {
            Pembelian &pembelian = antrianPembelian[pilih - 1];
            if (pembelian.status == "Pending") {
                pembelian.status = "Konfirmasi";  
                cout << "Pembelian untuk " << pembelian.namaObat << " berhasil dikonfirmasi." << endl;
            } else {
                cout << "Pembelian ini sudah dikonfirmasi sebelumnya." << endl;
            }
        } else if (pilih == 0) {
            cout << "Batal konfirmasi pembelian." << endl;
        } else {
            cout << "Pilihan tidak valid!" << endl;
        }
    }
}

void ubahObat() {
    if (jumlahObat == 0) {
        cout << "Daftar masih kosong" << endl;
        return; 
    }

    lihatListObat();

    cout << "Masukkan nomor obat yang ingin diperbarui: ";
    int index;
    cin >> index;

    if (index < 1 || index > jumlahObat) {
        cout << "Nomor obat tidak valid." << endl <<endl;
        return; 
    }

    undoStack[undoCount++] = daftarObat[index - 1].nama;

    string namaBaru;
    while (true) {
        cout << "Masukkan nama obat baru (max 20 karakter): ";
        cin.ignore();
        getline(cin, namaBaru);

        if (!isValidNama(namaBaru)) {
            cout << "Pastikan tidak lebih dari 20 karakter dan hanya mengandung huruf, angka, atau spasi." << endl <<endl;
            continue; 
        }
        break;
    }
    daftarObat[index - 1].nama = namaBaru;

    string deskripsiBaru;
    while (true) {
        cout << "Masukkan deskripsi obat baru (max 50 karakter): ";
        getline(cin, deskripsiBaru);

        if (!isValidDeskripsi(deskripsiBaru)) {
            cout << "Pastikan tidak lebih dari 50 karakter dan hanya mengandung huruf, angka, atau spasi." << endl <<endl;
            continue; 
        }
        break; 
    }
    daftarObat[index - 1].deskripsi = deskripsiBaru;

    int stokBaru;
    while (true) {
        cout << "Masukkan jumlah stok baru (minimal 100): ";
        cin >> stokBaru;

        if (cin.fail() || stokBaru > 100) {
            cin.clear(); 
            cout << "Stok harus berupa angka dan minimal 100 pcs!" << endl <<endl;
            continue; 
        }
        break; 
    }
    daftarObat[index - 1].stok = stokBaru;

    cout << "Data obat berhasil diperbarui." << endl;
}

void hapusObat() {
    if (jumlahObat == 0) {
        cout << "Tidak ada data untuk dihapus." << endl;
        return;
    }

    lihatListObat();

    cout << "Masukkan nomor obat yang ingin dihapus: ";
    int index;

    while (true) {
        cin >> index;


    if (cin.fail()) {
            cin.clear(); 
            cin.ignore();
            cout << "Masukkan nomor yang valid." << endl;
            cout << "Masukkan nomor obat yang ingin dihapus: ";
            continue; 
        }

        if (index < 1 || index > jumlahObat) {
            cout << "Nomor obat tidak valid. Silakan coba lagi." << endl <<endl;
            cout << "Masukkan nomor obat yang ingin dihapus: ";
            continue; 
        }

        break; 
    }

    undoStack[undoCount++] = daftarObat[index - 1].nama;

    for (int i = index - 1; i < jumlahObat - 1; i++) {
        daftarObat[i] = daftarObat[i + 1];
    }
    jumlahObat--;
    cout << "Obat berhasil dihapus." << endl;
}


void menuAdmin()
{
    int pilihan;
    do
    {
        system("cls");
        cout << "|=======================================|\n";
        cout << "|              Menu Admin               |\n";
        cout << "|=======================================|\n";
        cout << "|       1. Tambah Obat                  |\n";
        cout << "|       2. Ubah Obat                    |\n";
        cout << "|       3. Hapus Obat                   |\n";
        cout << "|       4. Lihat Daftar Obat            |\n";
        cout << "|       5. Tampilkan Daftar Pembeli     |\n";
        cout << "|       6. Undo                         |\n";
        cout << "|       7. Urutkan Daftar Obat          |\n";
        cout << "|       8. Keluar                       |\n";
        cout << "|=======================================|\n";
        cout << "Pilih: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            tambahObat();
            break;
        case 2:
            ubahObat();
            break;
        case 3:
            hapusObat();
            break;
        case 4:
            lihatListObat();
            break;
        case 5:
            daftarPembelian();
            break;
        case 6:
            undo(); 
            break;
        case 7:
        {
            int sortOption;
            cout << "|==========================================|\n";
            cout << "|           Pilih Metode Pengurutan        |\n";
            cout << "|==========================================|\n";
            cout << "| 1. Urutkan berdasarkan stok (Ascending)  |\n";
            cout << "| 2. Urutkan berdasarkan stok (Descending) |\n";
            cout << "| 3. Urutkan berdasarkan nama (Ascending)  |\n";
            cout << "| 4. Urutkan berdasarkan nama (Descending) |\n";
            cout << "|==========================================|\n";
            cout << "Pilih: ";
            cin >> sortOption;


            switch (sortOption)
            {
            case 1:
                mergeSort(0, jumlahObat - 1); 
                break;
            case 2:
                shellSortByStockDesc(); 
                break;
            case 3:
                quickSortByNameAsc(0, jumlahObat - 1);
                break;
            case 4:
                quickSortByNameDesc(0, jumlahObat - 1); 
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
            }
            lihatListObat();
            break;
        }
        case 8:
            isLoggedIn = false;
            break;
        default:
            cout << "Pilihan tidak valid!" << endl;
        }
        cout << "Tekan Enter untuk melanjutkan...";
        cin.ignore();
        cin.get();
    } while (isLoggedIn);
}

void menuUser()
{
    int pilihan;
    do
    {
        system("cls");
        cout << "====================================================\n";
        cout << "|     Selamat Datang di Apotek Sehat Wal'afiat     |\n";
        cout << "|           Silahkan Mau Order Apa? :D             |\n";
        cout << "====================================================\n";
        cout << "|               1. Lihat Obat                      |\n";
        cout << "|               2. Beli Obat                       |\n";
        cout << "|               3. Daftar Obat yang Dibeli         |\n";
        cout << "|               4. Cari Obat                       |\n";
        cout << "|               5. Keluar                          |\n";
        cout << "|==================================================|\n";
        cout << "Pilih: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            lihatListObat();
            break;
        case 2:
            beliObat();
            break;
        case 3:
            daftarPembelianUser();
            break;
        case 4:
        {
            int metode;
            cout << "|=======================================|\n";
            cout << "|              Cari Obat                |\n";
            cout << "|=======================================|\n";
            cout << "|    1. Boyer-Moore (Nama Obat)         |\n";
            cout << "|    2. Fibonacci Search (Stok)         |\n";
            cout << "|    3. Jump Search (Stok)              |\n";
            cout << "|=======================================|\n";
            cout << "Pilihan: ";
            cin >> metode;

            for (int i = 0; i < jumlahObat; i++) {
            stokObat[i] = daftarObat[i].stok;
            }   


            if (metode == 1)
            {
                cariDenganBoyerMoore();
            }
            else if (metode == 2)
            {
                cariDenganFibonacci();
            }
            else if (metode == 3)
            {
                cariDenganJumpSearch();
            }
            else
            {
                cout << "Metode tidak valid!" << endl;
            }
            break;
        }
        case 5:
            isLoggedIn = false;
            break;
        default:
            cout << "Pilihan tidak valid!" << endl;
        }
        cout << "Tekan Enter untuk melanjutkan...";
        cin.ignore();
        cin.get();
    } while (isLoggedIn);
}

// ini menggunakan queue untuk menyimpan pembelian.
void beliObat() {
    lihatListObat();

    string nama;
    int jumlah;
    cout << "Masukkan nama obat yang ingin dibeli: ";
    cin.ignore();
    getline(cin, nama);
    cout << "Masukkan jumlah obat yang ingin dibeli: ";
    cin >> jumlah;

    for (int i = 0; i < jumlahObat; i++) {
        if (daftarObat[i].nama == nama) {
            if (daftarObat[i].stok >= jumlah) {
                daftarObat[i].stok -= jumlah;
                currentUser ->purchaseQueue[currentUser ->purchaseCount++] = nama;

                Pembelian pembelian = {currentUser ->username, nama, daftarObat[i].deskripsi, jumlah, "Pending"};
                antrianPembelian[pembelianCount++] = pembelian;

               // Menampilkan hasil akhir pembelian dalam format kotak
                cout << "================================================\n";
                cout << "               Rincian Pembelian                \n";
                cout << "================================================\n";
                cout << "         Username: " << currentUser ->username << "\n";
                cout << "         Obat: " << nama << "\n";
                cout << "         Deskripsi: " << daftarObat[i].deskripsi << "\n";
                cout << "         Jumlah: " << jumlah << "\n";
                cout << "         Status: Pending\n";;
                cout << "=================================================\n";

            } else {
                cout << "Stok tidak mencukupi untuk obat " << nama << "." << endl;
            }
            continue;
        }
    }
    cout << "Obat tidak ditemukan." << endl;
}

void daftarPembelianUser () {
    if (pembelianCount == 0) {
        cout << "Belum ada pembelian yang dilakukan." << endl;
    } else {
        cout << "================================================================= \n";
        cout << "              Daftar Pembelian untuk pengguna                    \n";
        cout << "                        " << currentUser ->username << "                     \n";
        cout << "=================================================================\n";
        
        
        for (int i = 0; i < pembelianCount; i++) {
            Pembelian pembelian = antrianPembelian[i];
            if (pembelian.username == currentUser ->username) {
                cout << " Obat: " << pembelian.namaObat << "\n";;
                cout << " Deskripsi: " << pembelian.deskripsi << "\n";;
                cout << " Jumlah: " << pembelian.jumlah << " \n";
                cout << " Status: " << pembelian.status << "\n";
                cout << "-----------------------------------------------------------------\n";
            }
        }
        cout << "|=================================================================|\n";
    }
}

void menuUtama()
{
    int pilihan;
    do
    {
        system("cls");
        cout << "====================================================\n";
        cout << "|     Selamat Datang di Apotek Sehat Wal'afiat     |\n";
        cout << "|         Silahkan Masuk Terlebih Dahulu           |\n";
        cout << "====================================================\n";
        cout << "|             1. Registrasi                        |\n";
        cout << "|             2. Login                             |\n";
        cout << "|             3. Keluar                            |\n";
        cout << "====================================================\n";
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan)
        {
        case 1:
            registrasi();
            break;
        case 2:
            login();
            break;
        case 3:
            cout << "Terima kasih! Sampai jumpa!" << endl;
            break;
        default:
            cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 3); 
}

//merge sort
void merge(Obat arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Obat L[MAX_OBAT], R[MAX_OBAT];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].stok <= R[j].stok) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(left, mid);
        mergeSort(mid + 1, right);
        merge(daftarObat, left, mid, right);
    }
}


// Shell Sort Ascending
void shellSortByStockAsc() {
    for (int gap = jumlahObat / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < jumlahObat; i++) {
            Obat temp = daftarObat[i];
            int j;
            for (j = i; j >= gap && daftarObat[j - gap].stok > temp.stok; j -= gap) {
                daftarObat[j] = daftarObat[j - gap];
            }
            daftarObat[j] = temp;
        }
    }
}

// Shell Sort Descending
void shellSortByStockDesc() {
    for (int gap = jumlahObat / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < jumlahObat; i++) {
            Obat temp = daftarObat[i];
            int j;
            for (j = i; j >= gap && daftarObat[j - gap].stok < temp.stok; j -= gap) {
                daftarObat[j] = daftarObat[j - gap];
            }
            daftarObat[j] = temp;
        }
    }
}

// Quick Sort Ascending
int partitionByNameAsc(Obat arr[], int low, int high) {
    string pivot = arr[high].nama;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].nama <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSortByNameAsc(int low, int high) {
    if (low < high) {
        int pi = partitionByNameAsc(daftarObat, low, high);
        quickSortByNameAsc(low, pi - 1);
        quickSortByNameAsc(pi + 1, high);
    }
}

// Quick Sort Descending
int partitionByNameDesc(Obat arr[], int low, int high) {
    string pivot = arr[high].nama;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].nama >= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSortByNameDesc(int low, int high) {
    if (low < high) {
        int pi = partitionByNameDesc(daftarObat, low, high);
        quickSortByNameDesc(low, pi - 1);
        quickSortByNameDesc(pi + 1, high);
    }
}


bool pilihanBenar(int pilihan)
{
    return (pilihan >= 1 && pilihan <= 8);
}

//fibonaci Search
int fibonacciSearch(int arr[], int n, int x) {

    int fib2 = 0;
    int fib1 = 1;
    int fib = fib2 + fib1;

    while (fib < n) {
        fib2 = fib1;
        fib1 = fib;
        fib = fib2 + fib1;
    }

    int offset = -1;

    while (fib > 1) {
        int i = min(offset + fib2, n - 1);


        if (arr[i] < x) {
            fib = fib1;
            fib1 = fib2;
            fib2 = fib - fib1;
            offset = i;
        }
      
        else if (arr[i] > x) {
            fib = fib2;
            fib1 -= fib2;
            fib2 = fib - fib1;
        }

        else {
            return i;
        }
    }

    if (fib1 && arr[offset + 1] == x) {
        return offset + 1;
    }

    return -1; 
}

// mencari stock obat berdasarkan jumlah stock yang dimasukkan oleh pengguna
void cariDenganFibonacci() {
    int x;
    cout << "Masukkan jumlah stok obat yang dicari: ";
    cin >> x;

    int stokObat[MAX_OBAT];
    int index = 0;

    for (int i = 0; i < jumlahObat; i++) {
        stokObat[i] = daftarObat[i].stok;
    }

    int result = fibonacciSearch(stokObat, jumlahObat, x);

    if (result == -1) {
        cout << "Obat dengan stok yang diminta tidak ditemukan." << endl;
    } else {
        cout << "Obat ditemukan: " << daftarObat[result].nama << " - Stok: " << daftarObat[result].stok << endl;
    }
}

//jump search
int jumpSearch(int arr[], int n, int x) {
    int step = sqrt(n);  
    int prev = 0;
    
    while (arr[min(step, n) - 1] < x) {
        prev = step;
        step += sqrt(n);
        if (prev >= n) {
            return -1;  
        }
    }
    
    for (int i = prev; i < min(step, n); i++) {
        if (arr[i] == x) {
            return i;
        }
    }
    return -1;  
}

// digunakan untuk mencari stok obat dengan cara yang sama seperti Fibonacci Search.
void cariDenganJumpSearch() {
    int x;
    cout << "Masukkan jumlah stok obat yang dicari: ";
    cin >> x;

    int stokObat[MAX_OBAT];
    int index = 0;

    for (int i = 0; i < jumlahObat; i++) {
        stokObat[i] = daftarObat[i].stok;
    }

    int result = jumpSearch(stokObat, jumlahObat, x);

    if (result != -1) {
        cout << "Obat ditemukan: " << daftarObat[result].nama << " - Stok: " << daftarObat[result].stok << endl;
    } else {
        cout << "Obat dengan stok yang diminta tidak ditemukan." << endl;
    }
}

//booyer more
int boyerMooreSearch(string text, string pattern)
{
    int m = pattern.length();
    int n = text.length();

    int badChar[MAX_CHAR];
    fill(badChar, badChar + MAX_CHAR, -1);  

    for (int i = 0; i < m; i++)
    {
        badChar[(int)pattern[i]] = i;
    }

    int s = 0;
    while (s <= (n - m))
    {
        int j = m - 1;

        while (j >= 0 && pattern[j] == text[s + j])
        {
            j--;
        }

        if (j < 0)
        {
            return s;
        }
        else
        {
            s += max(1, j - badChar[text[s + j]]);
        }
    }

    return -1;  
}

// digunakan untuk mencari nama obat dalam daftar obat yang ada. 
void cariDenganBoyerMoore() {
    string pattern;
    cout << "Masukkan nama obat yang ingin dicari: ";
    cin.ignore();
    getline(cin, pattern);

    bool found = false;
    for (int i = 0; i < jumlahObat; i++) {
        if (daftarObat[i].nama.find(pattern) != string::npos) {
            found = true;
            cout << "Obat ditemukan: " << daftarObat[i].nama 
                 << " - Deskripsi: " << daftarObat[i].deskripsi 
                 << " - Stok: " << daftarObat[i].stok << endl;
        }
    }

    if (!found) {
        cout << "Obat tidak ditemukan." << endl;
    }
}

void dataobat() {
    // Menambahkan beberapa data obat statis
    daftarObat[jumlahObat++] = {"Paracetamol", "Obat pereda nyeri", 50};
    daftarObat[jumlahObat++] = {"Amoxicillin", "Antibiotik", 30};
    daftarObat[jumlahObat++] = {"Ibuprofen", "Obat antiinflamasi", 20};
    daftarObat[jumlahObat++] = {"Cetirizine", "Antihistamin", 40};
    daftarObat[jumlahObat++] = {"Loperamide", "Obat diare", 25};
}

int main() {
    userList[0] = {"admin", "admin", "admin", {}, 0};
    jumlahUser  = 1; 
    dataobat();
    menuUtama();
    return 0;
}