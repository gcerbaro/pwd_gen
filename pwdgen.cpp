#include <iostream>
#include <cstdlib>
#include <random>
#include <string>
#include <map>

class RNG
{
private:
    std::random_device rd;

public:
    RNG() {}

    int get(int max)
    {
        if (max <= 0)
        {
            throw std::invalid_argument("max must be > 0");
        }

        std::uniform_int_distribution<int> dist(0, max - 1);
        return dist(rd);
    }
};

class Alphabet
{
    const std::map<std::string, std::string> char_sets = {
        {"lower", "abcdefghijklmnopqrstuvwxyz"},
        {"upper", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"},
        {"numbers", "0123456789"},
        {"symbols", "!@#$%^&*()-_=+[]{}<>?/|"}};

public:
    Alphabet() {}

    std::map<std::string, std::string> get_alphabet()
    {
        return char_sets;
    }
};

class PasswordPolicy
{
    std::map<std::string, bool> curr_policy = {
        {"lower", true},
        {"upper", true},
        {"numbers", true},
        {"symbols", true}};

    int pwd_length = 16;
    int pin_length = 8;

public:
    PasswordPolicy() {}

    std::string to_string()
    {
        std::string s = "";
        for (auto &it : curr_policy)
        {
            s += it.first + ": " + (it.second ? "On" : "Off") + "\n";
        }
        return s;
    }

    std::map<std::string, bool> get_curr_policy()
    {
        return curr_policy;
    }

    void set_policy(std::string key)
    {
        for (auto &it : curr_policy)
        {
            if (it.first == key)
            {
                it.second = !it.second;
                break;
            }
        }
    }

    int get_pwd_length()
    {
        return pwd_length;
    }
    int get_pin_length()
    {
        return pin_length;
    }

    void set_pwd_length(const int &n)
    {
        pwd_length = n;
    }
    void set_pin_length(const int &n)
    {
        pin_length = n;
    }
};

class Pwd
{
    PasswordPolicy pwp = PasswordPolicy();
    Alphabet ab = Alphabet();

    RNG rng = RNG();

    char pick_a_char()
    {
        std::string allowed;
        for (auto &it : pwp.get_curr_policy())
        {
            if (it.second)
            {
                allowed += ab.get_alphabet().at(it.first);
            }
        }
        if (allowed.empty())
            throw std::runtime_error("No characters allowed");

        return allowed[rng.get(allowed.size())];
    }

    char pick_a_number()
    {
        return ab.get_alphabet().at("numbers")[rng.get(ab.get_alphabet().at("numbers").size())];
    }

    bool verify_policy(const std::string &s)
    {
        std::map<std::string, bool> policy = pwp.get_curr_policy();
        std::map<std::string, bool> found = {
            {"lower", false},
            {"upper", false},
            {"numbers", false},
            {"symbols", false}};

        for (char c : s)
        {
            for (auto &it : policy)
            {
                if (it.second && ab.get_alphabet().at(it.first).find(c) != std::string::npos)
                {
                    found[it.first] = true;
                }
            }
        }

        for (auto &it : policy)
        {
            if (it.second && !found[it.first])
            {
                return false;
            }
        }

        return true;
    }

public:
    Pwd() {}

    std::string generate_pwd()
    {
        std::string pwd = "";

        for (int i = 0; i < pwp.get_pwd_length(); i++)
        {
            pwd += pick_a_char();
        }

        if (verify_policy(pwd))
        {
            return pwd;
        }
        else
        {
            return generate_pwd();
        }
        return pwd;
    }

    std::string generate_pin()
    {
        std::string pin = "";

        for (int i = 0; i < pwp.get_pin_length(); i++)
        {
            pin += pick_a_number();
        }
        return pin;
    }

    std::string get_policy()
    {
        return pwp.to_string();
    }

    void set_pwd_length(const int &n)
    {
        pwp.set_pwd_length(n);
    }
    void set_pin_length(const int &n)
    {
        pwp.set_pin_length(n);
    }

    void set_policy(std::string key)
    {
        pwp.set_policy(key);
    }

    bool validate_numeric_input(const int &input)
    {
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return false;
        }
        else
            return true;
    }

    bool switch_pwd_policy(const int &input)
    {
        if (input == 1)
        {
            set_policy("lower");
        }
        else if (input == 2)
        {
            set_policy("upper");
        }
        else if (input == 3)
        {
            set_policy("numbers");
        }
        else if (input == 4)
        {
            set_policy("symbols");
        }
        else{ 
            return false;
        }

        return true;
    }
};

class Menu
{
public:
    Menu() {};

    void help_str() { std::cout << "Press H for help\n"; }

    void display_menu()
    {
        std::cout << "0- See current policy\n"
                  << "1- Set password size\n"
                  << "2-  Generate new password\n"
                  << "3- Generate a PIN\n"
                  << "4- Set PIN size\n"
                  << "5- Close\n"
                  << "6- Change password policy\n";
    }

    int get_input()
    {
        int input;
        std::cin >> input;

        return input;
    }

    void display_change_of_pwd_policy_menu()
    {
        std::cout << "0- See current policy\n"
                  << "1- Set lowercase letters On/Off\n"
                  << "2- Set uppercase letters On/Off\n"
                  << "3- Set numbers On/Off\n"
                  << "4- Set symbols On/Off\n"
                  << "5- Return to main menu\n";
    }

    int change_pwd_policy(const int &input, Pwd &p)
    {
        
        if(input == 0){
            std::cout << p.get_policy() << std::endl;
            return input;
        }
        else if(input == 5)
        {
            std::cout << "Returning to main menu\n";
            return input;
        }
        else if(p.switch_pwd_policy(input))
        {
            return input;
        }
         else
        {
            std::cout << "Option " << input << " is invalid\n";
            return -1;
        }   
    }
};

int main()
{
    char input;
    std::string s;
    int size = 0;
    Pwd p;
    Menu menu = Menu();

    while (true)
    {
        menu.help_str();
        std::cin >> input;

        if (input == 'h' || input == 'H')
        {
            menu.display_menu();
        }

        else if (input == '1')
        {
            std::cout << "Password size: ";
            size = menu.get_input();
            if (p.validate_numeric_input(size))
            {
                if (size <= 0)
                {
                    std::cout << "Password size must be greater than 0\n";
                    continue;
                }
                p.set_pwd_length(size);
            }
            else
            {
                std::cout << "Invalid input. Please enter a numeric value.\n";
            }
        }

        else if (input == '2')
        {
            s = p.generate_pwd();
            std::cout << "Password generated: " << s << std::endl;
        }

        else if (input == '3')
        {
            s = p.generate_pin();
            std::cout << "PIN generated: " << s << std::endl;
        }
        else if (input == '4')
        {
            std::cout << "PIN size: ";
            size = menu.get_input();
            p.set_pin_length(size);
        }
        else if (input == '5')
            exit(0);

        else if (input == '6')
        {
            do
            {
                menu.display_change_of_pwd_policy_menu();
            } while (menu.change_pwd_policy(menu.get_input(),p ) != 5);
        }

        else
        {
            std::cout << "Invalid option\n";
        }
    }

    return 0;
}