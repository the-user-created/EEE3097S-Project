import serial
from time import time
from datetime import timedelta
import ctypes


def dumphex(string):
    i = 0
    line = 0
    for j in range(0, len(string), 2):
        hex_v = string[j:j + 2]
        if i == 0:
            print(f"{line}\t", end="")
        print(hex_v, end="")
        i += 1

        if i == 8:
            print("  ", end="")
        elif i == 16:
            i = 0
            line += 1
            print()
        else:
            print(" ", end="")


def read_data(ser):
    data = ser.read(ser.inWaiting()).hex()
    while True:
        bytes_to_read = ser.inWaiting()
        data = data + ser.read(bytes_to_read).hex()
        if "0d454545454545450d0a" in data:
            break

    data = data.replace("0d454545454545450d0a", "")

    return data


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

    choice = "magn"
    start_time = time()
    while True:
        data = ser.read(60).replace(b"\x00", b"").replace(b"\n", b"")
        read_time = time() - start_time

        if data:
            data = data.split(b",")
            int_data = [ctypes.c_int16(int(data[i], 16)).value for i in range(0, len(data))]
            print(f"{timedelta(seconds=read_time)} ", end="")

            if choice == "accel":
                print(f"{int_data[0]},{int_data[1]},{int_data[2]}")
            elif choice == "gyro":
                print(f"{int_data[3]},{int_data[4]},{int_data[5]}")
            else:
                print(f"{int_data[6]},{int_data[7]},{int_data[8]}")


if __name__ == '__main__':
    main()
