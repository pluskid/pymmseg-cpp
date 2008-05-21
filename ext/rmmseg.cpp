#include <ruby.h>

#include "token.h"
#include "dict.h"
#include "algor.h"

using namespace std;

extern "C" {

    /*****************************************
     * 
     * Normal interface
     *
     *****************************************/

    /*********************
     * RMMSeg module
     *********************/
    static VALUE mRMMSeg;

    /**********************
     * Token Class
     **********************/
    struct Token
    {
        VALUE text;
        VALUE start;
        VALUE end;
    };

    static void tk_mark(Token *t)
    {
        // start and end are Fixnums, no need to mark them
        rb_gc_mark(t->text);
    }
    static void tk_free(Token *t)
    {
        free(t);
    }

    static VALUE cToken;
    static VALUE tk_create(const char* base, const rmmseg::Token &t)
    {
        Token *tk = (Token *)malloc(sizeof(Token));
        tk->text = rb_str_new(t.text, t.length);
        tk->start = t.text-base;
        tk->end = tk->start + t.length;
        return Data_Wrap_Struct(cToken,
                                (RUBY_DATA_FUNC)tk_mark,
                                (RUBY_DATA_FUNC)tk_free,
                                tk);
    }

    /*********************
     * Algorithm Class
     *********************/
    struct Algorithm
    {
        VALUE text;             // hold to avoid being garbage collected
        rmmseg::Algorithm *algor;
    };

    static void algor_mark(Algorithm *a)
    {
        rb_gc_mark(a->text);
    }
    static void algor_free(Algorithm *a)
    {
        free(a->algor);
    }

    static VALUE cAlgorithm;
    static VALUE algor_create(VALUE klass, VALUE text)
    {
        Algorithm *algor = (Algorithm *)malloc(sizeof(Algorithm));
        void *mem;
        algor->text = text;
        mem = malloc(sizeof(rmmseg::Algorithm));
        algor->algor = new(mem) rmmseg::Algorithm(RSTRING(text)->ptr,
                                                  RSTRING(text)->len);
        
        return Data_Wrap_Struct(klass,
                                (RUBY_DATA_FUNC)algor_mark,
                                (RUBY_DATA_FUNC)algor_free,
                                algor);
    }


    void Init_rmmseg()
    {
        typedef VALUE (*RUBY_METHOD) (...);
        mRMMSeg = rb_define_module("RMMSeg");

        cToken = rb_define_class_under(mRMMSeg, "Token", rb_cObject);
        rb_undef_method(rb_singleton_class(cToken), "new");

        cAlgorithm = rb_define_class_under(mRMMSeg, "Algorithm", rb_cObject);
        rb_define_singleton_method(cAlgorithm, "new", (RUBY_METHOD)algor_create, 1);
    }
}
