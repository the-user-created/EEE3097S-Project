import binascii
from time import sleep
import serial


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


def write_data(ser, data):
    ser.write(data)


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

    while True:
        # WAIT FOR DATA COLLECTED ACK FROM STM
        while True:
            data_collected = read_data(ser)

            # Check if the transmitted sequence is hex for 'Data Collected'
            if data_collected.lower() == "4461746120436f6c6c6563746564":
                sleep(0.1)
                break

        # Tell 1st STM that the user is ready for transmission of data
        write_data(ser, b"RDY")

        # Receive the checksum of the sensor data from 1st STM
        print("CRC32 CHECKSUM:")
        crc = read_data(ser)
        dumphex(crc)
        sleep(0.1)

        # Receive the encrypted data from 1st STM
        encrypted_data = read_data(ser)
        print("\nRECEIVED ENCRYPTED DATA")

        # Receive the compression data size from 1st STM
        compressed_size = read_data(ser)
        print("RECEIVED COMPRESSED DATA SIZE:")
        dumphex(compressed_size)

        # Receive the padding length used from 1st STM
        padding_len = read_data(ser)
        print("\nRECEIVED PADDING LENGTH:")
        dumphex(padding_len)

        while True:
            user_input = input("\nDecrypt and decompress? (y/n): ")
            if user_input.lower() == "y":
                # Transmit encrypted data to 2nd STM for decryption and decompression
                write_data(ser, b"GMD")
                sleep(0.1)

                # Transmit padding length used to 2nd STM
                print("TRANSMITTING PADDING LENGTH")
                write_data(ser, bytes.fromhex(padding_len))
                sleep(0.1)

                # Transmit compression data size to 2nd STM
                print("TRANSMITTING COMPRESSED DATA SIZE")
                write_data(ser, bytearray.fromhex(compressed_size))
                sleep(0.1)

                # Transmit encrypted data to 2nd STM
                print("TRANSMITTING ENCRYPTED DATA")
                write_data(ser, bytearray.fromhex(encrypted_data))
                sleep(0.1)

                # Receive decompressed (sensor) data from 2nd STM
                decompressed_data = read_data(ser)
                print("RECEIVED DECOMPRESSED DATA:")
                dumphex(decompressed_data)

                # Receive checksum of decompressed data from 2nd STM
                print("\nCRC32 CHECKSUM:")
                crc = read_data(ser)
                dumphex(crc)
                print()
                break
            else:
                # DO NOT DECRYPT AND DECOMPRESS THIS DATA
                write_data(ser, b"SKP")
                break


if __name__ == '__main__':
    main()
