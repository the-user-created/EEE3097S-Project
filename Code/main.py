from lzma import LZMADecompressor, LZMACompressor, LZMAError
from twofish import Twofish
import filecmp
import sys
import time
from os import listdir, stat
from os.path import isfile, join


def compress(input_file, output_file):
    input_file_size = stat(input_file).st_size
    print(f"Uncompressed file size = {input_file_size} Bytes")
    lzc = LZMACompressor()

    start_time = time.time()
    with open(input_file, mode="r") as infile, open(output_file, "wb") as outfile:
        for chunk in infile.read():
            compressed_chunk = lzc.compress(chunk.encode("ascii"))
            outfile.write(compressed_chunk)
        outfile.write(lzc.flush())
    end_time = time.time()

    output_file_size = stat(output_file).st_size
    print(f"Compressed file size = {output_file_size} Bytes")
    print(f"Compression took {round(end_time - start_time, 10)} seconds")


def encrypt(input_file, output_file):
    password = input("Enter encryption password: ")
    input_file_size = stat(input_file).st_size
    print(f"Unencrypted file size = {input_file_size} Bytes")
    bs = 16  # block size 16 bytes or 128 bits

    start_time = time.time()
    with open(file=input_file, mode="rb") as infile, open(file=output_file, mode="wb") as outfile:
        plaintext = infile.read()
        if len(plaintext) % bs:  # add padding
            padded_plaintext = plaintext + ('%' * (bs - len(plaintext) % bs)).encode('ascii')
        else:
            padded_plaintext = plaintext

        T = Twofish(str.encode(password))
        ciphertext = b''

        for x in range(int(len(padded_plaintext) / bs)):
            ciphertext += T.encrypt(padded_plaintext[x * bs:(x + 1) * bs])

        outfile.write(ciphertext)
    end_time = time.time()

    output_file_size = stat(output_file).st_size
    print(f"Encrypted file size = {output_file_size} Bytes")
    print(f"Encryption took {round(end_time - start_time, 10)} seconds")


def decrypt(input_file, output_file):
    password = input("Enter encryption password: ")
    input_file_size = stat(input_file).st_size
    print(f"Encrypted file size = {input_file_size} Bytes")
    bs = 16  # block size 16 bytes or 128 bits

    start_time = time.time()
    with open(file=input_file, mode="rb") as infile, open(file=output_file, mode="wb") as outfile:
        ciphertext = infile.read()
        T = Twofish(str.encode(password))  # Convert password to binary
        plaintext = b''

        for x in range(int(len(ciphertext) / bs)):
            plaintext += T.decrypt(ciphertext[x * bs:(x + 1) * bs])

        outfile.write(plaintext.strip('%'.encode("ascii")))  # remove padding
    end_time = time.time()

    output_file_size = stat(output_file).st_size
    print(f"Unencrypted file size = {output_file_size} Bytes")
    print(f"Decryption took {round(end_time - start_time, 10)} seconds")


def decompress(input_file, output_file):
    lzd = LZMADecompressor()

    invalid_password = False

    start_time = time.time()
    with open(input_file, mode="rb") as infile, open(output_file, "w") as outfile:
        for chunk in infile.read():
            try:
                decompressed_chunk = lzd.decompress(chunk.to_bytes(1, byteorder="little")).decode("ascii")
            except LZMAError:
                print("Incorrect decryption password provided")
                invalid_password = True
                break

            if not invalid_password:
                outfile.write(decompressed_chunk)

    if not invalid_password:
        end_time = time.time()

        input_file_size = stat(input_file).st_size
        print(f"Uncompressed file size = {input_file_size} Bytes")
        output_file_size = stat(output_file).st_size
        print(f"Compressed file size = {output_file_size} Bytes")
        print(f"Decompression took {round(end_time - start_time, 10)} seconds")


def compare_files(file1, file2):
    res = filecmp.cmp(file1, file2, shallow=False)
    print(f"The original file {'matches' if res else 'does not match'} the output file!")


def main():
    option = 2
    # For overall functionality test:
    if option == 0:
        input_file = "EEE3097S_2022_Turntable_Example_Data.csv"
        compress(input_file="sample_data/" + input_file, output_file="sample_data/results/" + input_file + ".lz")
        print()
        encrypt(input_file="sample_data/results/" + input_file + ".lz", output_file="sample_data/results/" + input_file + ".lz.tf")
        print()
        decrypt(input_file="sample_data/results/" + input_file + ".lz.tf", output_file="sample_data/results/" + input_file + ".lz")
        print()
        decompress(input_file="sample_data/results/" + input_file + ".lz", output_file="sample_data/results/" + input_file)
        print()
        compare_files("sample_data/" + input_file, "sample_data/results/" + input_file)
    elif option == 1:  # For compression block tests
        files = [f for f in listdir("sample_data/") if isfile(join("sample_data/", f)) and f[0] != "." and not f.endswith(".lz") and f != "t.csv"]
        for input_file in files:
            print(f"Compressing {input_file}:")
            compress(input_file="sample_data/" + input_file, output_file="sample_data/results/" + input_file + ".lz")
            print(f"\nDecompressing {input_file + '.lz'}:")
            decompress(input_file="sample_data/results/" + input_file + ".lz", output_file="sample_data/results/" + input_file)
            print()
            compare_files("sample_data/" + input_file, "sample_data/results/" + input_file)
            print()
    else:  # For encryption block tests
        files = [f for f in listdir("sample_data/") if isfile(join("sample_data/", f)) and f.endswith(".lz")]
        for input_file in files:
            print(f"Encrypting {input_file}:")
            encrypt(input_file="sample_data/" + input_file, output_file="sample_data/results/" + input_file + ".tf")
            print(f"\nDecrypting {input_file + '.tf'}:")
            decrypt(input_file="sample_data/results/" + input_file + ".tf", output_file="sample_data/results/" + input_file)
            print()
            compare_files("sample_data/" + input_file, "sample_data/results/" + input_file)
            print()
            break


if __name__ == "__main__":
    main()
