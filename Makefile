FLAGS = -ggdb3 -std=c++17 -O0 \
	-Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat \
	-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
	-Wconditionally-supported -Wconversion -Wctor-dtor-privacy \
	-Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security \
	-Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
	-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked \
	-Wpointer-arith -Winit-self -Wredundant-decls -Wshadow \
	-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 \
	-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types \
	-Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef \
	-Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros \
	-Wno-literal-suffix -Wno-missing-field-initializers \
	-Wno-narrowing -Wno-old-style-cast -Wno-varargs \
    -fcheck-new -fsized-deallocation -fstack-protector \
	-fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer \
	-Wlarger-than=32768 -Wstack-usage=8192 -pie -fPIE -Werror=vla \
	-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr


OBJDIR = obj
$(OBJDIR):
	@mkdir -p $(OBJDIR)


FILES = $(OBJDIR)/main.o $(OBJDIR)/tree.o $(OBJDIR)/tree_io.o $(OBJDIR)/tree_comparison.o \
    $(OBJDIR)/stack.o $(OBJDIR)/stack_error.o
DEBUG_FILES = $(OBJDIR)/graph_generator.o $(OBJDIR)/html_builder.o $(OBJDIR)/tree_test.o
OUTPUT_NAME = akinator


clean: 
	@echo "cleaning up object files..."
	@rm -rf $(OBJDIR)


debug: FLAGS += -DDEBUG


debug: $(FILES) $(DEBUG_FILES)
	@g++ $(FILES) $(DEBUG_FILES) $(FLAGS) -o $(OUTPUT_NAME)


release: $(FILES)
	@g++ $(FILES) $(FLAGS) -o $(OUTPUT_NAME)


$(OBJDIR)/main.o: main.cpp | $(OBJDIR)
	@g++ -c main.cpp $(FLAGS) -o $(OBJDIR)/main.o


$(OBJDIR)/tree.o: tree.cpp | $(OBJDIR)
	@g++ -c tree.cpp $(FLAGS) -o $(OBJDIR)/tree.o


$(OBJDIR)/tree_io.o: tree_io.cpp | $(OBJDIR)
	@g++ -c tree_io.cpp $(FLAGS) -o $(OBJDIR)/tree_io.o


$(OBJDIR)/graph_generator.o: graph_generator.cpp | $(OBJDIR)
	@g++ -c graph_generator.cpp $(FLAGS) -o $(OBJDIR)/graph_generator.o


$(OBJDIR)/html_builder.o: html_builder.cpp | $(OBJDIR)
	@g++ -c html_builder.cpp $(FLAGS) -o $(OBJDIR)/html_builder.o


$(OBJDIR)/tree_test.o: tree_test.cpp | $(OBJDIR)
	@g++ -c tree_test.cpp $(FLAGS) -o $(OBJDIR)/tree_test.o


$(OBJDIR)/tree_comparison.o: tree_comparison.cpp | $(OBJDIR)
	@g++ -c tree_comparison.cpp $(FLAGS) -o $(OBJDIR)/tree_comparison.o


$(OBJDIR)/stack.o: stack.cpp | $(OBJDIR)
	@g++ -c stack.cpp $(FLAGS) -o $(OBJDIR)/stack.o


$(OBJDIR)/stack_error.o: stack_error.cpp | $(OBJDIR)
	@g++ -c stack_error.cpp $(FLAGS) -o $(OBJDIR)/stack_error.o
