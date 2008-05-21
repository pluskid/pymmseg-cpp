#include <ruby.h>
#include <cstdio>               // for debug

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


    /*********************
     * Dictionary module
     *********************/
    static VALUE mDictionary;
    
    static VALUE dic_load_chars(VALUE mod, VALUE path)
    {
        if (rmmseg::dict::load_chars(RSTRING(path)->ptr))
            return Qtrue;
        return Qfalse;
    }
    static VALUE dic_load_words(VALUE mod, VALUE path)
    {
        if (rmmseg::dict::load_words(RSTRING(path)->ptr))
            return Qtrue;
        return Qfalse;
    }
    static VALUE dic_add(VALUE mod, VALUE word, VALUE len, VALUE freq)
    {
        const char *str = RSTRING(word)->ptr;
        int nbytes = RSTRING(word)->len;
        rmmseg::Word *w = rmmseg::make_word(str, FIX2INT(len), FIX2INT(freq), nbytes);
        rmmseg::dict::add(w);
        return Qnil;
    }
    static VALUE dic_has_word(VALUE mod, VALUE word)
    {
        const char *str = RSTRING(word)->ptr;
        int nbytes = RSTRING(word)->len;
        if (rmmseg::dict::get(str, nbytes) != NULL)
            return Qtrue;
        return Qfalse;
    }


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

    static VALUE tk_text(VALUE self)
    {
        Token *tk = (Token *)DATA_PTR(self);
        return tk->text;
    }
    static VALUE tk_start(VALUE self)
    {
        Token *tk = (Token *)DATA_PTR(self);
        return tk->start;
    }
    static VALUE tk_end(VALUE self)
    {
        Token *tk = (Token *)DATA_PTR(self);
        return tk->end;
    }

    static VALUE cToken;
    static VALUE tk_create(const char* base, const rmmseg::Token &t)
    {
        Token *tk = (Token *)malloc(sizeof(Token));
        int start = t.text-base;
        tk->text = rb_str_new(t.text, t.length);
        tk->start = INT2FIX(start);
        tk->end = INT2FIX(start + t.length);
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
    static VALUE algor_next_token(VALUE self)
    {
        Algorithm *algor = (Algorithm *)DATA_PTR(self);
        rmmseg::Token tk = algor->algor->next_token();

        if (tk.length == 0)
            return Qnil;
        return tk_create(RSTRING(algor->text)->ptr, tk);
    }


    void Init_rmmseg()
    {
        typedef VALUE (*RUBY_METHOD) (...);
        mRMMSeg = rb_define_module("RMMSeg");

        mDictionary = rb_define_module_under(mRMMSeg, "Dictionary");
        rb_define_singleton_method(mDictionary, "load_chars", (RUBY_METHOD)dic_load_chars, 1);
        rb_define_singleton_method(mDictionary, "load_words", (RUBY_METHOD)dic_load_words, 1);
        rb_define_singleton_method(mDictionary, "load_add", (RUBY_METHOD)dic_add, 3);
        rb_define_singleton_method(mDictionary, "has_word?", (RUBY_METHOD)dic_has_word, 1);

        cToken = rb_define_class_under(mRMMSeg, "Token", rb_cObject);
        rb_undef_method(rb_singleton_class(cToken), "new");
        rb_define_method(cToken, "text", (RUBY_METHOD)tk_text, 0);
        rb_define_method(cToken, "start", (RUBY_METHOD)tk_start, 0);
        rb_define_method(cToken, "end", (RUBY_METHOD)tk_end, 0);

        cAlgorithm = rb_define_class_under(mRMMSeg, "Algorithm", rb_cObject);
        rb_define_singleton_method(cAlgorithm, "new", (RUBY_METHOD)algor_create, 1);
        rb_define_method(cAlgorithm, "next_token", (RUBY_METHOD)algor_next_token, 0);
    }
}
