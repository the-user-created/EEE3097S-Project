import matplotlib.pyplot as plt
import numpy as np

"""
66, 132, 198, 264, 330, 396, 462, 528, 594, 660, 726
72, 136, 200, 264, 336, 400, 464, 528, 600, 664, 728
85.866, 155.080, 225.800, 292.847, 370.048, 442.593, 511.385, 581.825, 657.277, 727.974, 798.617
77.821, 151.242, 220.632, 291.795, 365.553, 435.963, 505.961, 580.378, 652.113, 721.273, 792.994
"""


def compression():
    data_size = [66, 176, 286, 396, 506, 616, 726, 836, 946, 1012]
    out_size = [52, 131, 169, 225, 269, 351, 374, 439, 422, 430]
    compression_time = [105.228, 227.086, 292.021, 367.486, 425.389, 557.499, 585.581, 686.253, 655.423, 654.141]
    decompression_time = [82.075, 205.773, 327.105, 446.712, 567.747, 693.522, 814.379, 935.235, 1052.512, 1122.388]

    compression_speed = []
    decompression_speed = []
    compression_ratio = []

    for i in range(0, len(data_size)):
        # compression_speed.append(round(1000 * data_size[i] / compression_time[i], 3))
        # decompression_speed.append(round(1000 * data_size[i] / decompression_time[i], 3))
        # compression_ratio.append(round(data_size[i] / out_size[i], 3))

        compression_speed.append(1000 * data_size[i] / compression_time[i])
        decompression_speed.append(1000 * data_size[i] / decompression_time[i])
        compression_ratio.append(data_size[i] / out_size[i])

    print(compression_ratio)
    print(compression_speed)
    print(decompression_speed)

    print(np.mean(compression_speed))
    print(np.mean(decompression_speed))
    print(np.mean(compression_ratio))

    plt.plot(data_size, compression_speed, color="blue")
    plt.scatter(data_size, compression_speed, color="blue", marker="o", label="Compression")

    plt.plot(data_size, decompression_speed, color="red")
    plt.scatter(data_size, decompression_speed, color="red", marker="x", label="Decompression")

    plt.title("Data size vs. speed", fontsize=18)
    plt.xlabel("Data size [bytes]", fontsize=16)
    plt.ylabel("Speed [bytes/second]", fontsize=16)

    plt.grid(True, which="both")
    plt.minorticks_on()
    plt.tight_layout()
    plt.legend()
    # plt.savefig(f"compression_data_size_vs_speed.png")
    # plt.close()
    plt.show()


def encryption():
    decrypted_size = [66, 132, 198, 264, 330, 396, 462, 528, 594, 660, 726]
    encrypted_size = [72, 136, 200, 264, 336, 400, 464, 528, 600, 664, 728]
    decryption_time = [85.866, 155.080, 225.800, 292.847, 370.048, 442.593, 511.385, 581.825, 657.277, 727.974, 798.617]
    encryption_time = [77.821, 151.242, 220.632, 291.795, 365.553, 435.963, 505.961, 580.378, 652.113, 721.273, 792.994]

    decryption_speed = []
    encryption_speed = []
    padding_length = []

    for i in range(0, len(decrypted_size)):
        # encryption_speed.append(round(1000 * encrypted_size[i] / encryption_time[i], 3))
        # decryption_speed.append(round(1000 * encrypted_size[i] / decryption_time[i], 3))

        encryption_speed.append(1000 * encrypted_size[i] / encryption_time[i])
        decryption_speed.append(1000 * encrypted_size[i] / decryption_time[i])

        padding_length.append(encrypted_size[i] - decrypted_size[i])

    print(encryption_speed)
    print(decryption_speed)
    print(padding_length)

    print(np.mean(encryption_speed))
    print(np.mean(decryption_speed))
    print(np.mean(padding_length))

    plt.plot(encrypted_size, decryption_speed, color="blue")
    plt.scatter(encrypted_size, decryption_speed, color="blue", marker="o", label="Decryption")

    plt.plot(encrypted_size, encryption_speed, color="red")
    plt.scatter(encrypted_size, encryption_speed, color="red", marker="x", label="Encryption")

    plt.title("Data size vs. speed", fontsize=18)
    plt.xlabel("Data size [bytes]", fontsize=16)
    plt.ylabel("Speed [bytes/second]", fontsize=16)

    plt.grid(True, which="both")
    plt.minorticks_on()
    plt.tight_layout()
    plt.legend()
    plt.savefig(f"encryption_data_size_vs_speed.png")
    plt.close()
    # plt.show()


def checksum():
    data_size = [66, 550, 1034, 1518, 2002, 2486, 2970, 3454, 3938, 4422]
    gen_time = [0.015, 0.017, 0.019, 0.022, 0.024, 0.025, 0.029, 0.030, 0.033, 0.035]

    gen_speed = []

    for i in range(0, len(data_size)):
        gen_speed.append(round(1000 * data_size[i] / gen_time[i], 0))

    print(gen_speed)
    print(np.mean(gen_speed))


if __name__ == '__main__':
    # compression()
    # encryption()
    checksum()
