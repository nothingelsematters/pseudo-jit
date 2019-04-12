#ifndef PSEUDOCOMPILER_HPP
#define PSEUDOCOMPILER_HPP


#include <exception>
#include <cstring>
#include <cstddef>
#include <sys/mman.h>

static const int PAGE_SIGE = 8192;

class pcompiler {
public:
    pcompiler(const char* str) : str(str) {
        memory = mmap(NULL, PAGE_SIGE, PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

        if (memory == MAP_FAILED) {
            perror("failed to map memory");
            throw std::exception();
        }
    }

    ~pcompiler() {
        if (munmap(memory, PAGE_SIGE) == -1) {
            perror("\nfailed to delete allocated memory");
        }
    }

    void compile()  {
        std::memcpy(memory, code, sizeof(code));
        unsigned char* func = static_cast<unsigned char*>(memory);
        intptr_t ptr = reinterpret_cast<intptr_t>(str);

        for (size_t i = 0; i < 6; ++i) {
            func[insertion + i] = get_part(ptr, i);
        }
    }

    int execute() const {
        if (mprotect(memory, PAGE_SIGE, PROT_EXEC) == -1) {
            perror("\nfailed to make a page executable");
            throw std::exception();
        }

        return ((int (*)()) memory)();
    }

private:
    unsigned char get_part(intptr_t ptr, int part) {
        int shift = part * 8;
        return (ptr & (((1ll << 8) - 1) << shift)) >> shift;
    }

    const char* str;
    void* memory;

    static constexpr size_t insertion = 6;
    static constexpr unsigned char code[] = {
        0x55,                                       // push   %rbp
        0x48, 0x89, 0xe5,                           // mov    %rsp,%rbp
        0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00,   // movabs $0x0,%rax
        0x00, 0x00, 0x00,                           //
        0x48, 0x89, 0x45, 0xf0,                     // mov    %rax,-0x10(%rbp)
        0x48, 0x8b, 0x45, 0xf0,                     // mov    -0x10(%rbp),%rax
        0x48, 0x89, 0x45, 0xe8,                     // mov    %rax,-0x18(%rbp)
        0xc7, 0x45, 0xe4, 0x93, 0x01, 0x00, 0x01,   // movl   $0x1000193,-0x1c(%rbp)
        0xc7, 0x45, 0xfc, 0xc5, 0x9d, 0x1c, 0x81,   // movl   $0x811c9dc5,-0x4(%rbp)
        0xc7, 0x45, 0xf8, 0x00, 0x00, 0x00, 0x00,   // movl   $0x0,-0x8(%rbp)
        0x8b, 0x45, 0xf8,                           // mov    -0x8(%rbp),%eax
        0x48, 0x63, 0xd0,                           // movslq %eax,%rdx
        0x48, 0x8b, 0x45, 0xe8,          	        // mov    -0x18(%rbp),%rax
        0x48, 0x01, 0xd0,                           // add    %rdx,%rax
        0x0f, 0xb6, 0x00,                           // movzbl (%rax),%eax
        0x84, 0xc0,                                 // test   %al,%al
        0x74, 0x2a,                                 // je     401152 <_Z7fnvhashPKc+0x5b>
        0x8b, 0x45, 0xfc,                           // mov    -0x4(%rbp),%eax
        0x69, 0xd0, 0x93, 0x01, 0x00, 0x01,         // imul   $0x1000193,%eax,%edx
        0x8b, 0x45, 0xf8,                           // mov    -0x8(%rbp),%eax
        0x48, 0x63, 0xc8,                           // movslq %eax,%rcx
        0x48, 0x8b, 0x45, 0xe8,                     // mov    -0x18(%rbp),%rax
        0x48, 0x01, 0xc8,                           // add    %rcx,%rax
        0x0f, 0xb6, 0x00,                           // movzbl (%rax),%eax
        0x0f, 0xbe, 0xc0,                           // movsbl %al,%eax
        0x0f, 0xb6, 0xc0,                           // movzbl %al,%eax
        0x31, 0xd0,                                 // xor    %edx,%eax
        0x89, 0x45, 0xfc,                           // mov    %eax,-0x4(%rbp)
        0x83, 0x45, 0xf8, 0x01,                     // addl   $0x1,-0x8(%rbp)
        0xeb, 0xc2,                                 // jmp    401114 <_Z7fnvhashPKc+0x1d>
        0x8b, 0x45, 0xfc,                           // mov    -0x4(%rbp),%eax
        0x5d,                                       // pop    %rbp
        0xc3                                        // retq
    };
};

#endif /* end of include guard: PSEUDOCOMPILER_HPP */
