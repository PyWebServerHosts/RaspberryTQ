import machine.Pin
import machine.RTC
import kotlin.system.exitProcess
import java.util.Scanner

val led = Pin(25, Pin.OUT)
val rtc = RTC()
rtc.datetime = arrayOf(2025, 5, 24, 6, 12, 0, 0, 0) // Default time

var ledState = false

fun blinkLed(times: Int = 1, duration: Double = 0.2) {
    repeat(times) {
        led.on()
        Thread.sleep((duration * 1000).toLong())
        led.off()
        Thread.sleep((duration * 1000).toLong())
    }
}

fun showTime() {
    val now = rtc.datetime
    println("Current time: ${String.format("%02d", now[4])}:${String.format("%02d", now[5])}:${String.format("%02d", now[6])}")
}

fun setRtcFromCommand(parts: List<String>) {
    try {
        if (parts.size != 7) {
            println("Usage: set_time YYYY MM DD HH MM SS")
            return
        }
        val year = parts[1].toInt()
        val month = parts[2].toInt()
        val day = parts[3].toInt()
        val hour = parts[4].toInt()
        val minute = parts[5].toInt()
        val second = parts[6].toInt()
        rtc.datetime = arrayOf(year, month, day, 0, hour, minute, second, 0)
        println("RTC time set successfully.")
    } catch (e: Exception) {
        println("Error setting time: ${e.message}")
    }
}

fun printMainMenu() {
    println("\n======== RaspberryTQ ========")
    println("Type one of the following commands:")
    println("1 or time           → Show current time")
    println("2 or blink N        → Blink LED N times")
    println("3 or shell          → Enter custom shell")
    println("4 or toggle         → Toggle LED ON/OFF")
    println("5 or set_time Y M D H M S")
    println("6 or exit           → Stop the program")
    println("=============================\n")
}

fun customShell() {
    println("Entering custom shell. Type 'exit' to return.")
    println("Available: help, time, led_on, led_off, blink, exit")

    val scanner = Scanner(System.`in`)
    while (true) {
        print("cstuser@RaspberryTQ >>> ")
        val cmd = scanner.nextLine().trim().lowercase()
        when (cmd) {
            "exit" -> {
                println("Exiting shell.")
                break
            }
            "help" -> println("Commands: time, led_on, led_off, blink, exit")
            "time" -> showTime()
            "led_on" -> {
                led.on()
                println("LED turned ON.")
            }
            "led_off" -> {
                led.off()
                println("LED turned OFF.")
            }
            "blink" -> blinkLed()
            else -> println("Unknown command.")
        }
    }
}

fun main() {
    printMainMenu()
    val scanner = Scanner(System.`in`)
    while (true) {
        print("RaspberryTQ> ")
        val line = scanner.nextLine().trim().lowercase()
        val parts = line.split(" ")

        if (parts.isEmpty()) continue

        val cmd = parts[0]

        when (cmd) {
            "1", "time" -> {
                showTime()
                blinkLed(1)
            }
            "2", "blink" -> {
                try {
                    val times = if (parts.size > 1) parts[1].toInt() else 1
                    blinkLed(times)
                } catch (e: Exception) {
                    println("Usage: blink [times]")
                }
            }
            "3", "shell" -> customShell()
            "4", "toggle" -> {
                ledState = !ledState
                led.value(ledState)
                println("LED is now ${if (ledState) "ON" else "OFF"}.")
            }
            "5", "set_time" -> setRtcFromCommand(parts)
            "6", "exit" -> {
                println("Exiting RaspberryTQ. Goodbye!")
                exitProcess(0)
            }
            else -> {
                println("Unknown command. Type again.")
                printMainMenu()
            }
        }
    }
}

main()

