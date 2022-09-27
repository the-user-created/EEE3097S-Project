from time import sleep

import serial


def dumphex(string):
    i = 0
    line = 0
    for j in range(0, len(string), 2):
        if i == 0:
            print(f"{line}\t", end="")
        hex_v = string[j:j+2]
        print(hex_v, end="")
        i += 1

        if i == 8:
            print("  ", end="")
        elif i == 16:
            i = 0
            line += 1
            print("\n", end="")
        else:
            print(" ", end="")


def read_data(ser):
    myBytes = ser.read(ser.inWaiting()).hex()
    while True:
        bytesToRead = ser.inWaiting()
        myBytes = myBytes + ser.read(bytesToRead).hex()
        if "0d454545454545450d0a" in myBytes:
            break

    myBytes = myBytes.replace("0d454545454545450d0a", "")

    dumphex(myBytes)


def write_data(ser):
    ser.write(b"e")


def main():
    ser = serial.Serial(
        port='/dev/tty.usbserial-0001',
        baudrate=9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=0.5,
        inter_byte_timeout=0.1
    )
    write_data(ser)
    sleep(0.1)
    myBytes = ser.read(ser.inWaiting())
    print(myBytes)
    #dumphex(myBytes)


if __name__ == '__main__':
    main()
