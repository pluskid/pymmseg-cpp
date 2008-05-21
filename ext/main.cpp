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

    const char *txt = "通过最大匹配找到了“研究生”一个词，进而吞掉了本该属于“生命”的“生”字。要如何解决这类问题呢？咋一看是没有办法了，除非理解这个句子，其实不然，有不少（比起“理解句子”来说）简单得多的算法可以排除不少错误的分割。不过，在介绍他们之前，值得注意一下的是，如果把最大匹配算法的扫描方向由正向改为反向，精确度可以提高不少。不过有时候拿到的是一个文本流，就没有办法首先得到末尾了。";
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
