#include<iostream>
#include "pwdgen.cpp"

class Menu
{
public:
    Menu() {};

    void help_str() { std::cout << "Press H for help\n"; }

    void display_menu()
    {
        std::cout << "1- Set password size\n"
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
            if (menu.validate_numeric_input(size))
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