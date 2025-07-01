#include "s21_matrix.h"

// === Forward Declarations for Static Functions ===
static result_code_t populate_complements_matrix(const matrix_t *A, matrix_t *result);
static void perform_multiplication(const matrix_t *A, const matrix_t *B, matrix_t *result);

/**
 * @file s21_matrix.c
 * @brief Файл с реализацией функций библиотеки s21_matrix.
 */

// --- Вспомогательные (приватные) функции ---

/**
 * @brief Выделяет память под строки матрицы и инициализирует их нулями.
 * @details Вызывается после того, как память под массив указателей уже выделена.
 *          В случае ошибки выделения памяти для одной из строк, освобождает
 *          всю ранее выделенную память для этой матрицы.
 * @param[in,out] result Указатель на матрицу.
 * @return OK (0) в случае успеха, иначе INCORRECT_MATRIX (1).
 */
static result_code_t allocate_matrix_rows(matrix_t *result) {
  result_code_t return_code = OK;
  for (int i = 0; i < result->rows; ++i) {
    result->matrix[i] = (double *)calloc(result->columns, sizeof(double));
    if (result->matrix[i] == NULL) {
      // Ошибка выделения памяти для строки.
      // Освобождаем всю матрицу, включая уже выделенные строки.
      s21_remove_matrix(result);
      return_code = INCORRECT_MATRIX;
      break;
    }
  }
  return return_code;
}

/**
 * @brief Проверяет, является ли матрица корректной.
 * @param[in] A Указатель на матрицу для проверки.
 * @return 1 если матрица корректна, иначе 0.
 * @note Корректная матрица - не NULL, указатель на данные не NULL,
 *       размеры строк и столбцов положительные.
 */
static int is_matrix_valid(const matrix_t *A) {
  return (A != NULL && A->matrix != NULL && A->rows > 0 && A->columns > 0);
}

/**
 * @brief Создает минорную матрицу, исключая заданную строку и столбец.
 * @param[in] A Исходная матрица.
 * @param[in] excluding_row Индекс исключаемой строки.
 * @param[in] excluding_col Индекс исключаемого столбца.
 * @param[in] result Указатель на результирующую минорную матрицу.
 */
static void get_minor_matrix(const matrix_t *A, int excluding_row,
                             int excluding_col, matrix_t *result) {
  int res_row = 0;
  for (int i = 0; i < A->rows; ++i) {
    if (i == excluding_row) continue;
    int res_col = 0;
    for (int j = 0; j < A->columns; ++j) {
      if (j == excluding_col) continue;
      result->matrix[res_row][res_col] = A->matrix[i][j];
      res_col++;
    }
    res_row++;
  }
}

/**
 * @brief Выполняет вычисление произведения двух матриц и записывает в результирующую.
 * @details Эта вспомогательная функция предполагает, что все проверки на валидность
 *          и совместимость размеров матриц уже пройдены, и результирующая матрица 'result'
 *          уже создана и имеет корректные размеры.
 * @param[in] A Первая матрица-множитель.
 * @param[in] B Вторая матрица-множитель.
 * @param[out] result Матрица для записи результата.
 */
static void perform_multiplication(const matrix_t *A, const matrix_t *B,
                                   matrix_t *result) {
  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < B->columns; ++j) {
      result->matrix[i][j] = 0;
      for (int k = 0; k < A->columns; ++k) { // A->columns == B->rows
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
}

/**
 * @brief Копирует элементы из исходной матрицы в транспонированную.
 * @details Эта вспомогательная функция предполагает, что обе матрицы,
 *          A и result, уже существуют и имеют корректные (взаимно-транспонированные) размеры.
 * @param[in] A Исходная матрица.
 * @param[out] result Результирующая (транспонированная) матрица.
 */
static void perform_transposition(const matrix_t *A, matrix_t *result) {
  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->columns; ++j) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }
}

/**
 * @brief Заполняет матрицу алгебраическими дополнениями.
 * @details Эта вспомогательная функция итерирует по каждому элементу исходной
 *          матрицы A, вычисляет для него минор и его определитель, а затем
 *          записывает итоговое алгебраическое дополнение в матрицу result.
 *          Предполагается, что 'result' уже создана и имеет те же размеры, что и 'A'.
 * @param[in] A Исходная матрица.
 * @param[out] result Матрица для записи алгебраических дополнений.
 * @return Код результата операции: OK (0) или код ошибки от вложенных вызовов.
 */
static result_code_t populate_complements_matrix(const matrix_t *A,
                                                 matrix_t *result) {
  result_code_t return_code = OK;

  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->columns; ++j) {
      matrix_t minor = {NULL, 0, 0};
      double minor_det = 0.0;

      // Создание минора для элемента (i, j)
      return_code = s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
      if (return_code != OK) break;

      get_minor_matrix(A, i, j, &minor);

      // Определитель минора
      return_code = s21_determinant(&minor, &minor_det);
      s21_remove_matrix(&minor); // Освобождаем память из-под минора
      if (return_code != OK) break;

      // Записываем алгебраическое дополнение в результирующую матрицу
      result->matrix[i][j] = minor_det * pow(-1, i + j);
    }
    if (return_code != OK) break; // Выход из внешнего цикла при ошибке
  }
  return return_code;
}

/**
 * @brief Выполняет основную последовательность вычислений для нахождения обратной матрицы.
 * @details Эта вспомогательная функция вызывается после того, как исходная матрица
 *          проверена и ее определитель вычислен. Она находит матрицу алгебраических
 *          дополнений, транспонирует ее и умножает на (1/определитель).
 * @param[in] A Исходная матрица.
 * @param[in] det Заранее вычисленный ненулевой определитель матрицы A.
 * @param[out] result Указатель на результирующую обратную матрицу.
 * @return Код результата операции: OK (0) или код ошибки от вложенных вызовов.
 */
static result_code_t perform_inverse_calculation(const matrix_t *A, double det,
                                                 matrix_t *result) {
  result_code_t return_code = OK;
  
  // Инициализация временных матриц в безопасное состояние (NULL-указатели).
  matrix_t complements = {NULL, 0, 0};
  matrix_t transposed_complements = {NULL, 0, 0};

  // Вычисление матрицы алгебраических дополнений.
  return_code = s21_calc_complements(A, &complements);

  // Шаг 2: Транспонировать ее, если предыдущий шаг прошел успешно.
  if (return_code == OK) {
    return_code = s21_transpose(&complements, &transposed_complements);
  }

  if (return_code == OK) {
    return_code = s21_mult_number(&transposed_complements, 1.0 / det, result);
  }

  // Очищаем память, выделенную для временных матриц.
  s21_remove_matrix(&complements);
  s21_remove_matrix(&transposed_complements);

  return return_code;
}

// --- Реализация публичных функций ---

static void perform_sum(const matrix_t *A, const matrix_t *B, matrix_t *result) {
  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->columns; ++j) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }
}

static void perform_sub(const matrix_t *A, const matrix_t *B, matrix_t *result) {
  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->columns; ++j) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
}


static void perform_mult_number(const matrix_t *A, double number, matrix_t *result) {
  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->columns; ++j) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }
}

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  result_code_t return_code = OK;

  if (rows <= 0 || columns <= 0 || result == NULL) {
    return_code = INCORRECT_MATRIX;
  }

  if (return_code == OK) {
    result->matrix = (double **)calloc(rows, sizeof(double *));
    if (result->matrix == NULL) {
      return_code = INCORRECT_MATRIX;
    } else {
      // Инициализируем поля структуры сразу после успешного выделения.
      result->rows = rows;
      result->columns = columns;
    }
  }

  if (return_code == OK) {
    return_code = allocate_matrix_rows(result);
  }

  return return_code;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL && A->matrix != NULL) {
    for (int i = 0; i < A->rows; ++i) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int return_code = SUCCESS;

  if (!is_matrix_valid(A) || !is_matrix_valid(B) ||
      A->rows != B->rows || A->columns != B->columns) {
    return_code = FAILURE;
  }

  if (return_code == SUCCESS) {
    for (int i = 0; i < A->rows; ++i) {
      for (int j = 0; j < A->columns; ++j) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > S21_EPS) {
          return_code = FAILURE;
          break;
        }
      }
      if (return_code == FAILURE) {
        break;
      }
    }
  }

  return return_code;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  result_code_t return_code = OK;

  if (!is_matrix_valid(A) || !is_matrix_valid(B)) {
    return_code = INCORRECT_MATRIX;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    return_code = CALCULATION_ERROR;
  }

  if (return_code == OK) {
    return_code = s21_create_matrix(A->rows, A->columns, result);
  }

  if (return_code == OK) {
    perform_sum(A, B, result);
  }

  return return_code;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  result_code_t return_code = OK;

  if (!is_matrix_valid(A) || !is_matrix_valid(B)) {
    return_code = INCORRECT_MATRIX;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    return_code = CALCULATION_ERROR;
  }

  if (return_code == OK) {
    return_code = s21_create_matrix(A->rows, A->columns, result);
  }

  if (return_code == OK) {
    perform_sub(A, B, result);
  }

  return return_code;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  result_code_t return_code = OK;

  if (!is_matrix_valid(A)) {
    return_code = INCORRECT_MATRIX;
  }

  if (return_code == OK) {
    return_code = s21_create_matrix(A->rows, A->columns, result);
  }

  if (return_code == OK) {
    perform_mult_number(A, number, result);
  }

  return return_code;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  result_code_t return_code = OK;

  if (!is_matrix_valid(A) || !is_matrix_valid(B)) {
    return_code = INCORRECT_MATRIX;
  }

  if (return_code == OK && A->columns != B->rows) {
    return_code = CALCULATION_ERROR;
  }

  if (return_code == OK) {
    return_code = s21_create_matrix(A->rows, B->columns, result);
  }

  if (return_code == OK) {
    perform_multiplication(A, B, result);
  }

  return return_code;
}


int s21_transpose(matrix_t *A, matrix_t *result) {
  result_code_t return_code = OK;

  if (!is_matrix_valid(A)) {
    return_code = INCORRECT_MATRIX;
  }

  if (return_code == OK) {
    return_code = s21_create_matrix(A->columns, A->rows, result);
  }

  if (return_code == OK) {
    perform_transposition(A, result);
  }

  return return_code;
}


int s21_determinant(matrix_t *A, double *result) {
  result_code_t return_code = OK;
  *result = 0.0;

  if (!is_matrix_valid(A)) {
    return_code = INCORRECT_MATRIX;
  } else if (A->rows != A->columns) {
    return_code = CALCULATION_ERROR;
  } else if (A->rows == 1) {
    *result = A->matrix[0][0];
  } else {
    for (int j = 0; j < A->columns; ++j) {
      matrix_t minor;
      return_code = s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
      if (return_code != OK) break;

      get_minor_matrix(A, 0, j, &minor);
      double minor_det = 0.0;
      return_code = s21_determinant(&minor, &minor_det);
      s21_remove_matrix(&minor);

      if (return_code != OK) break;
      double sign = (j % 2 == 0) ? 1.0 : -1.0;
      *result += sign * A->matrix[0][j] * minor_det;
    }
  }

  return return_code;
}

int s21_calc_complements(const matrix_t *A, matrix_t *result) {
  result_code_t return_code = OK;

  if (!is_matrix_valid(A)) {
    return_code = INCORRECT_MATRIX;
  } else if (A->rows != A->columns) {
    return_code = CALCULATION_ERROR;
  }

  if (return_code == OK) {
    return_code = s21_create_matrix(A->rows, A->columns, result);
  }

  if (return_code == OK) {
    return_code = populate_complements_matrix(A, result);
  }
  
  return return_code;
}


int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  result_code_t return_code = OK;
  double det = 0.0;

  if (!is_matrix_valid(A)) {
    return_code = INCORRECT_MATRIX;
  }

  // Функция s21_determinant верифицирует квадратичность матрицы.
  if (return_code == OK) {
    return_code = s21_determinant(A, &det);
  }

  if (return_code == OK) {
    if (fabs(det) < S21_EPS) {
      return_code = CALCULATION_ERROR;
    }
  }

  if (return_code == OK) {
    return_code = perform_inverse_calculation(A, det, result);
  }

  return return_code;
}