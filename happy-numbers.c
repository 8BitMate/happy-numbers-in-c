#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    size_t size;
    uint8_t *vector;
} byte_vector;

typedef struct {
    size_t size;
    uint32_t *vector;
} int_vector;

byte_vector new_byte_vector(size_t size) {
    byte_vector b_vector;
    b_vector.size = size;
    b_vector.vector = calloc(size, sizeof(uint8_t));
    return b_vector;
}

int_vector new_int_vector(size_t size) {
    int_vector i_vector;
    i_vector.size = size;
    i_vector.vector = calloc(size, sizeof(uint32_t));
    return i_vector;
}

void free_int_vector(int_vector* vector) {
    free(vector -> vector);
}

void free_byte_vector(byte_vector* vector) {
    free(vector -> vector);
}

uint32_t sum_of_squared_digits(uint32_t num) {
    uint32_t rem;
    uint32_t sum = 0;
    while (num) {
        rem = num % 10;
        num /= 10;
        sum += rem * rem;
    }
    return sum;
}

uint32_t number_of_digits(uint32_t num) {
    uint32_t count;
    for (count = 0; num; ++count) {
        num /= 10;
    }
    return count == 0 ? 1 : count;
}

static void set_happy(uint8_t *happy_vector, size_t n) {
    size_t byte = n >> 3; //n / 8;
    size_t byte_index = n & 7; //n % 8
    happy_vector[byte] |= (1 << byte_index);
}

static uint8_t is_happy(uint8_t *happy_vector, size_t n) {
    size_t byte = n >> 3; //n / 8;
    size_t byte_index = n & 7; //n % 8
    return happy_vector[byte] & (1 << byte_index);
}

static size_t find_first(size_t from, size_t to, uint32_t *temp_seen,
                         uint8_t *seen, uint8_t *happy_checker, uint8_t *happy) {
    size_t happy_count = 0;
    for (size_t i = from; i < to; i++) {
        uint32_t square = sum_of_squared_digits(i);
        size_t seen_count = 0;
        seen[i] = 1;
        temp_seen[seen_count++] = i;
        while (!seen[square]) {
                seen[square] = 1;
                temp_seen[seen_count++] = square;
                square = sum_of_squared_digits(square);
        }
        if (square == 1 || happy_checker[square]) {
            ++happy_count;
            set_happy(happy, i);
            for (size_t j = 0; j < seen_count; j++) {
                happy_checker[temp_seen[j]] = 1;
            }
        }
    }
    return happy_count;
}

static size_t find_rest(size_t from, size_t to, uint8_t *happy_checker, uint8_t *happy) {
    size_t happy_count = 0;
    for (size_t i = from; i < to; i++){
        uint32_t square = sum_of_squared_digits(i);
        if (happy_checker[square]){
            ++happy_count;
            set_happy(happy, i);
        }
    }
    return happy_count;
}

static void copy_numbers(size_t happy_count, uint8_t *happy, uint32_t *happy_numbers) {
    for (size_t h_counter = 0, h_number = 0; h_counter < happy_count; h_number++){
        if (is_happy(happy, h_number)) {
            happy_numbers[h_counter++] = h_number;
        }
    }
}

int_vector find_happy_numbers(uint32_t n) {
    uint32_t num_digits = number_of_digits(n);
    size_t seen_size = num_digits > 3 ? num_digits * 81 + 1: 81 * 3 + 1;
    int_vector temp_seen = new_int_vector(seen_size);
    byte_vector seen = new_byte_vector(seen_size);
    byte_vector happy_checker = new_byte_vector(seen_size);
    byte_vector happy = new_byte_vector(n / 8 + 1);
    size_t to = (seen_size < n ? seen_size : n);

    size_t happy_count = find_first(1, to, temp_seen.vector, seen.vector,
                                    happy_checker.vector, happy.vector);

    free_int_vector(&temp_seen);
    free_byte_vector(&seen);

    happy_count += find_rest(seen_size, n, happy_checker.vector, happy.vector);
    int_vector happy_numbers = new_int_vector(happy_count);
    copy_numbers(happy_count, happy.vector, happy_numbers.vector);

    free_byte_vector(&happy_checker);
    free_byte_vector(&happy);
    return happy_numbers;
}

int main(int argc, char *argv[]) {
    uint32_t n = 1000;
    if (argc > 1) {
        char *endptr = argv[1];
        n = (uint32_t) strtol(argv[1], &endptr, 10);
        if (!argv[1] || (*endptr) || (int32_t) n < 0) { //Not a valid number
            printf("Invalid number!\n");
            exit(0);
        }
    } else {
        printf("Usage: %s <n>\nUsing default value %d\n", argv[0], n);
    }
    int_vector numbers = find_happy_numbers(n);
    size_t size = numbers.size;
    uint32_t *vector = numbers.vector;
    printf("The fifty largest happy numbers found:\n");
    for (size_t i = size < 50 ? 0 : size - 50; i < size; ++i) {
        printf("%d\n", vector[i]);
    }
    free_int_vector(&numbers);
    return 0;
}
