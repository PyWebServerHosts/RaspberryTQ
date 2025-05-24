#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <iomanip>

class Pin {
public:
    Pin(int pinNumber, bool mode) : pinNumber(pinNumber), mode(mode), state(false) {}
    void on() { state = true; }
    void off() { state = false; }
    void value(bool state) { this->state = state; }
private:
    int pinNumber;
    bool mode;
    bool state;
};

class RTC {
public:
    void datetime(std::tuple<int, int, int, int, int, int, int, int> dt) {
        std::tie(year, month, day, _, hour, minute, second, _) = dt;
    }
    std::tuple<int, int, int, int, int, int, int> datetime() {
        return std::make_tuple(year, month, day, 0, hour, minute, second);
    }
private:
    int year, month, day, hour, minute, second;
};

Pin led(25, true);
RTC rtc;
bool led_state = false;

void sleep(double duration) {
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(duration * 1000)));
}

void blink_led(int times = 1, double duration = 0.2) {
    for (int i = 0; i < times; ++i) {
        led.on();
        sleep(duration);
        led.off();
        sleep(duration);
    }
}

void show_time() {
    auto now = rtc.datetime();
    std::cout << "Current time: " 
              << std::setw(2) << std::setfill('0') << std::get<4>(now) << ":"
              << std::setw(2) << std::setfill('0') << std::get<5>(now) << ":"
              << std::setw(2) << std::setfill('0') << std::get<6>(now) << std::endl;
}

void set_rtc_from_command(const std::vector<std::string>& parts) {
    try {
        if (parts.size() != 7) {
            std::cout << "Usage: set_time YYYY MM DD HH MM SS" << std::endl;
            return;
        }
        int year = std::stoi(parts[1]);
        int month = std::stoi(parts[2]);
        int day = std::stoi(parts[3]);
        int hour = std::stoi(parts[4]);
        int minute = std::stoi(parts[5]);
        int second = std::stoi(parts[6]);
        rtc.datetime(std::make_tuple(year, month, day, 0, hour, minute, second, 0));
        std::cout << "RTC time set successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error setting time: " << e.what() << std::endl;
    }
}

void print_main_menu() {
    std::cout << "\n======== RaspberryTQ ========" << std::endl;
    std::cout << "Type one of the following commands:" << std::endl;
    std::cout << "1 or time           → Show current time" << std::endl;
    std::cout << "2 or blink N        → Blink LED N times" << std::endl;
    std::cout << "3 or shell          → Enter custom shell" << std::endl;
    std::cout << "4 or toggle         → Toggle LED ON/OFF" << std::endl;
    std::cout << "5 or set_time Y M D H M S" << std::endl;
    std::cout << "6 or exit           → Stop the program" << std::endl;
    std::cout << "=============================\n" << std::endl;
}

void custom_shell() {
    std::cout << "Entering custom shell. Type 'exit' to return." << std::endl;
    std::cout << "Available: help, time, led_on, led_off, blink, exit" << std::endl;

    while (true) {
        std::cout << "cstuser@RaspberryTQ >>> ";
        std::string cmd;
        std::getline(std::cin, cmd);
        cmd = cmd.substr(0, cmd.find('\n'));
        if (cmd == "exit") {
            std::cout << "Exiting shell." << std::endl;
            break;
        } else if (cmd == "help") {
            std::cout << "Commands: time, led_on, led_off, blink, exit" << std::endl;
        } else if (cmd == "time") {
            show_time();
        } else if (cmd == "led_on") {
            led.on();
            std::cout << "LED turned ON." << std::endl;
        } else if (cmd == "led_off") {
            led.off();
            std::cout << "LED turned OFF." << std::endl;
        } else if (cmd == "blink") {
            blink_led();
        } else {
            std::cout << "Unknown command." << std::endl;
        }
    }
}

int main() {
    print_main_menu();
    while (true) {
        std::cout << "RaspberryTQ> ";
        std::string line;
        std::getline(std::cin, line);
        line = line.substr(0, line.find('\n'));
        std::istringstream iss(line);
        std::vector<std::string> parts((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

        if (parts.empty()) {
            continue;
        }

        std::string cmd = parts[0];

        if (cmd == "1" || cmd == "time") {
            show_time();
            blink_led(1);
        } else if (cmd == "2" || cmd == "blink") {
            try {
                int times = (parts.size() > 1) ? std::stoi(parts[1]) : 1;
                blink_led(times);
            } catch (...) {
                std::cout << "Usage: blink [times]" << std::endl;
            }
        } else if (cmd == "3" || cmd == "shell") {
            custom_shell();
        } else if (cmd == "4" || cmd == "toggle") {
            led_state = !led_state;
            led.value(led_state);
            std::cout << "LED is now " << (led_state ? "ON" : "OFF") << "." << std::endl;
        } else if (cmd == "5" || cmd == "set_time") {
            set_rtc_from_command(parts);
        } else if (cmd == "6" || cmd == "exit") {
            std::cout << "Exiting RaspberryTQ. Goodbye!" << std::endl;
            break;
        } else {
            std::cout << "Unknown command. Type again." << std::endl;
            print_main_menu();
        }
    }
    return 0;
}

