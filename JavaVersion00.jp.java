
import com.pi4j.io.gpio.GpioController;
import com.pi4j.io.gpio.GpioFactory;
import com.pi4j.io.gpio.GpioPinDigitalOutput;
import com.pi4j.io.gpio.PinState;
import com.pi4j.io.gpio.RaspiPin;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Scanner;

public class RaspberryTQ {
private static GpioPinDigitalOutput led;
private static LocalDateTime rtc;
private static boolean ledState = false;

public static void main(String[] args) {
GpioController gpio = GpioFactory.getInstance();
led = gpio.provisionDigitalOutputPin(RaspiPin.GPIO_25, "LED", PinState.LOW);
rtc = LocalDateTime.of(2025, 5, 24, 6, 12); // Default time

printMainMenu();
Scanner scanner = new Scanner(System.in);
while (true) {
System.out.print("RaspberryTQ> ");
String line = scanner.nextLine().trim().toLowerCase();
String[] parts = line.split("\\s+");

if (parts.length == 0) {
continue;
}

String cmd = parts[0];

if (cmd.equals("1") || cmd.equals("time")) {
showTime();
blinkLed(1);
} else if (cmd.equals("2") || cmd.equals("blink")) {
try {
int times = parts.length > 1 ? Integer.parseInt(parts[1]) : 1;
blinkLed(times);
} catch (NumberFormatException e) {
System.out.println("Usage: blink [times]");
}
} else if (cmd.equals("3") || cmd.equals("shell")) {
customShell(scanner);
} else if (cmd.equals("4") || cmd.equals("toggle")) {
ledState = !ledState;
led.setState(ledState ? PinState.HIGH : PinState.LOW);
System.out.println("LED is now " + (ledState ? "ON" : "OFF") + ".");
} else if (cmd.equals("5") || cmd.equals("set_time")) {
setRtcFromCommand(parts);
} else if (cmd.equals("6") || cmd.equals("exit")) {
System.out.println("Exiting RaspberryTQ. Goodbye!");
break;
} else {
System.out.println("Unknown command. Type again.");
printMainMenu();
}
}
scanner.close();
gpio.shutdown();
}

private static void blinkLed(int times) {
blinkLed(times, 0.2);
}

private static void blinkLed(int times, double duration) {
for (int i = 0; i < times; i++) {
led.high();
sleep((long) (duration * 1000));
led.low();
sleep((long) (duration * 1000));
}
}

private static void showTime() {
DateTimeFormatter formatter = DateTimeFormatter.ofPattern("HH:mm:ss");
System.out.println("Current time: " + rtc.format(formatter));
}

private static void setRtcFromCommand(String[] parts) {
try {
if (parts.length != 7) {
System.out.println("Usage: set_time YYYY MM DD HH MM SS");
return;
}
int year = Integer.parseInt(parts[1]);
int month = Integer.parseInt(parts[2]);
int day = Integer.parseInt(parts[3]);
int hour = Integer.parseInt(parts[4]);
int minute = Integer.parseInt(parts[5]);
int second = Integer.parseInt(parts[6]);
rtc = LocalDateTime.of(year, month, day, hour, minute, second);
System.out.println("RTC time set successfully.");
} catch (Exception e) {
System.out.println("Error setting time: " + e.getMessage());
}
}

private static void printMainMenu() {
System.out.println("\n======== RaspberryTQ ========");
System.out.println("Type one of the following commands:");
System.out.println("1 or time → Show current time");
System.out.println("2 or blink N → Blink LED N times");
System.out.println("3 or shell → Enter custom shell");
System.out.println("4 or toggle → Toggle LED ON/OFF");
System.out.println("5 or set_time Y M D H M S");
System.out.println("6 or exit → Stop the program");
System.out.println("=============================\n");
}

private static void customShell(Scanner scanner) {
System.out.println("Entering custom shell. Type 'exit' to return.");
System.out.println("Available: help, time, led_on, led_off, blink, exit");

while (true) {
System.out.print("cstuser@RaspberryTQ >>> ");
String cmd = scanner.nextLine().trim().toLowerCase();
if (cmd.equals("exit")) {
System.out.println("Exiting shell.");
break;
} else if (cmd.equals("help")) {
System.out.println("Commands: time, led_on, led_off, blink, exit");
} else if (cmd.equals("time")) {
showTime();
} else if (cmd.equals("led_on")) {
led.high();
System.out.println("LED turned ON.");
} else if (cmd.equals("led_off")) {
led.low();
System.out.println("LED turned OFF.");
} else if (cmd.equals("blink")) {
blinkLed(1);
} else {
System.out.println("Unknown command.");
}
}
}

private static void sleep(long millis) {
try {
Thread.sleep(millis);
} catch (InterruptedException e) {
Thread.currentThread().interrupt();
}
}
}
