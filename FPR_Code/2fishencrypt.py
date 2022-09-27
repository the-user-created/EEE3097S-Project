import filecmp

from twofish import Twofish


def tfencrypt(infile, outfile, password):
    bs = 16  # block size 16 bytes or 128 bits
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


def tfdecrypt(infile, outfile, password):
    bs = 16  # block size 16 bytes or 128 bits
    ciphertext = infile.read()
    T = Twofish(str.encode(password))
    plaintext = b''

    for x in range(int(len(ciphertext) / bs)):
        plaintext += T.decrypt(ciphertext[x * bs:(x + 1) * bs])

    outfile.write(plaintext.strip('%'.encode("ascii")))  # remove padding


password = 'dC99C9#K2L4f3Bok@dcyRfpBLV#kSFxX'

with open('IO/in', 'rb') as infile, open('outfile_e.csv.xz.tf', 'wb') as outfile:
    tfencrypt(infile, outfile, password)

print("Encryption done")

with open('outfile_e.csv.xz.tf', 'rb') as infile, open('outfile_decrypted', 'wb') as outfile:
    tfdecrypt(infile, outfile, password)

print("Decryption done")
res = filecmp.cmp("IO/in", "outfile_decrypted", shallow=False)
print("If the file is identical, this will return True: ", end="")
print(res)
