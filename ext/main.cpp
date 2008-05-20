#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "word.h"
#include "dict.h"

using namespace std;

int main(int argc, char const *argv)
{
    rmmseg::dict::load_chars("../chars.dic");
    rmmseg::dict::load_words("../words.dic");

    const char *str = "我";
    printf("looking for %s (nbytes=%d)\n", str, strlen(str));
    rmmseg::Word *w = rmmseg::dict::get(str, strlen(str));

    if (w)
        printf("%s (nbytes = %d, length = %d, frequency = %d)\n",
               w->text, w->nbytes, w->length, w->freq);
    return 0;
}
