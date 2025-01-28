/*
2024/06/26 ChatGPT�ɖ₢���킹�č���Ă�������v���O����









*/



#include <stdio.h>
#include <math.h>

// �ʎq���֐�
unsigned int quantize(double input_value, double full_scale, int num_bits) {
    // �ő�l�ƍŏ��l���v�Z
    double max_value = pow(2.0, num_bits) - 1.0;
    double min_value = 0.0;

    // �����l���t���X�P�[���ɃX�P�[�����O
    double scaled_value = (input_value - min_value) / (full_scale - min_value);

    // �ʎq��
    unsigned int quantized_value = (unsigned int)round(scaled_value * max_value);

    return quantized_value;
}

// ���C���֐�
int main() {
    double input_value = 3.14159; // �ʎq�������������l
    double full_scale = 5.0; // AD�R���o�[�^�̃t���X�P�[���l
    int num_bits = 12; // �ʎq���r�b�g��

    // �ʎq��
    unsigned int quantized_result = quantize(input_value, full_scale, num_bits);

    printf("Input value: %f\n", input_value);
    printf("Quantized value (using %d bits): %u\n", num_bits, quantized_result);

    return 0;
}
