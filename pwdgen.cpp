#include<iostream>
#include<cstdlib>
#include<random>
#include<string>

namespace options{

    class RNG{
        private:
            std::random_device rd;
            std::mt19937 gen{rd()};
        public:
            RNG() : gen(rd()) {}

            int get(const int &max){
                std::uniform_int_distribution<> dist(0, max - 1);
                return dist(gen);
            }

    };

    struct Alphabet {
        const std::string lower = "abcdefghijklmnopqrstuvwxyz";
        const std::string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const std::string numbers = "0123456789";
        const std::string symbols = "!@#$%^&*()-_=+[]{}<>?/|";
        const std::string all[4] = {lower,upper, numbers, symbols};
    };

    struct PasswordPolicy {
        bool useLower = true;
        bool useUpper = true;
        bool useNumbers = true;
        bool useSymbols = true;
        bool char_sets[4] = {useLower, useUpper, useNumbers, useSymbols};

        int pwd_length = 16;
        int pin_length = 8;

        std::string CUSTOM_to_string(){
            std::string s = "";
            s += "Lowercase : " + useLower + '\n';
            s += "Uppercase : " + useUpper + '\n';
            s += "Numbers : " + useNumbers + '\n';
            s += "Symbols : " + useSymbols + '\n';
            std::cout << s << std::endl;
            return s;
        }
    };

    class Options{
        Alphabet ab;
        PasswordPolicy pwp;

        public:
            Options(){}

            Alphabet get_alphabet(){
                return ab;
            }

            PasswordPolicy get_password_policy(){
                return pwp;
            }

            void set_password_policy(const int &n){
                for(int i = 0; i < 4; i++){
                    if(i == n){
                        pwp.char_sets[i] = !pwp.char_sets[i];
                    }
                }
            }
            int get_pwd_length(){
                return pwp.pwd_length;
            }
            int get_pin_length(){
                return pwp.pin_length;
            }

            int set_pwd_length(const int &n){
                pwp.pwd_length = n;
                return n;
            }
            int set_pin_length(const int &n){
                pwp.pin_length = n;
                return n;
            }
    };
}

namespace pwd{

    class Pwd{
        options::Options options = options::Options();
        options::RNG rng = options::RNG();
       // const short int LIMIT_PERCENTAGE_REPETITION = 0.2;

        char pick_a_char(){
            std::string allowed;

            for(int i=0; i < 4; i++){
                if(options.get_password_policy().char_sets[i]){
                    allowed += options.get_alphabet().all[i];
                }
            }
            if(allowed.empty())
            throw std::runtime_error("No characters allowed");

            return allowed[rng.get(allowed.size())];
        }

        char pick_a_number(){
            return options.get_alphabet().numbers[rng.get(options.get_alphabet().numbers.size())];
        }

        public:
            Pwd(){}
            //bool check_for_repetition(const std::string &s){}

            std::string generate_pwd(){
                std::string pwd = "";

                for(int i = 0; i < options.get_pwd_length(); i++){
                    pwd += pick_a_char();
                }

                return pwd;
            }

            std::string generate_pin(){
                std::string pin = "";

                for(int i = 0; i< options.get_pin_length(); i++){
                    pin += pick_a_number();
                }
                return pin;
            }

            std::string get_policy(){
                return options.get_password_policy().CUSTOM_to_string();
            }

            void set_policy(const int &n){
                options.set_password_policy(n);
            }

            int change_pwd_policy(const int &input){ 
                if(input == '0'){
                    std::cout << get_policy() << std::endl;
                }
                if(input == '1'){
                    set_policy(0);   
                }
                if(input == '2'){
                    set_policy(1);
                }
                if(input == '3'){
                    set_policy(2);
                }
                if(input == '4'){
                    set_policy(3);
                }

                return input;
            }

    };
}

class Menu{
    public:
    Menu(){};

    void help_str(){ std::cout << "Press H for help\n"; }

    void display_menu(){ 
        std::cout << 
        "1- Set password size\n2- Generate new password\n3- Generate a PIN\n4- Set PIN size\n5- Close\n6- Change password policy\n";
    }

    int get_input(){
        int input;
        std::cin>> input;

        return input;
    }

    void display_change_of_pwd_policy_menu(){
        std::cout << "0-See current policy\n"
                  << "1 - Set lowercase letters On/Off\n"
                  << "2- Set uppercase letters On/Off\n"
                  << "3- Set numbers On/Off\n"
                  << "4- Set symbols On/Off\n"
                  << "5- Return to main menu\n";
    }
};

int main(){
    char input;
    std::string s;
    int size = 0;
    pwd::Pwd p;
    Menu menu = Menu();

    while(true){
        menu.help_str();
        std::cin>> input;

        if(input == 'h' || input == 'H'){
            menu.display_menu();
        }

        else if(input == '1'){
            std::cout << "Password size: ";
            size = menu.get_input();
        }

        else if(input == '2'){
            s = p.generate_pwd();
            std::cout << "Password generated: " << s << std::endl;
        }

        else if(input == '3'){
            s = p.generate_pin();
            std::cout << "PIN generated: "<< s << std::endl;
        }
        else if(input == '4'){
            std::cout << "PIN size: ";
            size = menu.get_input();
        }
        else if(input == '5') exit(0);

        else if(input == '6'){
            do{
                menu.display_change_of_pwd_policy_menu();
            }while(p.change_pwd_policy(menu.get_input()) != 5);
        }

        else{
            std::cout << "Invalid option\n";
        }
    }


    return 0;
}