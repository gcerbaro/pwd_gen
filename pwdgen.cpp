#include<iostream>
#include<cstdlib>
#include<random>
#include<string>
#include<map>

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
        const std::map<std::string, std::string> char_sets = {
            {"lower", "abcdefghijklmnopqrstuvwxyz"},
            {"upper", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"},
            {"numbers", "0123456789"},
            {"symbols", "!@#$%^&*()-_=+[]{}<>?/|"}
        };
        /*
        const std::string lower = "abcdefghijklmnopqrstuvwxyz";
        const std::string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const std::string numbers = "0123456789";
        const std::string symbols = "!@#$%^&*()-_=+[]{}<>?/|";
        const std::string all[4] = {lower,upper, numbers, symbols};*/
    };

    struct PasswordPolicy {
        std::map<std::string, bool> char_sets = {
            {"lower", true},
            {"upper", true},
            {"numbers", true},
            {"symbols", true}
        };
        
        int pwd_length = 16;
        int pin_length = 8;

        std::string to_string(){
            std::string s = "";
            for(auto &it : char_sets){
                s += it.first + ": " + (it.second ? "On" : "Off") + "\n";
            }
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

            void set_password_policy(std::string key){
                for(auto &it: pwp.char_sets){
                    if(it.first == key){
                        it.second = !it.second;
                        break;
                    }
                }
            }
            int get_pwd_length(){
                return pwp.pwd_length;
            }
            int get_pin_length(){
                return pwp.pin_length;
            }

            bool set_pwd_length(const int &n){
                pwp.pwd_length = n;
                return (pwp.pwd_length == n)? true : false;
            }
            bool set_pin_length(const int &n){
                pwp.pin_length = n;
                return (pwp.pin_length == n)? true : false;
            }
    };
}

namespace pwd{

    class Pwd{
        options::Options options = options::Options();
        options::RNG rng = options::RNG();

        char pick_a_char(){
            std::string allowed;
            for(auto &it : options.get_password_policy().char_sets){
                if(it.second){
                    allowed += options.get_alphabet().char_sets.at(it.first);
                }
            }
            if(allowed.empty())
            throw std::runtime_error("No characters allowed");

            return allowed[rng.get(allowed.size())];
        }

        char pick_a_number(){
            return options.get_alphabet().char_sets.at("numbers")[rng.get(options.get_alphabet().char_sets.at("numbers").size())];
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
                return options.get_password_policy().to_string();
            }

            void set_pwd_length(const int &n){
                if(options.set_pwd_length(n)) std::cout << "Password length set to " << n << std::endl;
            }
            void set_pin_length(const int &n){
                if(options.set_pin_length(n)) std::cout << "PIN length set to " << n << std::endl;
            }

            void set_policy(std::string key){
                options.set_password_policy(key);
            }

            int change_pwd_policy(const int &input){ 
                if(input == 0){
                    std::cout << get_policy() << std::endl;
                }
                else if(input == 1){
                    set_policy("lower");
                }
                else if(input == 2){
                    set_policy("upper");
                }
                else if(input == 3){
                    set_policy("numbers");
                }
                else if(input == 4){
                    set_policy("symbols");
                }
                else{
                    std::cout << "Option " << input << " is invalid\n";
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
        std::cout << "0- See current policy\n"
                  << "1- Set lowercase letters On/Off\n"
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