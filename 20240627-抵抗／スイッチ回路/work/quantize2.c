/*
2024/06/26 ChatGPTに問い合わせて作ってもらったプログラム









*/



#include <stdio.h>
#include <math.h>

// 量子化関数
unsigned int quantize(double input_value, double full_scale, int num_bits) {
    // 最大値と最小値を計算
    double max_value = pow(2.0, num_bits) - 1.0;
    double min_value = 0.0;

    // 実数値をフルスケールにスケーリング
    double scaled_value = (input_value - min_value) / (full_scale - min_value);

    // 量子化
    unsigned int quantized_value = (unsigned int)round(scaled_value * max_value);

    return quantized_value;
}

// メイン関数
int main() {
    double input_value = 3.14159; // 量子化したい実数値
    double full_scale = 5.0; // ADコンバータのフルスケール値
    int num_bits = 12; // 量子化ビット数

    // 量子化
    unsigned int quantized_result = quantize(input_value, full_scale, num_bits);

    printf("Input value: %f\n", input_value);
    printf("Quantized value (using %d bits): %u\n", num_bits, quantized_result);

    return 0;
}
