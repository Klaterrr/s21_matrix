/**
 * @file s21_matrix_test.c
 * @author Johan Kamiyama (a.k.a @kuretaru or shanaecl)
 * @brief Юнит-тесты для библиотеки s21_matrix.
 * @details Используется фреймворк Check для организации и запуска тестов.
 *          Тесты сгруппированы по тестируемым функциям.
 * @version 1.0
 * @date 2025-06-30
 */

#include <check.h>
#include "../s21_matrix.h"

// =================================================================================
// Группировка тестов для удобной навигации
// =================================================================================

/**
 * @defgroup unit_tests Юнит-тесты
 * @brief Набор тестов для проверки корректности работы библиотеки s21_matrix.
 */

/**
 * @defgroup test_create Тесты s21_create_matrix
 * @ingroup unit_tests
 */
/**
 * @defgroup test_eq Тесты s21_eq_matrix
 * @ingroup unit_tests
 */
/**
 * @defgroup test_sum_sub Тесты s21_sum_matrix и s21_sub_matrix
 * @ingroup unit_tests
 */
/**
 * @defgroup test_mult Тесты операций умножения
 * @ingroup unit_tests
 */
/**
 * @defgroup test_transform Тесты трансформаций матриц
 * @ingroup unit_tests
 */
/**
 * @defgroup test_algebra Тесты алгебраических операций
 * @ingroup unit_tests
 */

// =================================================================================
// --- Тесты для s21_create_matrix ---
// =================================================================================

/**
 * @ingroup test_create
 * @brief Проверка успешного создания матрицы стандартного размера.
 */
START_TEST(test_create_matrix_1) {
  matrix_t A = {NULL, 0, 0};
  int rows = 3, columns = 5;
  int code = s21_create_matrix(rows, columns, &A);
  ck_assert_int_eq(code, OK);
  ck_assert_int_eq(A.rows, rows);
  ck_assert_int_eq(A.columns, columns);
  s21_remove_matrix(&A);
}
END_TEST

/**
 * @ingroup test_create
 * @brief Проверка ошибки создания матрицы с нулевым количеством строк.
 */
START_TEST(test_create_matrix_fail_1) {
  matrix_t A = {NULL, 0, 0};
  int code = s21_create_matrix(0, 5, &A);
  ck_assert_int_eq(code, INCORRECT_MATRIX);
}
END_TEST

/**
 * @ingroup test_create
 * @brief Проверка ошибки создания матрицы с отрицательным количеством столбцов.
 */
START_TEST(test_create_matrix_fail_2) {
  matrix_t A = {NULL, 0, 0};
  int code = s21_create_matrix(3, -1, &A);
  ck_assert_int_eq(code, INCORRECT_MATRIX);
}
END_TEST

// =================================================================================
// --- Тесты для s21_eq_matrix ---
// =================================================================================

/**
 * @ingroup test_eq
 * @brief Проверка сравнения двух полностью идентичных матриц.
 */
START_TEST(test_eq_matrix_1) {
  matrix_t A = {NULL, 0, 0};
  matrix_t B = {NULL, 0, 0};
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = i + j;
      B.matrix[i][j] = i + j;
    }
  }
  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

/**
 * @ingroup test_eq
 * @brief Проверка сравнения матриц, отличающихся одним элементом.
 */
START_TEST(test_eq_matrix_fail_1) {
  matrix_t A = {NULL, 0, 0};
  matrix_t B = {NULL, 0, 0};
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = i + j;
      B.matrix[i][j] = i + j;
    }
  }
  B.matrix[2][2] = 100;
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

/**
 * @ingroup test_eq
 * @brief Проверка сравнения матриц разного размера.
 */
START_TEST(test_eq_matrix_fail_2) {
  matrix_t A = {NULL, 0, 0};
  matrix_t B = {NULL, 0, 0};
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(2, 3, &B);
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

/**
 * @ingroup test_eq
 * @brief Проверка сравнения вещественных чисел с заданной точностью S21_EPS.
 */
START_TEST(test_eq_matrix_precision) {
  matrix_t A = {NULL, 0, 0};
  matrix_t B = {NULL, 0, 0};
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  A.matrix[0][0] = 0.12345678;
  B.matrix[0][0] = 0.12345679;
  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS); // Разница меньше S21_EPS
  B.matrix[0][0] = 0.1234569;
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE); // Разница больше S21_EPS
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

// =================================================================================
// --- Тесты для s21_sum_matrix и s21_sub_matrix ---
// =================================================================================

/**
 * @ingroup test_sum_sub
 * @brief Проверка успешного сложения двух матриц.
 */
START_TEST(test_sum_matrix_1) {
  matrix_t A = {NULL, 0, 0};
  matrix_t B = {NULL, 0, 0};
  matrix_t result = {NULL, 0, 0};
  matrix_t check = {NULL, 0, 0};
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);
  s21_create_matrix(3, 3, &check);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = i;
      B.matrix[i][j] = j;
      check.matrix[i][j] = i + j;
    }
  }
  int code = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(code, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &check), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&check);
}
END_TEST

/**
 * @ingroup test_sum_sub
 * @brief Проверка ошибки сложения матриц разного размера.
 */
START_TEST(test_sum_matrix_fail) {
  matrix_t A = {NULL, 0, 0};
  matrix_t B = {NULL, 0, 0};
  matrix_t result = {NULL, 0, 0};
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(2, 3, &B);
  int code = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(code, CALCULATION_ERROR);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

/**
 * @ingroup test_sum_sub
 * @brief Проверка успешного вычитания двух матриц.
 */
START_TEST(test_sub_matrix_1) {
  matrix_t A = {NULL, 0, 0};
  matrix_t B = {NULL, 0, 0};
  matrix_t result = {NULL, 0, 0};
  matrix_t check = {NULL, 0, 0};
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);
  s21_create_matrix(3, 3, &check);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = i * 2.5;
      B.matrix[i][j] = j * 1.5;
      check.matrix[i][j] = (i * 2.5) - (j * 1.5);
    }
  }
  int code = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(code, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &check), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&check);
}
END_TEST

// =================================================================================
// --- Тесты для операций умножения ---
// =================================================================================

/**
 * @ingroup test_mult
 * @brief Проверка успешного умножения матрицы на число.
 */
START_TEST(test_mult_number_1) {
  matrix_t A = {NULL, 0, 0};
  matrix_t result = {NULL, 0, 0};
  matrix_t check = {NULL, 0, 0};
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &check);
  double number = -2.5;
  A.matrix[0][0] = 1; A.matrix[0][1] = 2;
  A.matrix[1][0] = 3; A.matrix[1][1] = 4;
  check.matrix[0][0] = -2.5; check.matrix[0][1] = -5;
  check.matrix[1][0] = -7.5; check.matrix[1][1] = -10;

  int code = s21_mult_number(&A, number, &result);
  ck_assert_int_eq(code, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &check), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&check);
}
END_TEST

/**
 * @ingroup test_mult
 * @brief Проверка успешного умножения двух совместимых матриц.
 */
START_TEST(test_mult_matrix_1) {
  matrix_t A = {NULL, 0, 0};
  matrix_t B = {NULL, 0, 0};
  matrix_t result = {NULL, 0, 0};
  matrix_t check = {NULL, 0, 0};
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  s21_create_matrix(2, 2, &check);
  A.matrix[0][0] = 1; A.matrix[0][1] = 2; A.matrix[0][2] = 3;
  A.matrix[1][0] = 4; A.matrix[1][1] = 5; A.matrix[1][2] = 6;
  B.matrix[0][0] = 7; B.matrix[0][1] = 8;
  B.matrix[1][0] = 9; B.matrix[1][1] = 10;
  B.matrix[2][0] = 11;B.matrix[2][1] = 12;
  check.matrix[0][0] = 58; check.matrix[0][1] = 64;
  check.matrix[1][0] = 139; check.matrix[1][1] = 154;

  int code = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(code, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &check), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&check);
}
END_TEST

/**
 * @ingroup test_mult
 * @brief Проверка ошибки умножения двух несовместимых матриц.
 */
START_TEST(test_mult_matrix_fail) {
  matrix_t A = {NULL, 0, 0};
  matrix_t B = {NULL, 0, 0};
  matrix_t result = {NULL, 0, 0};
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(4, 2, &B);
  int code = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(code, CALCULATION_ERROR);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

// =================================================================================
// --- Тесты для трансформаций матриц ---
// =================================================================================

/**
 * @ingroup test_transform
 * @brief Проверка успешного транспонирования матрицы.
 */
START_TEST(test_transpose_1) {
  matrix_t A = {NULL, 0, 0};
  matrix_t result = {NULL, 0, 0};
  matrix_t check = {NULL, 0, 0};
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &check);
  A.matrix[0][0] = 1; A.matrix[0][1] = 2; A.matrix[0][2] = 3;
  A.matrix[1][0] = 4; A.matrix[1][1] = 5; A.matrix[1][2] = 6;
  check.matrix[0][0] = 1; check.matrix[0][1] = 4;
  check.matrix[1][0] = 2; check.matrix[1][1] = 5;
  check.matrix[2][0] = 3; check.matrix[2][1] = 6;

  int code = s21_transpose(&A, &result);
  ck_assert_int_eq(code, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &check), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&check);
}
END_TEST

// =================================================================================
// --- Тесты для алгебраических операций ---
// =================================================================================

/**
 * @ingroup test_algebra
 * @brief Проверка вычисления определителя для матрицы с нулевым определителем.
 */
START_TEST(test_determinant_1) {
  matrix_t A = {NULL, 0, 0};
  double result;
  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1; A.matrix[0][1] = 2; A.matrix[0][2] = 3;
  A.matrix[1][0] = 4; A.matrix[1][1] = 5; A.matrix[1][2] = 6;
  A.matrix[2][0] = 7; A.matrix[2][1] = 8; A.matrix[2][2] = 9;
  
  int code = s21_determinant(&A, &result);
  ck_assert_int_eq(code, OK);
  ck_assert_double_eq_tol(result, 0, S21_EPS);
  s21_remove_matrix(&A);
}
END_TEST

/**
 * @ingroup test_algebra
 * @brief Проверка вычисления определителя для матрицы 2x2.
 */
START_TEST(test_determinant_2) {
  matrix_t A = {NULL, 0, 0};
  double result;
  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = -5; A.matrix[0][1] = 1.5;
  A.matrix[1][0] = 4; A.matrix[1][1] = 2;

  int code = s21_determinant(&A, &result);
  ck_assert_int_eq(code, OK);
  ck_assert_double_eq_tol(result, -16, S21_EPS);
  s21_remove_matrix(&A);
}
END_TEST

/**
 * @ingroup test_algebra
 * @brief Проверка ошибки вычисления определителя для неквадратной матрицы.
 */
START_TEST(test_determinant_fail) {
  matrix_t A = {NULL, 0, 0};
  double result;
  s21_create_matrix(2, 3, &A);
  int code = s21_determinant(&A, &result);
  ck_assert_int_eq(code, CALCULATION_ERROR);
  s21_remove_matrix(&A);
}
END_TEST

/**
 * @ingroup test_algebra
 * @brief Проверка успешного вычисления матрицы алгебраических дополнений.
 */
START_TEST(test_complements_1) {
  matrix_t A = {NULL, 0, 0};
  matrix_t result = {NULL, 0, 0};
  matrix_t check = {NULL, 0, 0};
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &check);
  A.matrix[0][0] = 1; A.matrix[0][1] = 2; A.matrix[0][2] = 3;
  A.matrix[1][0] = 0; A.matrix[1][1] = 4; A.matrix[1][2] = 2;
  A.matrix[2][0] = 5; A.matrix[2][1] = 2; A.matrix[2][2] = 1;

  check.matrix[0][0] = 0;   check.matrix[0][1] = 10; check.matrix[0][2] = -20;
  check.matrix[1][0] = 4;   check.matrix[1][1] = -14;check.matrix[1][2] = 8;
  check.matrix[2][0] = -8;  check.matrix[2][1] = -2; check.matrix[2][2] = 4;

  int code = s21_calc_complements(&A, &result);
  ck_assert_int_eq(code, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &check), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&check);
}
END_TEST

/**
 * @ingroup test_algebra
 * @brief Проверка ошибки вычисления дополнений для неквадратной матрицы.
 */
START_TEST(test_complements_fail) {
  matrix_t A = {NULL, 0, 0};
  matrix_t result = {NULL, 0, 0};
  s21_create_matrix(2, 3, &A);
  int code = s21_calc_complements(&A, &result);
  ck_assert_int_eq(code, CALCULATION_ERROR);
  s21_remove_matrix(&A);
}
END_TEST

/**
 * @ingroup test_algebra
 * @brief Проверка успешного нахождения обратной матрицы.
 */
START_TEST(test_inverse_1) {
  matrix_t A = {NULL, 0, 0};
  matrix_t result = {NULL, 0, 0};
  matrix_t check = {NULL, 0, 0};
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &check);
  A.matrix[0][0] = 2; A.matrix[0][1] = 5; A.matrix[0][2] = 7;
  A.matrix[1][0] = 6; A.matrix[1][1] = 3; A.matrix[1][2] = 4;
  A.matrix[2][0] = 5; A.matrix[2][1] = -2;A.matrix[2][2] = -3;

  check.matrix[0][0] = 1;   check.matrix[0][1] = -1;  check.matrix[0][2] = 1;
  check.matrix[1][0] = -38; check.matrix[1][1] = 41;  check.matrix[1][2] = -34;
  check.matrix[2][0] = 27;  check.matrix[2][1] = -29; check.matrix[2][2] = 24;

  int code = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(code, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &check), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&check);
}
END_TEST

/**
 * @ingroup test_algebra
 * @brief Проверка ошибки нахождения обратной матрицы для вырожденной матрицы (определитель 0).
 */
START_TEST(test_inverse_fail_det_zero) {
  matrix_t A = {NULL, 0, 0};
  matrix_t result = {NULL, 0, 0};
  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1; A.matrix[0][1] = 2; A.matrix[0][2] = 3;
  A.matrix[1][0] = 4; A.matrix[1][1] = 5; A.matrix[1][2] = 6;
  A.matrix[2][0] = 7; A.matrix[2][1] = 8; A.matrix[2][2] = 9;

  int code = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(code, CALCULATION_ERROR);
  s21_remove_matrix(&A);
}
END_TEST

// --- Функция для создания и запуска набора тестов ---

/**
 * @brief Создает и конфигурирует тестовый набор (Suite).
 * @details Объединяет все тестовые случаи (TCase), сгруппированные по
 *          функциональности (основная, обработка ошибок), в один набор
 *          для последующего запуска через SRunner.
 * @return Указатель на созданный Suite.
 */
Suite *s21_matrix_suite(void) {
  Suite *s;
  TCase *tc_core, *tc_errors;

  s = suite_create("S21_Matrix");

  // Core functionality tests
  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_create_matrix_1);
  tcase_add_test(tc_core, test_eq_matrix_1);
  tcase_add_test(tc_core, test_eq_matrix_precision);
  tcase_add_test(tc_core, test_sum_matrix_1);
  tcase_add_test(tc_core, test_sub_matrix_1);
  tcase_add_test(tc_core, test_mult_number_1);
  tcase_add_test(tc_core, test_mult_matrix_1);
  tcase_add_test(tc_core, test_transpose_1);
  tcase_add_test(tc_core, test_determinant_1);
  tcase_add_test(tc_core, test_determinant_2);
  tcase_add_test(tc_core, test_complements_1);
  tcase_add_test(tc_core, test_inverse_1);
  suite_add_tcase(s, tc_core);
  
  // Error handling tests
  tc_errors = tcase_create("Errors");
  tcase_add_test(tc_errors, test_create_matrix_fail_1);
  tcase_add_test(tc_errors, test_create_matrix_fail_2);
  tcase_add_test(tc_errors, test_eq_matrix_fail_1);
  tcase_add_test(tc_errors, test_eq_matrix_fail_2);
  tcase_add_test(tc_errors, test_sum_matrix_fail);
  tcase_add_test(tc_errors, test_mult_matrix_fail);
  tcase_add_test(tc_errors, test_determinant_fail);
  tcase_add_test(tc_errors, test_complements_fail);
  tcase_add_test(tc_errors, test_inverse_fail_det_zero);
  suite_add_tcase(s, tc_errors);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = s21_matrix_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}