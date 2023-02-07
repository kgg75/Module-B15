#include <iomanip>
#include "Source.h"
#include "Chat.h"


int main() {
    Chat chat;
    char _login[LOGINLENGTH];
    char _pass[LOGINLENGTH * 2];
    size_t pass_length;

    for (int i = 0; i < 3; i++) {
        std::cout << "Registration:\nNew name: ";
        std::cin.getline(_login, LOGINLENGTH);
        std::cout << "Password: ";
        std::cin.getline(_pass, LOGINLENGTH * 2);
        pass_length = strlen(_pass);

        chat.reg(_login, _pass, pass_length);
        std::cout << "----------------------------------\n";
    }

    bool result;

    for (int i = 0; i < 3; i++) {
        std::cout << "----------------------------------\nLogin:\nLogin name: ";
        std::cin.getline(_login, LOGINLENGTH);
        std::cout << "Password: ";
        std::cin.getline(_pass, LOGINLENGTH * 2);
        pass_length = strlen(_pass);//std::cin.gcount() - 1;

        result = chat.login(_login, _pass, pass_length);
        std::cout << "Login: " << (result ? "OK!" : "wrong login-pwd!") << '\n';
    }

    return 0;
}