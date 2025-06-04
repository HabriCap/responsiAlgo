#include <iostream>
using namespace std;

struct PTBNode {
    string kode;
    string nama;
    int stok;
    string jenis;
    PTBNode* left;
    PTBNode* right;

    PTBNode(string k, string n, int s, string j) {
        kode = k;
        nama = n;
        stok = s;
        jenis = j;
        left = nullptr;
        right = nullptr;
    }
};

struct aksiundo {
    string tipe;
    PTBNode data;
    aksiundo* next;
};

aksiundo* top = nullptr;

PTBNode* buatnode(string k, string n, int s, string j) {
    return new PTBNode(k, n, s, j);
}
void pushundo(string tipe, PTBNode data) {
    aksiundo* baru = new aksiundo{tipe, data, top};
    top = baru;
}

bool stackkosong() {
    return top == nullptr;
}

void popundo() {
    if (!stackkosong()) {
        aksiundo* hapus = top;
        top = top->next;
        delete hapus;
    }
}

void insert(PTBNode*& akar, string k, string n, int s, string j) {
    if (akar == nullptr) {
        akar = new PTBNode(k, n, s, j);
        return;
    }

    PTBNode* current = akar;
    while (true) {
        if (k < current->kode) {
            if (current->left == nullptr) {
                current->left = new PTBNode(k, n, s, j);
                return;
            }
            current = current->left;
        } else {
            if (current->right == nullptr) {
                current->right = new PTBNode(k, n, s, j);
                return;
            }
            current = current->right;
        }
    }
}

void inorder(PTBNode* akar) {
    if (akar == nullptr) {
        return;
    }
    inorder(akar->left);
    cout << akar->kode << " | " << akar->nama << " | " << akar->stok << " | " << akar->jenis << endl;
    inorder(akar->right);
}

void postorder(PTBNode *akar){

    if(akar == nullptr){
        return;
    }

    postorder(akar->left);
    postorder(akar->right);
    cout << akar->kode << " | " << akar->nama << " | " << akar->stok << " | " << akar->jenis << endl;
}

void preorder(PTBNode *akar){
    if(akar == nullptr){
        return;
    }
    cout << akar->kode << " | " << akar->nama << " | " << akar->stok << " | " << akar->jenis << endl; 
    preorder(akar->left);
    preorder(akar->right);

}
PTBNode* searching(PTBNode* akar, string cari) {
    PTBNode* current = akar;
    while (current != nullptr) {
        if (current->kode == cari) {
            cout << cari << " ditemukan: " << current->nama << " (" << current->stok << ")\n";
            return current;
        }
        if (cari < current->kode)
            current = current->left;
        else
            current = current->right;
    }
    cout << cari << " tidak ditemukan\n";
    return nullptr;
}

void deletenode(PTBNode*& akar, string hapus, bool simpanundo = true) {
    if (akar == nullptr) return;

    if (hapus < akar->kode) {
        deletenode(akar->left, hapus);
    } else if (hapus > akar->kode) {
        deletenode(akar->right, hapus);
    } else {
        PTBNode* temp = akar;
        if (simpanundo) {
            pushundo("hapus", *akar);
        }
        if (akar->left == nullptr) {
            akar = akar->right;
        } else if (akar->right == nullptr) {
            akar = akar->left;
        } else {
            PTBNode* successor = akar->right;
            while (successor->left != nullptr)
                successor = successor->left;
            akar->kode = successor->kode;
            akar->nama = successor->nama;
            akar->stok = successor->stok;
            akar->jenis = successor->jenis;
            deletenode(akar->right, successor->kode);
            return;
        }
        delete temp;
        cout << "Data dengan kode " << hapus << " berhasil dihapus.\n";
    }
}

void undo(PTBNode*& akar) {
    if (stackkosong()) {
        cout << "Tidak ada aksi yang bisa di undo" << endl;
        return;
    }

    aksiundo* aksi = top;
    if (aksi->tipe == "tambah") {
        deletenode(akar, aksi->data.kode, false);
        cout << "Undo penambahan data produk: " << aksi->data.nama << endl;
    } else if (aksi->tipe == "hapus") {
        insert(akar, aksi->data.kode, aksi->data.nama, aksi->data.stok, aksi->data.jenis);
        cout << "Undo penghapusan data produk: " << aksi->data.nama << endl;
    }
    popundo();
}

void filterjenis(PTBNode* akar, string jenisFilter) {
    if (akar == nullptr) { 
        return;
    }
    filterjenis(akar->left, jenisFilter);
    if (akar->jenis == jenisFilter) {
        cout << akar->kode << " | " << akar->nama << " | " << akar->stok << " | " << akar->jenis << endl;
    }
    filterjenis(akar->right, jenisFilter);
}

int main() {
    PTBNode* akar = nullptr;
    int pilihan;
    char lanjut = 'y';

    do {
        cout << "Selamat datang di Panel Manajemen Gedung" << endl;
        cout << "Menu: " << endl;
        cout << "1. Tambah data produk" << endl;
        cout << "2. Tampilkan data produk" << endl;
        cout << "3. Cari data produk" << endl;
        cout << "4. Hapus data produk" << endl;
        cout << "5. Undo aksi terakhir" << endl;
        cout << "6. Keluar" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore(); 

        if (pilihan == 1) {
            string kode, nama, jenis;
            int stok;
            cout << "Masukkan kode: ";
            getline(cin, kode);
            cout << "Masukkan nama: ";
            getline(cin, nama);
            cout << "Masukkan stok: ";
            cin >> stok;
            cin.ignore();
            cout << "Masukkan jenis: ";
            getline(cin, jenis);

            insert(akar, kode, nama, stok, jenis);
            pushundo("tambah", PTBNode(kode, nama, stok, jenis));
            cout << "Data berhasil ditambahkan" << endl;

        } else if (pilihan == 2) {
            char pakaifilter;
            cout << "Tampilkan berdasarkan jenis produk? (Y/N) : ";
            cin >> pakaifilter;
            cin.ignore();

            if (pakaifilter == 'y' || pakaifilter == 'Y') {
                string jenisfilter;
                cout << "Masukkan jenis produk yang ingin ditampilkan: ";
                getline(cin, jenisfilter);
                cout << "=== produk dengan jenis \"" << jenisfilter << "\" ===" << endl;
                filterjenis(akar, jenisfilter);
            } else {
                cout << "=== Semua Produk ===" << endl;
                inorder(akar);
            }

        } else if (pilihan == 3) {
            cout << "Masukkan kode yang dicari: ";
            string kodeCari;
            getline(cin, kodeCari);
            searching(akar, kodeCari);

        } else if (pilihan == 4) {
            cout << "Masukkan kode yang ingin dihapus: ";
            string kodeHapus;
            getline(cin, kodeHapus);
            deletenode(akar, kodeHapus);

        } else if (pilihan == 5) {
            undo(akar);
        } else if (pilihan == 6) {
            cout << "terimakasih" << endl;
            return 0;
            
        }
           

         cout << "Kembali ke menu? (Y/N) : ";
         cin >> lanjut;
         cin.ignore();


    } while (lanjut == 'y' || lanjut == 'Y');

    return 0;
}
