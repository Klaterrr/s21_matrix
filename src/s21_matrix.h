#ifndef S21_MATRIX_H_
#define S21_MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/**
 * @file s21_matrix.h
 * @author Johan Kamiyama (a.k.a \@kuretaru or shanaecl)
 * @brief Заголовочный файл для библиотеки работы с матрицами.
 * @details Содержит объявление структуры matrix_t и прототипы всех
 *          публичных функций для создания, удаления, сравнения матриц,
 *          а также для выполнения арифметических и алгебраических операций.
 * @version 1.0
 * @date 2025-06-30
 */

// --- Макросы и перечисления ---

#define SUCCESS 1
#define FAILURE 0

#define S21_EPS 1e-7

/**
 * @brief Структура для представления матрицы.
 */
typedef struct matrix_struct {
  double **matrix; /**< Указатель на двумерный массив элементов матрицы. */
  int rows;        /**< Количество строк. */
  int columns;     /**< Количество столбцов. */
} matrix_t;

/**
 * @brief Перечисление кодов возврата для функций библиотеки.
 */
typedef enum {
  OK = 0,                /**< 0 - Успешное выполнение */
  INCORRECT_MATRIX = 1,  /**< 1 - Ошибка, некорректная матрица */
  CALCULATION_ERROR = 2, /**< 2 - Ошибка вычисления */
} result_code_t;

// =================================================================================
// Группировка функций для удобной навигации в Doxygen
// =================================================================================

/**
 * @defgroup matrix_core Основные функции
 * @brief Функции для создания, удаления и сравнения матриц.
 */

/**
 * @defgroup matrix_arithmetic Арифметические операции
 * @brief Функции для сложения, вычитания и умножения матриц.
 */

/**
 * @defgroup matrix_algebra Алгебраические операции
 * @brief Функции для вычисления определителя, обратной матрицы и т.д.
 */


// --- Основные функции API ---

/**
 * @ingroup matrix_core
 * @brief Создает матрицу заданного размера.
 * @param[in] rows Количество строк (должно быть > 0).
 * @param[in] columns Количество столбцов (должно быть > 0).
 * @param[out] result Указатель на структуру, куда будет сохранена созданная матрица.
 * @return OK (0) в случае успеха, иначе INCORRECT_MATRIX (1).
 */
int s21_create_matrix(int rows, int columns, matrix_t *result);

/**
 * @ingroup matrix_core
 * @brief Освобождает память, выделенную под матрицу.
 * @details Безопасно обрабатывает NULL указатели. После выполнения обнуляет поля структуры.
 * @param[in,out] A Указатель на матрицу для удаления.
 */
void s21_remove_matrix(matrix_t *A);

/**
 * @ingroup matrix_core
 * @brief Сравнивает две матрицы на идентичность с точностью до S21_EPS.
 * @param[in] A Указатель на первую матрицу.
 * @param[in] B Указатель на вторую матрицу.
 * @return SUCCESS (1) если матрицы равны, иначе FAILURE (0).
 */
int s21_eq_matrix(matrix_t *A, matrix_t *B);

/**
 * @ingroup matrix_arithmetic
 * @brief Складывает две матрицы.
 * @details Складывает матрицу B с матрицей A и сохраняет результат в `result`.
 *          Матрицы A и B должны иметь одинаковые размеры. Функция сама
 *          создает результирующую матрицу `result`.
 * @param[in] A Указатель на первое слагаемое.
 * @param[in] B Указатель на второе слагаемое.
 * @param[out] result Указатель на результирующую матрицу (сумма).
 * @return Код результата операции:
 * @return OK (0) - в случае успеха.
 * @return INCORRECT_MATRIX (1) - если одна из матриц A или B некорректна.
 * @return CALCULATION_ERROR (2) - если размеры матриц A и B не совпадают.
 */
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/**
 * @ingroup matrix_arithmetic
 * @brief Вычитает одну матрицу из другой.
 * @details Вычитает матрицу B из матрицы A и сохраняет результат в `result`.
 *          Матрицы A и B должны иметь одинаковые размеры. Функция сама
 *          создает результирующую матрицу `result`.
 * @param[in] A Указатель на уменьшаемое.
 * @param[in] B Указатель на вычитаемое.
 * @param[out] result Указатель на результирующую матрицу (разность).
 * @return Код результата операции:
 * @return OK (0) - в случае успеха.
 * @return INCORRECT_MATRIX (1) - если одна из матриц A или B некорректна.
 * @return CALCULATION_ERROR (2) - если размеры матриц A и B не совпадают.
 */
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/**
 * @ingroup matrix_arithmetic
 * @brief Умножает матрицу на скалярное значение.
 * @details Умножает каждый элемент матрицы A на число `number` и сохраняет
 *          результат в `result`. Функция сама создает результирующую матрицу `result`.
 * @param[in] A Указатель на матрицу для умножения.
 * @param[in] number Скалярное значение, на которое умножается матрица.
 * @param[out] result Указатель на результирующую матрицу (произведение).
 * @return Код результата операции:
 * @return OK (0) - в случае успеха.
 * @return INCORRECT_MATRIX (1) - если матрица A некорректна или не удалось
 *                                создать результирующую матрицу.
 */
int s21_mult_number(matrix_t *A, double number, matrix_t *result);

/**
 * @ingroup matrix_arithmetic
 * @brief Умножает две матрицы.
 * @param[in] A Указатель на первую матрицу-множитель.
 * @param[in] B Указатель на вторую матрицу-множитель.
 * @param[out] result Указатель на результирующую матрицу (произведение).
 * @return OK (0) - успех.
 * @return INCORRECT_MATRIX (1) - если одна из матриц некорректна.
 * @return CALCULATION_ERROR (2) - если количество столбцов A не равно количеству строк B.
 */
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/**
 * @ingroup matrix_algebra
 * @brief Транспонирует матрицу.
 * @param[in] A Указатель на исходную матрицу.
 * @param[out] result Указатель на транспонированную матрицу.
 * @return OK (0), INCORRECT_MATRIX (1).
 */
int s21_transpose(matrix_t *A, matrix_t *result);

/**
 * @ingroup matrix_algebra
 * @brief Вычисляет определитель матрицы.
 * @param[in] A Указатель на матрицу.
 * @param[out] result Указатель на переменную для сохранения определителя.
 * @warning Исходная матрица A должна быть квадратной.
 * @return OK (0), INCORRECT_MATRIX (1), CALCULATION_ERROR (2).
 */
int s21_determinant(matrix_t *A, double *result);

/**
 * @ingroup matrix_algebra
 * @brief Вычисляет матрицу алгебраических дополнений.
 * @param[in] A Указатель на исходную матрицу.
 * @param[out] result Указатель на результирующую матрицу.
 * @warning Исходная матрица A должна быть квадратной.
 * @return OK (0) - успех.
 * @return INCORRECT_MATRIX (1) - если матрица некорректна.
 * @return CALCULATION_ERROR (2) - если матрица не квадратная.
 */
int s21_calc_complements(const matrix_t *A, matrix_t *result);

/**
 * @ingroup matrix_algebra
 * @brief Находит обратную матрицу.
 * @details Обратная матрица существует только для квадратных матриц с ненулевым определителем.
 * @param[in] A Указатель на исходную матрицу.
 * @param[out] result Указатель на результирующую обратную матрицу.
 * @return OK (0) - успех.
 * @return INCORRECT_MATRIX (1) - если матрица некорректна.
 * @return CALCULATION_ERROR (2) - если матрица не квадратная или ее определитель равен 0.
 */
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

#endif  // S21_MATRIX_H_