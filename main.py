import sys
import re
import threading
import time


MAX_LED_OPTIONS = 4
leds = {
    1: '/dev/led163',
    2: '/dev/led164',
    3: '/dev/led165',
    4: '/dev/led166'
}


def show_menu():
    print()
    print('1. Turn on led')
    print('2. Turn off led')
    print('3. Check led status')
    print('4. Marquee')
    print('> ', end='')

def show_leds():
    print()
    print('1. Port GPIO_PU3 (gpio163), Pin 49')
    print('2. Port GPIO_PU4 (gpio164), Pin 52')
    print('3. Port GPIO_PU5 (gpio165), Pin 55')
    print('4. Port GPIO_PU6 (gpio166), Pin 58')
    print('> ', end='')

def exit():
    print()
    print('Bye Bye ~~')
    sys.exit(0)

def turn_led(on_off=True):
    print()
    print('Which led do you want to turn', 'on' if on_off else 'off')

    show_leds()
    try:
        led = int(input())
        if led > MAX_LED_OPTIONS:
            raise Exception()
    except KeyboardInterrupt:
        exit()
    except:
        print('Invalid command.')
        return

    do_turn_led(led, on_off)
    print()
    print('Turn', 'on' if on_off else 'off', 'led number', led)

def do_turn_led(number, state=True):
    if number in leds:
        if state:
            with open(leds[number], 'w') as f:
                f.write('1')
        else:
            with open(leds[number], 'w') as f:
                f.write('0')

def check_led_state(number):
    print()
    print('Which led do you want to check')

    show_leds()
    try:
        led = int(input())
        if led > MAX_LED_OPTIONS:
            raise Exception()
    except KeyboardInterrupt:
        exit()
    except:
        print('Invalid command.')
        return
    
    with open(leds[number], 'r') as f:
        state = f.read()
    if state == '1':
        print('Led is on')
    else:
        print('Led is off')


def marquee():
    status_list = ['0000', '0001', '0010', '0100', '1000']
    counter = 0
    interval = 1

    while True:
        print()
        print('-- MARQUEE --')
        print()
        print('  Status list:')
        for index, status in enumerate(status_list):
            print('    {}.'.format(index+1), status)
        print('Type "start" to start marquee')
        print('Type "add XXXX" to add new status')
        print('> ', end='')

        command = input()
        if command == 'start':
            print('Press Ctrl+C to stop marquee')
            print('Running marquee')
            try:
                while True:
                    for index, status in enumerate(status_list[counter]):
                        do_turn_led(index+1, status == '1')
                    counter = (counter + 1) % len(status_list)
                    time.sleep(interval)
            except KeyboardInterrupt:
                print('Stop marquee...')
        elif re.match(r'^add [0-1]{4}$', command):
            status_list.append(command.split(' ')[1])
        else:
            print('Invalid command.')

def main():
    MAX_MENU_OPTIONS = 3

    while True:
        show_menu()
        try:
            option = int(input())
            if option > MAX_MENU_OPTIONS:
                raise Exception()
        except KeyboardInterrupt:
            exit()
        except:
            print('Invalid command.')
            continue
        
        if 1 <= option <= 2:
            turn_led(True if option == 1 else False)
        elif 3:

        elif 4:
            marquee()


if __name__ == '__main__':
    main()