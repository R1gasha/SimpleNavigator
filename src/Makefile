.PHONY: all clean test coverage clang-format check-format

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic
GTESTFLAGS = -Wno-sign-compare
TEST_DIR = tests
SRC_DIR = src
GTEST_INCLUDE = /usr/local/include
GTEST_LIB = /usr/local/lib
TEST_EXEC = test_s21_containers
TEST_EXEC_COV = test_s21_containers_cov

CLANG_FORMAT ?= clang-format

UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
  OPEN = open
  LINKING = -L$(GTEST_LIB)
  COVERAGE_FLAGS = -fprofile-instr-generate -fcoverage-mapping
  COVERAGE_LIBS =
else
  OPEN = xdg-open
  LINKING =
  CXX = g++
  TEST_FLAGS=-lgtest -pthread
  COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
  COVERAGE_LIBS = -lgcov
endif

all: test

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(TEST_DIR)/*.cc
	@$(CXX) $(CXXFLAGS) $(GTESTFLAGS) \
		-I$(SRC_DIR) -I$(GTEST_INCLUDE) $(LINKING) \
		$(TEST_DIR)/*.cc \
		-lgtest -lgtest_main -lpthread \
		-o $@

mac-coverage: clean-coverage
	@$(CXX) $(CXXFLAGS) $(GTESTFLAGS) -fprofile-arcs -ftest-coverage -O0 \
		-I. -I$(GTEST_INCLUDE) $(LINKING) $(COVERAGE_LIBS) \
		tests/*.cc\
		-lgtest -lgtest_main -lpthread \
		-o $(TEST_EXEC_COV)
	
	@./$(TEST_EXEC_COV)
	
	@gcov vector.h
	@ls -la vector.h.gcda || echo "*** NO GCDA - tests not executing vector.h ***"

	@lcov --capture --directory . --output-file coverage.info --ignore-errors empty,unsupported,inconsistent,unused,format
	@lcov --remove coverage.info '/usr/local/*' '*/tests/*' -o coverage.info '*memory*' '*utility*' '*algorithm*' \
	'*string*' '*debug_utils*' '*functional*' '*iterator*' '*/v1/*' -o coverage.info
	@genhtml coverage.info --output-directory coverage_report --ignore-errors category
	@$(OPEN) coverage_report/index.html

lin-coverage: clean-coverage
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) tests/*.cc -o $(TEST_EXEC) $(TEST_FLAGS)
	mkdir -p report
	./$(TEST_EXEC)
	lcov --ignore-errors inconsistent,mismatch -t "Report" -c -d ./ --output-file ./coverage.info
	lcov --ignore-errors unused --remove ./coverage.info '/usr/*' '*/gtest/*' '*/gmock/*' -o ./coverage.info
	genhtml ./coverage.info --output-directory ./report/

cppcheck:
	@cppcheck --enable=all --inconclusive --std=c++20 \
	--suppress=missingIncludeSystem \
	--suppress=unusedFunction \
	--error-exitcode=0 \
	*.h $(TEST_DIR)/*.cc

test_gcc: clean_linux
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) tests/*.cc -o test $(TEST_FLAGS)

valgrind: test
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
    ./$(TEST_EXEC)

clean_linux:
	rm -rf *.a
	rm -rf *.o
	rm -rf *.gcda 
	rm -rf *.gcno 
	rm -rf coverage.info 
	rm -rf report/
	rm -f $(TEST_EXEC)

clean-coverage:
	@rm -rf report/
	@rm -f *.profraw *.profdata $(TEST_EXEC_COV) tests/*.o coverage.info test_s21_containers_cov *.gcda *.gcno
	@rm -rf coverage_report

clang-format:
	@$(CLANG_FORMAT) -style="{BasedOnStyle: Google}" -i *.h $(TEST_DIR)/*.cc

check-format:
	@$(CLANG_FORMAT) -style="{BasedOnStyle: Google}" --dry-run --Werror *.h $(TEST_DIR)/*.cc

clean:
	@rm -f $(TEST_EXEC)
	@rm -f $(TEST_EXEC)
	@rm -f s21py*.so s21py*.pyd


# --- Python-расширение через pybind11 ---
PYTHON ?= python3
PY_INCLUDES := $(shell $(PYTHON) -m pybind11 --includes)
EXT_SUFFIX  := $(shell python3-config --extension-suffix)
PY_MODULE   := s21py$(EXT_SUFFIX)

.PHONY: py_module
py_module: $(PY_MODULE)

$(PY_MODULE): bindings.cpp cpp_containers/s21_list.h cpp_containers/s21_stack.h cpp_containers/s21_queue.h
	$(CXX) -O2 -std=c++20 -fPIC -shared \
		$(PY_INCLUDES) \
		-I. \
		bindings.cpp \
		-o $@
