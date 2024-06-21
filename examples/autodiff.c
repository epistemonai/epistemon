#include <epistemon/epistemon.h>
#include <epistemon/math/autodiff.h>

int main()
{
        epiTape tp = {0};
        epi_tape_init(&tp);
        epi_tape_set(&tp);

        // e = (a + b) * d
        // 20.0 = (2.0 + 3.0) * 4.0
        size_t a = epi_val(2.0f);
        size_t b = epi_val(3.0f);
        size_t c = epi_add(a, b);
        size_t d = epi_val(4.0f);
        size_t e = epi_mul(c, d);

        epi_tape_print();

        epi_reverse(e);
        epi_print_tree(e);

        epi_tape_release();
        epi_tape_destroy(&tp);

        return 0;
}