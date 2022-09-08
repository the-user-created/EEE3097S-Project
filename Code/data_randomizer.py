import random
from main import compress, encrypt, decompress, decrypt, compare_files


def read_data(file_name):
    with open(file=file_name, mode="r") as data_file:
        lines = data_file.readlines()

    return lines


def add_randomness(lines):
    new_lines = [lines[0]]
    for line in lines[1:]:
        values = line.strip().split(",")
        new_values = [values[0], values[1]]
        for value in values[2:]:
            try:
                float_value = eval(value)
                rand_value = random.uniform(0, float_value * (-1 if float_value < 0 else 1) * 0.1)
                if values.index(value) == len(values) - 1:
                    new_values.append(str(eval(value) + rand_value) + "\n")
                else:
                    new_values.append(str(eval(value) + rand_value))
            except SyntaxError:
                new_values.append(value)
        new_lines.append(','.join(new_values))

    return new_lines


def write_data(file_name, lines):
    with open(file=file_name, mode="w+") as data_file:
        for line in lines:
            data_file.write(line)


def main():
    lines = read_data(file_name="sample_data/EEE3097S_2022_Turntable_Example_Data.csv")
    new_lines = add_randomness(lines=lines)
    write_data(file_name="sample_data/EEE3097S_2022_Turntable_Example_Data_random.csv", lines=new_lines)

    input_file = "EEE3097S_2022_Turntable_Example_Data_random.csv"
    compress(input_file="sample_data/" + input_file, output_file="sample_data/results/" + input_file + ".lz")
    print()
    encrypt(input_file="sample_data/results/" + input_file + ".lz",
            output_file="sample_data/results/" + input_file + ".lz.tf")
    print()
    decrypt(input_file="sample_data/results/" + input_file + ".lz.tf",
            output_file="sample_data/results/" + input_file + ".lz")
    print()
    decompress(input_file="sample_data/results/" + input_file + ".lz", output_file="sample_data/results/" + input_file)
    print()
    compare_files("sample_data/" + input_file, "sample_data/results/" + input_file)


if __name__ == '__main__':
    main()
