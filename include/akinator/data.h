#ifndef INCLUDE_AKINATOR_DATA_H_
#define INCLUDE_AKINATOR_DATA_H_


const int BUFFER_SIZE = 256;


extern const char* UNKNOWN_STRING;
extern const char* status_messages[];
extern const char* DUMP_DIRECTORY;
extern const char* DEFAULT_INPUT_FILE;
extern const char* DEFAULT_OUTPUT_FILE;


typedef enum {
    TREE_OK = 0,
    TREE_STACK_ERROR,
    TREE_NOT_FOUND,
    TREE_NULL_DATA_POINTER,
    TREE_ROOT_HAS_PARENT,
    TREE_MISSING_PARENT,
    TREE_PARENT_CHILD_MISMATCH,
    TREE_INVALID_BRANCH_STRUCTURE,
    TREE_OUT_OF_MEMORY,
    TREE_INPUT_READ_ERROR,
    TREE_INPUT_FILE_OPEN_ERROR,
    TREE_INPUT_FILE_READ_ERROR,
    TREE_INPUT_FILE_CLOSE_ERROR,
    TREE_OUTPUT_FILE_OPEN_ERROR,
    TREE_OUTPUT_FILE_WRITE_ERROR,
    TREE_OUTPUT_FILE_CLOSE_ERROR,
    TREE_INVALID_COUNT,
    TREE_UNKNOWN_CMD_ARGUMENTS,
    TREE_INVALID_USER_CHOICE
} TreeStatus;


typedef enum {
    CHOICE_GUESS = 0,
    CHOICE_DEFINE,
    CHOICE_COMPARE,
    CHOICE_EXIT_SAVE,
    CHOICE_EXIT_NO_SAVE
} UserChoice;


typedef enum {
    ANSWER_YES = 0,
    ANSWER_NO,
    ANSWER_NONE
} AnswerType;


#ifdef DEBUG

typedef struct {
    TreeStatus status;
    const char* message;
    const char* file;
    const char* function;
    int line;
} DumpInfo;


typedef struct {
    const char* name;
    const char* file;
    const char* function;
    int line;
} TreeCreationInfo;


typedef struct {
    FILE* file;
    char directory[BUFFER_SIZE];
    int image_counter;
} TreeDumpState;


typedef struct {
    TreeCreationInfo creation;
    TreeDumpState dump;
} TreeDebugInfo;

#endif // DEBUG


typedef struct {
    const char* input_file;
    const char* output_file;
} Arguments;


typedef struct Node Node;
struct Node {
    char* data;
    bool is_dynamic;
    Node* left;
    Node* right;
#ifdef DEBUG
    Node* parent;
#endif // DEBUG
};


typedef struct {
    Node* node;
    AnswerType answer;
} PathStep;


typedef struct {
    Node* root;
    char* buffer;
    Arguments args; 
#ifdef DEBUG
    TreeDebugInfo debug;
#endif // DEBUG
} BinaryTree;


#endif // INCLUDE_AKINATOR_DATA_H_
