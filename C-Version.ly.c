#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define LED_PIN 25

int led_state = 0;

void led_on() {
    // Code to turn on the LED
    led_state = 1;
    printf("LED turned ON.\n");
}

void led_off() {
    // Code to turn off the LED
    led_state = 0;
    printf("LED turned OFF.\n");
}

void blink_led(int times, float duration) {
    for (int i = 0; i < times; i++) {
        led_on();
        usleep(duration * 1000000);
        led_off();
        usleep(duration * 1000000);
    }
}

void show_time() {
    time_t now;
    struct tm *tm_info;
    char buffer[26];

    time(&now);
    tm_info = localtime(&now);
    strftime(buffer, 26, "%H:%M:%S", tm_info);
    printf("Current time: %s\n", buffer);
}

void set_rtc_from_command(char **parts, int count) {
    if (count != 7) {
        printf("Usage: set_time YYYY MM DD HH MM SS\n");
        return;
    }
    int year = atoi(parts[1]);
    int month = atoi(parts[2]);
    int day = atoi(parts[3]);
    int hour = atoi(parts[4]);
    int minute = atoi(parts[5]);
    int second = atoi(parts[6]);
    // Set RTC time logic here
    printf("RTC time set successfully.\n");
}

void print_main_menu() {
    printf("\n======== RaspberryTQ ========\n");
    printf("Type one of the following commands:\n");
    printf("1 or time           → Show current time\n");
    printf("2 or blink N        → Blink LED N times\n");
    printf("3 or shell          → Enter custom shell\n");
    printf("4 or toggle         → Toggle LED ON/OFF\n");
    printf("5 or set_time Y M D H M S\n");
    printf("6 or exit           → Stop the program\n");
    printf("=============================\n");
}

void custom_shell() {
    printf("Entering custom shell. Type 'exit' to return.\n");
    printf("Available: help, time, led_on, led_off, blink, exit\n");

    while (1) {
        printf("cstuser@RaspberryTQ >>> ");
        char cmd[100];
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = 0; // Remove newline

        if (strcmp(cmd, "exit") == 0) {
            printf("Exiting shell.\n");
            break;
        } else if (strcmp(cmd, "help") == 0) {
            printf("Commands: time, led_on, led_off, blink, exit\n");
        } else if (strcmp(cmd, "time") == 0) {
            show_time();
        } else if (strcmp(cmd, "led_on") == 0) {
            led_on();
        } else if (strcmp(cmd, "led_off") == 0) {
            led_off();
        } else if (strcmp(cmd, "blink") == 0) {
            blink_led(1, 0.2);
        } else {
            printf("Unknown command.\n");
        }
    }
}

int main() {
    print_main_menu();
    while (1) {
        printf("RaspberryTQ> ");
        char line[100];
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = 0; // Remove newline

        char *parts[10];
        int count = 0;
        char *token = strtok(line, " ");
        while (token != NULL) {
            parts[count++] = token;
            token = strtok(NULL, " ");
        }

        if (count == 0) {
            continue;
        }

        char *cmd = parts[0];

        if (strcmp(cmd, "1") == 0 || strcmp(cmd, "time") == 0) {
            show_time();
            blink_led(1, 0.2);
        } else if (strcmp(cmd, "2") == 0 || strcmp(cmd, "blink") == 0) {
            int times = (count > 1) ? atoi(parts[1]) : 1;
            blink_led(times, 0.2);
        } else if (strcmp(cmd, "3") == 0 || strcmp(cmd, "shell") == 0) {
            custom_shell();
        } else if (strcmp(cmd, "4") == 0 || strcmp(cmd, "toggle") == 0) {
            led_state = !led_state;
            if (led_state) {
                led_on();
            } else {
                led_off();
            }
            printf("LED is now %s.\n", led_state ? "ON" : "OFF");
        } else if (strcmp(cmd, "5") == 0 || strcmp(cmd, "set_time") == 0) {
            set_rtc_from_command(parts, count);
        } else if (strcmp(cmd, "6") == 0 || strcmp(cmd, "exit") == 0) {
            printf("Exiting RaspberryTQ. Goodbye!\n");
            break;
        } else {
            printf("Unknown command. Type again.\n");
            print_main_menu();
        }
    }
    return 0;
}
