import os
import random
import secrets
import shutil
import subprocess

def execute_command(cmd):
    ret = subprocess.call(cmd, shell=True)
    if ret != 0:
        raise RuntimeError('Exit code {} while executing "{}"'.format(ret, cmd))


def run_program(exe_name, input_filename, output_filename):
    execute_command('{} {} {}'.format(exe_name, input_filename, output_filename))


def check_random_bytes():
    cur_dir = os.getcwd()
    exe_name = os.path.join(cur_dir, '8.EXE')
    test_dir = os.path.join(cur_dir, "random_bytes")
    if os.path.isdir(test_dir):
        shutil.rmtree(test_dir)
    os.mkdir(test_dir)

    for content_length in range(10):
        cur_len_dir = os.path.join(test_dir, str(content_length))
        os.mkdir(cur_len_dir)
        if content_length == 0:
            input_filename = os.path.join(cur_len_dir, '0.in')
            output_filename = os.path.join(cur_len_dir, '0.out')
            open(input_filename, 'a').close()
            run_program(exe_name, input_filename, output_filename)
        else:
            for i in range(10):
                input_filename = os.path.join(cur_len_dir, '{}.in'.format(i))
                output_filename = os.path.join(cur_len_dir, '{}.out'.format(i))
                random_bytes = secrets.token_bytes(content_length)
                with open(input_filename, 'wb') as f:
                    f.write(random_bytes)
                run_program(exe_name, input_filename, output_filename)


def check_one_byte_change():
    cur_dir = os.getcwd()
    exe_name = os.path.join(cur_dir, '8.EXE')
    test_dir = os.path.join(cur_dir, "one_byte_change")
    if os.path.isdir(test_dir):
        shutil.rmtree(test_dir)
    os.mkdir(test_dir)

    content_length = 10
    cur_len_dir = os.path.join(test_dir, str(content_length))
    os.mkdir(cur_len_dir)
    random_bytes = secrets.token_bytes(content_length)
    input_filename = os.path.join(cur_len_dir, 'original.in')
    output_filename = os.path.join(cur_len_dir, 'original.out')
    with open(input_filename, 'wb') as f:
        f.write(random_bytes)
    run_program(exe_name, input_filename, output_filename)
    for i in range(content_length):
        new_byte = secrets.token_bytes(1)
        while new_byte == random_bytes[i]:
            new_byte = secrets.token_bytes(1)
        new_random_bytes = list(random_bytes)
        new_random_bytes[i] = list(new_byte)[0]
        new_random_bytes = bytes(new_random_bytes)
        input_filename = os.path.join(cur_len_dir, '{}.in'.format(i))
        output_filename = os.path.join(cur_len_dir, '{}.out'.format(i))
        with open(input_filename, 'wb') as f:
            f.write(new_random_bytes)
        run_program(exe_name, input_filename, output_filename)


def check_zero_bytes():
    cur_dir = os.getcwd()
    exe_name = os.path.join(cur_dir, '8.EXE')
    test_dir = os.path.join(cur_dir, "zero_bytes")
    if os.path.isdir(test_dir):
        shutil.rmtree(test_dir)
    os.mkdir(test_dir)

    for content_length in range(200):
        zero_bytes = bytes([0] * content_length)
        input_filename = os.path.join(test_dir, '{}.in'.format(content_length))
        output_filename = os.path.join(test_dir, '{}.out'.format(content_length))
        with open(input_filename, 'wb') as f:
            f.write(zero_bytes)
        run_program(exe_name, input_filename, output_filename)


def check_custom_bytes():
    cur_dir = os.getcwd()
    exe_name = os.path.join(cur_dir, '8.EXE')
    test_dir = os.path.join(cur_dir, "all_bytes")
    if os.path.isdir(test_dir):
        shutil.rmtree(test_dir)
    os.mkdir(test_dir)

    for byte in range(256):
        content_length = 100
        bytes_array = bytes([byte] * content_length)
        input_filename = os.path.join(test_dir, '{}.in'.format(byte))
        output_filename = os.path.join(test_dir, '{}.out'.format(byte))
        with open(input_filename, 'wb') as f:
            f.write(bytes_array)
        run_program(exe_name, input_filename, output_filename)


def read_bytes():
    cur_dir = os.getcwd()
    test_dir = os.path.join(cur_dir, "all_bytes")

    for byte in range(256):
        output_filename = os.path.join(test_dir, '{}.out'.format(byte))
        print("std::array<byte_t, 16>{ ", end="")
        with open(output_filename, 'rb') as f:
            data = list(f.read(16))
            for elem in data:
                print("0x{0:0{1}X}, ".format(elem, 2), end="")
        print("}, ")


def main():
    #check_random_bytes()
    #check_one_byte_change()
    #check_zero_bytes()
    #check_custom_bytes()
    read_bytes()


if __name__ == "__main__":
    main()
