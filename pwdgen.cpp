#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>

namespace options{

    class Options{
        std::string LOWER_CASE;
        std::string UPPER_CASE;
        std::string SYMBOLS;
        std::string NUMBERS;
        unsigned int NUMBER_OF_OPTIONS = 4;

        int get_random(const int &MAX){
            unsigned int number = std::rand() % MAX;
            return number;
        }

        public:
            Options(){
                LOWER_CASE = "abcdefghijklmnopqrstuvxwyz";
                UPPER_CASE = "ABCDEFGHIJKLMNOPQRSTUVXWYZ";
                SYMBOLS = "\'\"@#$%&*()_-=+`{^~}<>:\\/;.,";
                NUMBERS = "0123456789";
            }

            char pick_a_char(){
                const std::string OPTIONS[4] = {LOWER_CASE, UPPER_CASE, SYMBOLS, NUMBERS};
                unsigned short int list_source_of_symbol = get_random(NUMBER_OF_OPTIONS);
                unsigned short int word_source_of_symbol = get_random(OPTIONS[list_source_of_symbol].size());
                return OPTIONS[list_source_of_symbol][word_source_of_symbol];
            }
    };
}

namespace pwd{

    class Pwd{
        options::Options options;
        const short int LIMIT_PERCENTAGE_REPETITION = 0.2;
        const short int MINIMUM_PWD_SIZE = 8;

        public:
            Pwd(){}

            bool check_for_repetition(const std::string s){
                unsigned int char_counter = 0;

                for(int i=0; i < s.size(); i++){

                    for(int j=0; j < s.size(); j++){
                        if(s[i] == s[j]) char_counter++;
                    }

                    if(char_counter / s.size() > LIMIT_PERCENTAGE_REPETITION) return false;

                    std::cout << char_counter << '\n';
                    char_counter = 0;
                } 

                return true;
            }

            bool verify_pwd_size(const int &size){ return size < MINIMUM_PWD_SIZE; }

            std::string generate_pwd(const int &size){
                std::string pwd = "";

                if(verify_pwd_size(size)) return "";

                for(int i = 0; i < size; i++){
                    pwd += options.pick_a_char();
                }

                return pwd;
            }
    };
}

namespace menu{
    void help_str(){ std::cout << "Press H for help\n"; }

    void display_menu(){ std::cout << "1- Set password size\n2- Generate new password\n3- Close\n"; }

    int get_input(){
        int input;
        std::cin>> input;

        return input;
    }
}

int main(){
    char input;
    std::string s;
    int size = 0;
    pwd::Pwd p;
    std::srand(std::time(nullptr));

    while(true){
        menu::help_str();
        std::cin>> input;

        if(input == 'h' || input == 'H'){
            menu::display_menu();
        }

        else if(input == '1'){
            std::cout << "Password size: ";
            size = menu::get_input();
        }

        else if(input == '2'){
            s = p.generate_pwd(size);
            std::cout << "Password generated: " << s << std::endl;
        }

        else if(input == '3'){
            exit(0);
        }

        else{
            std::cout << "Invalid option\n";
        }
    }


    return 0;
}