#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <tapa.h>
#include <gflags/gflags.h>
#include <ap_int.h>

constexpr int input_size = 256;     // Number of input features
constexpr int hidden_size1 = 1024;   // Number of neurons in the first hidden layer
constexpr int hidden_size2 = 2048;   // Number of neurons in the second hidden layer
constexpr int output_size = 256;    // Number of output classes

constexpr int weight_size1 = input_size * hidden_size1 / 16;
constexpr int weight_size2 = hidden_size1 * hidden_size2 / 16;
constexpr int weight_size3 = hidden_size2 * output_size / 16;

using int16_v16 = tapa::vec_t<ap_int<16>, 16>;

void MLP(
    tapa::mmap<int16_v16> X,
    tapa::mmap<int16_v16> W1,
    tapa::mmap<int16_v16> W2,
    tapa::mmap<int16_v16> W3,
    tapa::mmap<int16_v16> data_out,
    tapa::mmap<int> cycle_count
);

template <typename T>
using aligned_vector = std::vector<T, tapa::aligned_allocator<T>>;

DEFINE_string(bitstream, "", "path to bitstream file");

int main(int argc, char *argv[]){
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    const int L = argc > 1 ? atoll(argv[1]) : input_size;

    srand((unsigned)time(nullptr));

    // Example input and weight matrices

    aligned_vector<ap_int<16>> X(input_size);
    aligned_vector<ap_int<16>> W1(weight_size1*16);
    aligned_vector<ap_int<16>> W2(weight_size2*16);
    aligned_vector<ap_int<16>> W3(weight_size3*16);
    aligned_vector<ap_int<16>> data_out(output_size);
    aligned_vector<int> cycle_count(1);

    int64_t kernel_time_ns = tapa::invoke(MLP, FLAGS_bitstream,
        tapa::read_only_mmap<ap_int<16>>(X).reinterpret<int16_v16>(),
        tapa::read_only_mmap<ap_int<16>>(W1).reinterpret<int16_v16>(),
        tapa::read_only_mmap<ap_int<16>>(W2).reinterpret<int16_v16>(),
        tapa::read_only_mmap<ap_int<16>>(W3).reinterpret<int16_v16>(),
        tapa::write_only_mmap<ap_int<16>>(data_out).reinterpret<int16_v16>(),
        tapa::write_only_mmap<int>(cycle_count));

    std::cout << "Cycle count: " << cycle_count[0] << std::endl;
    std::cout << "Latency: " << kernel_time_ns * 1e-9 << " s" << std::endl;

    return 0;
}
