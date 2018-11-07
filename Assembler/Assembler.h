
#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <unordered_map>
#include <experimental/string_view>
#include "../Stack.h"
#include "../CommandCodes.h"

using std::experimental::string_view;

#define ASSERT(COND, MSG)                                       \
    if(!(COND))                                                 \
    {                                                           \
        fprintf(stderr, "Error happened: %s\n", MSG);           \
        fprintf(stderr, "Function: %s\n", __PRETTY_FUNCTION__); \
        fprintf(stderr, "Line: %d\n", __LINE__);                \
        assert(COND);                                           \
    }

#define PROC_ARG                                          \
    if (!readArgument(argbuf, &argval))                   \
    {                                                     \
        printf("Read argument of unsupported format!\n"); \
        return 1;                                         \
    }

const size_t MAX_LINE_LENGTH = 256;
const size_t MAX_ARG_LENGTH  = 256;

enum LABEL_CODES
{
    WRITTEN_CODE  = 0,
    FIXUP_CREATED = 1,
    UNKNOWN_ARG   = 2
};

struct Fixup
{
    size_t index_where_;
    string_view label_name_;
    bool deleted_;
    
    Fixup():
        index_where_(0),
        label_name_(),
        deleted_(false)
    {}

    Fixup(size_t index_where, char* buf, size_t buf_size):
        index_where_(index_where),
        label_name_(),
        deleted_(false)
    {
        char* raw_string = new char[buf_size + 1];
        strcpy(raw_string, buf);
        label_name_ = string_view(raw_string, buf_size);
    }
    
    Fixup(Fixup&& that)
    {
        index_where_ = that.index_where_;
        label_name_ = that.label_name_;
        that.deleted_ = true;
    }
    
    Fixup& operator =(Fixup&& that)
    {
        index_where_ = that.index_where_;
        label_name_ = that.label_name_;
        deleted_ = that.deleted_;
        that.deleted_ = true;
        return *this;
    }
    
    void recover(FILE* file, std::unordered_map<string_view, size_t>& labels) const
    {
        ASSERT(file, "Wrong file to recover fixup in!\n");
        fseek(file, index_where_, SEEK_SET);
        int place = static_cast<int>(labels[label_name_]);
        fwrite(&place, sizeof(int), 1, file);
    }

    ~Fixup()
    {
        if (!deleted_)
            delete[] label_name_.data();
    }

    Fixup(const Fixup& that) = delete;
    friend std::ostream& operator <<(std::ostream& stream, const Fixup& fixup);
};

std::ostream& operator <<(std::ostream& stream, const Fixup& fixup)
{
    stream << "Fixup<" << fixup.index_where_ << ", " << fixup.label_name_ << ">";
}

class Assembler
{
private:
    FILE* source_code_file_;
    FILE* output_file_;
    Stack<Fixup> fixups_;
    size_t n_written_;
    std::unordered_map<string_view, size_t> labels_;

	bool readArgument(char* buf, int* argval, bool need_scanf = true)
	{
        if (need_scanf)
            fscanf(source_code_file_, "%s", buf);
		
        char* end = 0;
		*argval = strtol(buf, &end, 10);
        printf("Checking argument: %s\n", buf);    

		return *end == 0;
	}
    
    //Returns number to jump to or creates a fixup
    size_t readArgumentLabel(char* buf, int* argval)
    {
        size_t length = strlen(buf);
        ASSERT(length > 0, "Empty label provided!\n");
        
        if (buf[0] == ':' && length > 1)
        {
            Fixup new_fixup(n_written_, buf + 1, length - 1);
            fixups_.push(std::move(new_fixup));
            return FIXUP_CREATED;
        }
        else
        {
            if (readArgument(buf, argval, false))
                return WRITTEN_CODE;
            else
                return UNKNOWN_ARG;
        }
    }
	
	void writeInt(int number)
	{
		fwrite(&number, sizeof(int), 1, output_file_);
	    n_written_ += sizeof(int);
    }
    
    void writeChar(char c)
    {
        fwrite(&c, sizeof(char), 1, output_file_);
        n_written_ += sizeof(char);
    }

public:
    Assembler(FILE* file, const char* output_filename = "a.out"):
        source_code_file_(file),
        output_file_(fopen(output_filename, "wb")),
        n_written_(0)
    {
        ASSERT(file,          "Provided zero file");
        ASSERT(!ferror(file), "Provided invalid file");

        ASSERT(output_file_ && !ferror(output_file_), "Cannot open output file");
    }

    Assembler(const char* filename, const char* output_filename):
		Assembler(fopen(filename, "r"), output_filename)
    {}
	
    void dump()
    {
        printf("Assembler dump here\n");
        printf("Already %lu symbols have been written\n", n_written_);

        printf("Labels:\n");
        for (auto it = labels_.begin(); it != labels_.end(); ++it)
        {
            printf("   Label %s references to %lu\n", it->first.data(), it->second);
        }
        fixups_.dump("ASM Dump");
        printf("Bye!\n");
    }

    bool processNewLabel(char* buf, size_t size)
    {
        char* new_label = new char[size + 1];
        strcpy(new_label, buf);
        string_view new_label_view(new_label, size);

        char msg[MAX_ARG_LENGTH] = "";
        sprintf(msg, "Label %s redefined", buf);
        ASSERT(labels_.find(new_label_view) == labels_.end(), msg);

        labels_[new_label_view] = n_written_;
    }

    int makeAssembling()
    {
        while (!feof(source_code_file_))
        {
            char cmd_name[MAX_LINE_LENGTH] = "";
            fscanf(source_code_file_, "%s", cmd_name);
     		
            if (strlen(cmd_name) == 0)
                continue;

			int argval = 0;
			char argbuf[MAX_ARG_LENGTH] = "";
       
            if (false);
            else if (!strcmp(cmd_name, "push"))
            {
     			PROC_ARG
                writeChar(PUSH);
                writeInt(argval);
            }
            else if (!strcmp(cmd_name, "pop"))
            {
                writeChar(POP);
            }
            else if (!strcmp(cmd_name, "add"))
            {
                writeChar(ADD);
            }
            else if (!strcmp(cmd_name, "mul"))
            {
                writeChar(MUL);
            }
            else if (!strcmp(cmd_name, "in"))
            {
                writeChar(IN);
            }
            else if (!strcmp(cmd_name, "out"))
            {
                writeChar(OUT);
            }
            else if (!strcmp(cmd_name, "end"))
            {
                writeChar(END);
            }
            else if (!strcmp(cmd_name, "jmp"))
            {
                writeChar(JMP);
                fscanf(source_code_file_, "%s", argbuf);
                size_t res = readArgumentLabel(argbuf, &argval);

                switch (res)
                {
                    case WRITTEN_CODE:
                        writeInt(argval);
                        break;
                    
                    case FIXUP_CREATED:
                        writeInt(0);
                        break;

                    case UNKNOWN_ARG:
                        ASSERT(false, "Unsupported argument format");
                        break;
                }
            }
            else if (cmd_name[0] == ':')
            {
                processNewLabel(cmd_name + 1, strlen(cmd_name) - 1);
            }
            else
            {
                printf("Unknown command: %s\n", cmd_name);
                return 1;
            }
        }
        
        recoverFixups();
        return 0;
    }

    void recoverFixups()
    {
        while (!fixups_.empty())
        {
            fixups_.top().recover(output_file_, labels_);
            fixups_.pop();
        }
    }

    ~Assembler()
    {
        for (auto iter = labels_.begin(); iter != labels_.end(); ++iter)
        {
            delete[] iter->first.data();
        }

        fclose(output_file_);
        fclose(source_code_file_);
    }
};
