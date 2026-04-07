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
        {"lowercase", "abcdefghijklmnopqrstuvwxyz"},
        {"uppercase", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"},
        {"numbers", "0123456789"},
        {"symbols", "!@#$%^&*()-_=+[]{}<>?/|"}};

public:
    Alphabet() {}

    std::map<std::string, std::string> get_alphabet()
    {
        return char_sets;
    }
};

/**
 * Controls which character sets are to be used
 * values can be changed by the user to ensure best UX
 * since some systems or contexts do not allow for all types of characters
 */
class PasswordPolicy
{
    std::map<std::string, bool> curr_policy = {
        {"lowercase", true},
        {"uppercase", true},
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

    /**
     * Checks whether the requirements were met during generation
     */
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
