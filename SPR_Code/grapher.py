import matplotlib.pyplot as plt
import numpy as np

"""
"""


def compression():
    # FOR OVERALL FUNCTIONALITY TESTS
    data_size = [66, 66, 66, 132, 132, 132, 198, 198, 198, 264, 264, 264, 330, 330, 330, 396, 396, 396, 462, 462, 462, 528, 528, 528]
    out_size = [51, 53, 53, 100, 97, 96, 139, 143, 139, 189, 176, 168, 200, 207, 212, 226, 227, 219, 257, 252, 262, 277, 269, 266]
    compression_time = [99.247, 103.082, 100.752, 181.284, 170.482, 164.210, 230.942, 241.268, 233.201, 312.225, 294.273, 277.431, 325.992, 331.882, 349.422, 365.462, 357.379, 352.770, 398.513, 397.882, 417.537, 436.984, 424.623, 414.983]
    decompression_time = [81.695, 82.050, 85.665, 159.545, 156.233, 154.892, 229.303, 233.041, 229.045, 305.079, 304.629, 302.546, 377.054, 375.696, 377.768, 448.838, 450.288, 448.132, 525.047, 522.209, 522.628, 596.212, 592.654, 592.631]

    # FOR COMPRESSION BLOCK TESTS
    data_size = [66, 66, 66, 176, 176, 176, 286, 286, 286, 396, 396, 396, 506, 506, 506, 616, 616, 616, 726, 726, 726, 836, 836, 836, 946, 946, 946, 1012, 1012, 1012]
    out_size = [52, 53, 55, 135, 143, 134, 185, 184, 168, 229, 220, 234, 259, 267, 265, 322, 304, 307, 357, 336, 361, 416, 402, 420, 432, 470, 455, 450, 454, 503]
    compression_time = [100.182, 100.772, 101.176, 231.522, 236.873, 228.199, 304.115, 308.515, 279.246, 365.788, 348.977, 373.962, 407.981, 425.551, 415.255, 483.414, 468.701, 472.839, 542.230, 516.747, 548.701, 636.566, 618.971, 629.237, 651.877, 705.835, 679.237, 675.776, 682.564, 763.890]
    decompression_time = [80.765, 82.757, 81.869, 208.054, 206.914, 210.755, 327.614, 329.998, 325.378, 447.617, 448.724, 451.439, 568.688, 572.181, 571.438, 690.419, 692.684, 691.980, 814.721, 810.182, 813.227, 932.822, 933.682, 933.967, 1052.466, 1055.149, 1054.552, 1122.342, 1123.553, 1128.833]
    compression_speed = []
    decompression_speed = []
    compression_ratio = []
    data_sizes = []

    for i in range(0, len(data_size), 3):
        # compression_speed.append(round(1000 * data_size[i] / compression_time[i], 3))
        # decompression_speed.append(round(1000 * data_size[i] / decompression_time[i], 3))
        # compression_ratio.append(round(data_size[i] / out_size[i], 3))

        # compression_speed.append(1000 * data_size[i] / compression_time[i])
        # decompression_speed.append(1000 * data_size[i] / decompression_time[i])
        # compression_ratio.append(data_size[i] / out_size[i])

        compression_avg = 0
        decompression_avg = 0
        compression_ratio_avg = 0
        data_sizes.append(data_size[i])

        for j in range(0, 3):
            compression_avg += 1000 * data_size[i + j] / compression_time[i + j]
            decompression_avg += 1000 * data_size[i + j] / decompression_time[i + j]
            compression_ratio_avg += data_size[i + j] / out_size[i + j]

        compression_avg = round(compression_avg / 3, 3)
        decompression_avg = round(decompression_avg / 3, 3)
        compression_ratio_avg = round(compression_ratio_avg / 3, 3)

        compression_speed.append(compression_avg)
        decompression_speed.append(decompression_avg)
        compression_ratio.append(compression_ratio_avg)

    print(compression_ratio)
    print(compression_speed)
    print(decompression_speed)

    # print(np.mean(compression_speed))
    # print(np.mean(decompression_speed))
    # print(np.mean(compression_ratio))

    """plt.plot(data_sizes, compression_speed, color="blue")
    plt.scatter(data_sizes, compression_speed, color="blue", marker="o", label="Compression")

    plt.plot(data_sizes, decompression_speed, color="red")
    plt.scatter(data_sizes, decompression_speed, color="red", marker="x", label="Decompression")

    plt.title("Speed vs. data size", fontsize=18)
    plt.xlabel("Data size [bytes]", fontsize=16)
    plt.ylabel("Speed [bytes/second]", fontsize=16)

    plt.grid(True, which="both")
    plt.minorticks_on()
    plt.tight_layout()
    plt.legend()
    plt.savefig(f"compression_data_size_vs_speed.png")
    plt.close()"""
    # plt.show()


def encryption():
    # FOR OVERALL FUNCTIONALITY TESTS
    encrypted_size = [56, 56, 56, 104, 104, 96, 144, 144, 144, 192, 176, 168, 200, 208, 216, 232, 232, 224, 264, 256, 264, 280, 272, 272]
    decrypted_size = [51, 53, 53, 100, 97, 96, 139, 143, 139, 189, 176, 168, 200, 207, 212, 226, 227, 219, 257, 252, 262, 277, 269, 266]
    encryption_time = [67.316, 67.357, 69.482, 120.919, 120.419, 114.727, 165.888, 163.468, 162.382, 215.970, 197.294, 191.863, 226.650, 231.870, 242.458, 258.948, 257.843, 253.115, 296.662, 287.696, 291.973, 310.615, 301.723, 302.412]
    decryption_time = [76.983, 81.268, 77.736, 149.486, 112.123, 111.723, 160.940, 165.672, 158.876, 215.997, 201.156, 194.503, 232.821, 238.365, 241.614, 259.032, 260.137, 250.894, 288.751, 281.429, 289.880, 307.168, 298.265, 295.810]

    # FOR ENCRYPTION BLOCK TESTS
    decrypted_size = [66, 66, 66, 132, 132, 132, 198, 198, 198, 264, 264, 264, 330, 330, 330, 396, 396, 396, 462, 462, 462, 528, 528, 528, 594, 594, 594, 660, 660, 660]
    encrypted_size = [72, 72, 72, 136, 136, 136, 200, 200, 200, 264, 264, 264, 336, 336, 336, 400, 400, 400, 464, 464, 464, 528, 528, 528, 600, 600, 600, 664, 664, 664]
    decryption_time = [85.154, 85.679, 85.784, 153.094, 153.089, 154.823, 226.808, 223.974, 223.393, 295.683, 292.465, 293.394, 371.359, 370.470, 373.231, 441.560, 442.391, 441.162, 509.692, 510.034, 511.395, 580.993, 579.247, 582.506, 661.277, 658.119, 660.684, 728.164, 728.972, 727.241]
    encryption_time = [80.893, 78.680, 81.104, 153.286, 150.934, 149.583, 219.817, 222.079, 220.796, 293.328, 294.386, 294.342, 363.825, 363.981, 367.695, 437.072, 435.259, 435.616, 509.616, 508.206, 507.878, 578.576, 578.238, 580.411, 651.804, 650.903, 653.643, 720.719, 722.978, 721.049]
    
    decryption_speed = []
    encryption_speed = []
    padding_length = []
    data_sizes = []

    for i in range(0, len(decrypted_size), 3):
        # encryption_speed.append(round(1000 * encrypted_size[i] / encryption_time[i], 3))
        # decryption_speed.append(round(1000 * encrypted_size[i] / decryption_time[i], 3))

        # encryption_speed.append(1000 * encrypted_size[i] / encryption_time[i])
        # decryption_speed.append(1000 * encrypted_size[i] / decryption_time[i])

        encryption_avg = 0
        decryption_avg = 0
        data_sizes.append(encrypted_size[i])

        for j in range(0, 3):
            encryption_avg += 1000 * encrypted_size[i + j] / encryption_time[i + j]
            decryption_avg += 1000 * encrypted_size[i + j] / decryption_time[i + j]

        encryption_avg = round(encryption_avg / 3, 3)
        decryption_avg = round(decryption_avg / 3, 3)

        encryption_speed.append(encryption_avg)
        decryption_speed.append(decryption_avg)

        padding_length.append(encrypted_size[i] - decrypted_size[i])

    print(encryption_speed)
    print(decryption_speed)
    # print(padding_length)

    # print(np.mean(encryption_speed))
    # print(np.mean(decryption_speed))
    # print(np.mean(padding_length))

    """plt.plot(data_sizes, decryption_speed, color="blue")
    plt.scatter(data_sizes, decryption_speed, color="blue", marker="o", label="Decryption")

    plt.plot(data_sizes, encryption_speed, color="red")
    plt.scatter(data_sizes, encryption_speed, color="red", marker="x", label="Encryption")

    plt.title("Speed vs. data size", fontsize=18)
    plt.xlabel("Data size [bytes]", fontsize=16)
    plt.ylabel("Speed [bytes/second]", fontsize=16)

    plt.grid(True, which="both")
    plt.minorticks_on()
    plt.tight_layout()
    plt.legend()
    plt.savefig(f"encryption_data_size_vs_speed.png")
    plt.close()"""
    # plt.show()


def overall():
    # FOR OVERALL FUNCTIONALITY TESTS
    data_size = [66, 66, 66, 132, 132, 132, 198, 198, 198, 264, 264, 264, 330, 330, 330, 396, 396, 396, 462, 462, 462, 528, 528, 528]
    out_size = [51, 53, 53, 100, 97, 96, 139, 143, 139, 189, 176, 168, 200, 207, 212, 226, 227, 219, 257, 252, 262, 277, 269, 266]
    compression_time = [99.247, 103.082, 100.752, 181.284, 170.482, 164.210, 230.942, 241.268, 233.201, 312.225, 294.273, 277.431, 325.992, 331.882, 349.422, 365.462, 357.379, 352.770, 398.513, 397.882, 417.537, 436.984, 424.623, 414.983]
    decompression_time = [81.695, 82.050, 85.665, 159.545, 156.233, 154.892, 229.303, 233.041, 229.045, 305.079, 304.629, 302.546, 377.054, 375.696, 377.768, 448.838, 450.288, 448.132, 525.047, 522.209, 522.628, 596.212, 592.654, 592.631]

    encrypted_size = [56, 56, 56, 104, 104, 96, 144, 144, 144, 192, 176, 168, 200, 208, 216, 232, 232, 224, 264, 256, 264, 280, 272, 272]
    decrypted_size = [51, 53, 53, 100, 97, 96, 139, 143, 139, 189, 176, 168, 200, 207, 212, 226, 227, 219, 257, 252, 262, 277, 269, 266]
    encryption_time = [67.316, 67.357, 69.482, 120.919, 120.419, 114.727, 165.888, 163.468, 162.382, 215.970, 197.294, 191.863, 226.650, 231.870, 242.458, 258.948, 257.843, 253.115, 296.662, 287.696, 291.973, 310.615, 301.723, 302.412]
    decryption_time = [76.983, 81.268, 77.736, 149.486, 112.123, 111.723, 160.940, 165.672, 158.876, 215.997, 201.156, 194.503, 232.821, 238.365, 241.614, 259.032, 260.137, 250.894, 288.751, 281.429, 289.880, 307.168, 298.265, 295.810]

    compression_speed = []
    decompression_speed = []
    compression_ratio = []
    decryption_speed = []
    encryption_speed = []
    overall_time = []
    data_sizes = []

    for i in range(0, len(data_size), 3):
        compression_avg = 0
        decompression_avg = 0
        encryption_avg = 0
        decryption_avg = 0
        compression_ratio_avg = 0
        data_sizes.append(data_size[i])

        for j in range(0, 3):
            compression_avg += 1000 * data_size[i + j] / compression_time[i + j]
            decompression_avg += 1000 * data_size[i + j] / decompression_time[i + j]
            compression_ratio_avg += data_size[i + j] / out_size[i + j]
            encryption_avg += 1000 * encrypted_size[i + j] / encryption_time[i + j]
            decryption_avg += 1000 * encrypted_size[i + j] / decryption_time[i + j]
            #compression_avg += compression_time[i + j]
            #decompression_avg += decompression_time[i + j]
            #encryption_avg += encryption_time[i + j]
            #decryption_avg += decryption_time[i + j]

        compression_avg = round(compression_avg / 3, 3)
        decompression_avg = round(decompression_avg / 3, 3)
        compression_ratio_avg = round(compression_ratio_avg / 3, 3)
        encryption_avg = round(encryption_avg / 3, 3)
        decryption_avg = round(decryption_avg / 3, 3)

        compression_speed.append(compression_avg)
        decompression_speed.append(decompression_avg)
        compression_ratio.append(compression_ratio_avg)
        encryption_speed.append(encryption_avg)
        decryption_speed.append(decryption_avg)

        # overall_time.append(compression_avg + decompression_avg + encryption_avg + decryption_avg)

    print(compression_speed)
    print(decompression_speed)
    print(compression_ratio)
    print(encryption_speed)
    print(decryption_speed)

    """plt.plot(overall_time, data_sizes, color="blue")
    plt.scatter(overall_time, data_sizes, color="blue", marker="o", label="Overall")

    plt.title("Data size vs. time", fontsize=18)
    plt.xlabel("Time [ms]", fontsize=16)
    plt.ylabel("Data size [bytes]", fontsize=16)

    plt.grid(True, which="both")
    plt.minorticks_on()
    plt.tight_layout()
    # plt.legend()
    # plt.savefig(f"overall_time_vs_size.png")
    # plt.close()
    plt.show()"""


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
    encryption()
    # overall()
    # checksum()
