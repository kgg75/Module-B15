#pragma once

#include "Source.h"
//#include <test.h>
#include <iostream>
#include <iomanip>
#include <string>

#define SIZE 1
#define LOGINLENGTH 10


class Chat {
    public:
        Chat();
        ~Chat();
        void reg(char _login[LOGINLENGTH], char _pass[], size_t pass_length);
        bool login(char _login[LOGINLENGTH], char _pass[], size_t pass_length);
    private:
        struct AuthData {
            char login[LOGINLENGTH];
            uint* pass_sha1_hash;
            AuthData();
            ~AuthData();
            void addData(const char _login[], uint* _pass_sha1_hash);
        };
        AuthData* data;
        int data_count {0}; // количество добавленных пар ключ-значение
        int mem_size {SIZE}; // количество выделенной памяти
        void resize();
};