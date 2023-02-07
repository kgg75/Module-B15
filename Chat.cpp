#include "Chat.h"
//#include <string.h>
//#include <cstdint>


Chat::Chat() {
    data = new AuthData[mem_size];
    memset(data, 0, sizeof(AuthData) * mem_size);
}

Chat::~Chat() { delete[] data; }

Chat::AuthData::AuthData() : login(""), pass_sha1_hash(0) {}

Chat::AuthData::~AuthData() {
    if (pass_sha1_hash != 0)
        delete[] pass_sha1_hash;
}

int hash_func_quad(char _login[LOGINLENGTH], int _mem_size, int _offset) {
    int sum = 0, i = 0;    // вычисляем индекс
    for (; i < strlen(_login); i++) {
        sum *= _login[i];
    }
    return (sum % _mem_size + _offset * _offset) % _mem_size;    // квадратичные пробы
}

void Chat::AuthData::addData(const char _login[], uint* _pass_sha1_hash) {
    strcpy_s(login, _login);
    if (pass_sha1_hash != 0)
        delete[] pass_sha1_hash;
    pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];
    memcpy(pass_sha1_hash, _pass_sha1_hash, SHA1HASHLENGTHBYTES); 
}

void Chat::resize() {
    AuthData* saved_data = data; // запоминаем старый массив
    int oldSize = mem_size;

    mem_size *= 2;  // увеличиваем размер в два раза  
    data = new AuthData[mem_size]; // выделяем новую память

    for (int i = 0; i < oldSize; i++) {
        if (saved_data[i].login[0] != 0) {
            for (int j = 0; j < mem_size; j++) {
                int index = hash_func_quad(saved_data[i].login, mem_size, j);
                if (data[index].login[0] == 0) { // найдена пустая ячейка, занимаем ее
                    data[index].addData(saved_data[i].login, saved_data[i].pass_sha1_hash);
                    break;
                }
            }
        }
    }

    delete[] saved_data;    // чистим за собой
}


void Chat::reg(char _login[LOGINLENGTH], char _pass[], size_t pass_length) {
    if (data_count == mem_size)
        resize();

    int index, i = 0;

    for (; i < mem_size; i++) {    // берем пробы по всем i от 0 до размера массива
        index = hash_func_quad(_login, mem_size, i);
        if (data[index].login[0] == 0) { // найдена пустая ячейка, занимаем ее
            break;
        }
    }

    data[index].addData(_login, sha1(_pass, pass_length));    // кладем в свободную ячейку пару
    data_count++;
}


bool Chat::login(char _login[LOGINLENGTH], char _pass[], size_t pass_length) {
    int index, i;

    for (i = 0; i < mem_size; i++) {
        index = hash_func_quad(_login, mem_size, i);
        if (strcmp(data[index].login, _login) == 0) // login совпадает
            break;
    }

    if (i >= mem_size) // login не найден
        return false;

    uint* shaPwdArray = sha1(_pass, pass_length);

    for (i = 0; i < SHA1HASHLENGTHUINTS; i++)
        std::cout << std::hex << std::setw(8) << std::setfill('0') << shaPwdArray[i] << " ";
    std::cout << '\n';

    for (i = 0; i < SHA1HASHLENGTHUINTS; i++)
        std::cout << std::hex << std::setw(8) << std::setfill('0') << data[index].pass_sha1_hash[i] << " ";
    std::cout << '\n';

    i = 0;
    while (i < SHA1HASHLENGTHUINTS)
        if (data[index].pass_sha1_hash[i] != shaPwdArray[i++])
            break;

    delete[] shaPwdArray;

    return i >= SHA1HASHLENGTHUINTS;    // если i меньше - сравнение не прошло    // Нужно вернуть true в случае успешного логина
}