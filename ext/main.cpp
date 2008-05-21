#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "word.h"
#include "dict.h"
#include "complex.h"

using namespace std;

int main(int argc, char const *argv)
{
    rmmseg::dict::load_chars("../chars.dic");
    rmmseg::dict::load_words("../words.dic");

    const char *str = "中文";
    printf("looking for %s (nbytes=%d)\n", str, strlen(str));
    rmmseg::Word *w = rmmseg::dict::get(str, strlen(str));

    if (w)
        printf("%s (nbytes = %d, length = %d, frequency = %d)\n",
               w->text, w->nbytes, w->length, w->freq);

    const char *txt = "我们是一段中文我们";
    char buf[12];
    rmmseg::ComplexAlgorithm algor(txt);

    while (true)
    {
        rmmseg::Token t = algor.next_token();
        if (t.length == 0)
            break;
        strncpy(buf, t.text, t.length);
        buf[t.length] = '\0';
        printf("%s ", buf);
    }
    return 0;
}
