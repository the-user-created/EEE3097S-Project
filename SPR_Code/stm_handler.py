from time import sleep
import serial
import time


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
            # sleep(0.1)
            print()

            if data_collected.lower() == "4461746120436f6c6c6563746564":
                ### FOR CHECKSUM BLOCK TESTS ###
                # print("SENSOR DATA:")
                # sensor_data = read_data(ser)
                # print(f"Sensor data size: {len(sensor_data) // 2} bytes")
                # dumphex(sensor_data)
                sleep(0.1)
                # print()
                break

        # GET ENCRYPTED DATA FROM STM
        write_data(ser, b"RDY")
        # sleep(0.1)

        print("CRC32 CHECKSUM:")
        start_time = time.time()
        crc = read_data(ser)
        crc_time = time.time() - start_time
        dumphex(crc)
        print()

        ### FOR COMPRESSION BLOCK AND OVERALL FUNCTIONALITY TESTS ###
        print("COMPRESSED DATA:")
        sleep(0.1)
        start_time = time.time()
        compressed_data = read_data(ser)
        print(f"Compression took: {round((time.time() - start_time) * 1000, 3)} milliseconds")
        print(f"Compressed data size: {len(compressed_data) // 2} bytes")
        # dumphex(compressed_data)
        print()

        ### FOR ENCRYPTION BLOCK AND OVERALL FUNCTIONALITY TESTS ###
        print("ENCRYPTED DATA:")
        sleep(0.1)
        start_time = time.time()
        encrypted_data = read_data(ser)
        print(f"Encryption took: {round((time.time() - start_time) * 1000, 3)} milliseconds")
        print(f"Encrypted data size: {len(encrypted_data) // 2} bytes")
        # dumphex(encrypted_data)
        print()

        ### FOR COMPRESSION BLOCK AND OVERALL FUNCTIONALITY TESTS ###
        # GET COMPRESSED SIZE FROM STM
        print("COMPRESSED DATA SIZE")
        compressed_size = read_data(ser)
        dumphex(compressed_size)
        print()

        ### FOR ENCRYPTION BLOCK AND OVERALL FUNCTIONALITY TESTS ###
        # GET PADDING LENGTH FROM STM
        print("PADDING LENGTH:")
        padding_len = read_data(ser)
        dumphex(padding_len)
        print()

        while True:
            user_input = input("Decrypt and decompress? (y/n): ")
            if user_input.lower() == "y":
                # SEND DATA TO "NEXT" STM FOR DECRYPTION AND DECOMPRESSION
                write_data(ser, b"GMD")
                sleep(0.1)

                ### FOR ENCRYPTION BLOCK AND OVERALL FUNCTIONALITY TESTS ###
                print("PADDING LENGTH CONFIRMATION:")
                write_data(ser, bytes.fromhex(padding_len))
                padding_len = read_data(ser)
                dumphex(padding_len)
                sleep(0.1)
                print()

                ### FOR COMPRESSION BLOCK AND OVERALL FUNCTIONALITY TESTS ###
                print("COMPRESSED DATA SIZE CONFIRMATION:")
                write_data(ser, bytearray.fromhex(compressed_size))
                compressed_size = read_data(ser)
                dumphex(compressed_size)
                sleep(0.1)
                print()

                ### FOR COMPRESSION BLOCK TESTS ###
                # print("COMPRESSED DATA CONFIRMATION:")
                # write_data(ser, bytearray.fromhex(compressed_data))
                # compressed_data = read_data(ser)
                # dumphex(compressed_data)
                # print()

                # sleep(0.1)

                ### FOR ENCRYPTION BLOCK AND OVERALL FUNCTIONALITY TESTS ###
                print("ENCRYPTED DATA CONFIRMATION:")
                write_data(ser, bytearray.fromhex(encrypted_data))
                encrypted_data = read_data(ser)
                # dumphex(encrypted_data)
                print()

                ### FOR ENCRYPTION BLOCK AND OVERALL FUNCTIONALITY TESTS ###
                print("DECRYPTED DATA:")
                sleep(0.1)
                start_time = time.time()
                decrypted_data = read_data(ser)
                print(f"Decryption took: {round((time.time() - start_time) * 1000, 3)} milliseconds")
                print(f"Decrypted data size: {len(decrypted_data) // 2} bytes")
                # dumphex(decrypted_data)
                print()

                ### FOR COMPRESSION BLOCK AND OVERALL FUNCTIONALITY TESTS ###
                print("DECOMPRESSED DATA:")
                sleep(0.1)
                start_time = time.time()
                decompressed_data = read_data(ser)
                print(f"Decompression took: {round((time.time() - start_time) * 1000, 3)} milliseconds")
                print(f"Decompressed data size: {len(decompressed_data) // 2} bytes")
                # dumphex(decompressed_data)
                print()

                ### FOR CHECKSUM BLOCK TESTS ###
                # print("SENSOR DATA CONFIRMATION:")
                # write_data(ser, bytearray.fromhex(sensor_data))
                # sensor_data = read_data(ser)
                # dumphex(sensor_data)
                # sleep(0.1)
                # print()

                print("CRC32 CHECKSUM:")
                start_time = time.time()
                crc = read_data(ser)
                crc_time += time.time() - start_time
                print(f"Average checksum generation time: {round(crc_time/2, 3)} milliseconds")
                dumphex(crc)
                print()
                break
            else:
                # DO NOT DECRYPT AND DECOMPRESS THIS DATA
                write_data(ser, b"SKP")
                break


if __name__ == '__main__':
    main()
