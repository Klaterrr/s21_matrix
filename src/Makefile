# =================================================================================
# Инструменты и компилятор
# =================================================================================
CC = gcc
AR = ar
CLANG_FORMAT = clang-format

# =================================================================================
# Флаги компиляции и линковки
# =================================================================================
CFLAGS = -std=c11 -Wall -Werror -Wextra -g
LDFLAGS = -lcheck -lm -lsubunit
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

# =================================================================================
# Имена файлов и директорий
# =================================================================================
LIB_NAME = s21_matrix.a
TEST_EXECUTABLE = test_runner
GCOV_EXECUTABLE = gcov_runner

# Используем wildcard для автоматического поиска файлов
SRC_FILES = $(wildcard s21_*.c)
TEST_FILES = $(wildcard tests/s21_*.c)
ALL_SOURCES = $(SRC_FILES) $(TEST_FILES)
HEADER_FILES = $(wildcard s21_*.h)

# Директория для объектных файлов и других артефактов сборки
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))
TEST_OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(TEST_FILES:.c=.o)))

# Путь к конфигурации clang-format
CLANG_FORMAT_CONFIG = ../materials/linters/.clang-format

# =================================================================================
# Основные цели (user-facing)
# =================================================================================

# .PHONY: цели, которые не создают файлы с таким же именем
.PHONY: all clean rebuild test gcov_report style format script_build script_test script_style script_run

# Цель по умолчанию: собрать библиотеку
all: $(LIB_NAME)

# Собрать библиотеку
$(LIB_NAME): $(OBJ_FILES)
	@echo "Creating static library $(LIB_NAME)..."
	$(AR) rcs $@ $^
	@echo "Library $(LIB_NAME) created successfully."

# Запустить тесты
test: CFLAGS += -DTEST_MODE # Пример добавления флага для тестов
test: $(LIB_NAME) $(TEST_OBJ_FILES)
	@echo "Building and running tests..."
	$(CC) $(CFLAGS) $(TEST_OBJ_FILES) -o $(BUILD_DIR)/$(TEST_EXECUTABLE) $(LIB_NAME) $(LDFLAGS)
	$(BUILD_DIR)/$(TEST_EXECUTABLE)

# Создать отчет о покрытии кода тестами
gcov_report:
	@echo "Generating GCOV report..."
	@mkdir -p report
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC_FILES) $(TEST_FILES) -o $(BUILD_DIR)/$(GCOV_EXECUTABLE) $(LDFLAGS)
	cd $(BUILD_DIR) && ./$(GCOV_EXECUTABLE)
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	@echo "------------------------------------------------------------"
	@echo "GCOV report generated in 'report' directory."
	@echo "To view it, open this file in your browser:"
	@echo "file://$(shell pwd)/report/index.html"
	@echo "------------------------------------------------------------"
	# Эта секция пытается автоматически открыть отчет. Может не работать на всех системах.
	@case "`uname`" in \
	  Linux*)    xdg-open report/index.html >/dev/null 2>&1 || echo "Could not auto-open report." ;; \
	  Darwin*)   open report/index.html >/dev/null 2>&1 || echo "Could not auto-open report." ;; \
	  MINGW*|CYGWIN*) start report/index.html >/dev/null 2>&1 || echo "Could not auto-open report." ;; \
	  *)         echo "Auto-opening not supported on this OS. Please open the report manually." ;; \
	esac

# Проверить стиль кода
style:
	@echo "Checking code style with clang-format..."
	@$(CLANG_FORMAT) -n -Werror --style=file:$(CLANG_FORMAT_CONFIG) $(ALL_SOURCES) $(HEADER_FILES)
	@echo "Style check passed."

# Автоматически отформатировать код
format:
	@echo "Formatting code with clang-format..."
	@$(CLANG_FORMAT) -i --style=file:$(CLANG_FORMAT_CONFIG) $(ALL_SOURCES) $(HEADER_FILES)
	@echo "Code formatted."

# Очистка
clean:
	@echo "Cleaning up build artifacts..."
	rm -rf $(BUILD_DIR) $(LIB_NAME) *.gcda *.gcno *.info gcov_report.info report
	@echo "Cleanup complete."

# Полная пересборка
rebuild: clean all

# =================================================================================
# Правила компиляции
# =================================================================================

# Правило для компиляции исходных файлов библиотеки
$(OBJ_DIR)/%.o: %.c $(HEADER_FILES)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для компиляции тестовых файлов
$(OBJ_DIR)/%.o: tests/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@