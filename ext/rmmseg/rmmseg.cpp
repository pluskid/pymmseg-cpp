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

    /*
     * Load a character dictionary.
     *
     * call-seq:
     *   load_chars(path)    -> status
     *
     * Return +true+ if loaded successfully, +false+ otherwise.
     */ 
    static VALUE dic_load_chars(VALUE mod, VALUE path)
    {
        if (rmmseg::dict::load_chars(RSTRING(path)->ptr))
            return Qtrue;
        return Qfalse;
    }

    /*
     * Load a word dictionary.
     *
     * call-seq:
     *   load_words(path)    -> status
     *
     * Return +true+ if loaded successfully, +false+ otherwise.
     */ 
    static VALUE dic_load_words(VALUE mod, VALUE path)
    {
        if (rmmseg::dict::load_words(RSTRING(path)->ptr))
            return Qtrue;
        return Qfalse;
    }

    /*
     * Add a word to the in-memory dictionary.
     *
     * call-seq:
     *   add(word, length, freq)
     *
     * - +word+ is a String.
     * - +length+ is number of characters (not number of bytes) of the
     *   word to be added.
     * - +freq+ is the frequency of the word. This is only used when
     *   it is a one-character word.
     */ 
    static VALUE dic_add(VALUE mod, VALUE word, VALUE len, VALUE freq)
    {
        const char *str = RSTRING(word)->ptr;
        int nbytes = RSTRING(word)->len;
        rmmseg::Word *w = rmmseg::make_word(str, FIX2INT(len), FIX2INT(freq), nbytes);
        rmmseg::dict::add(w);
        return Qnil;
    }

    /*
     * Check whether one word is included in the dictionary.
     *
     * call-seq:
     *   has_word?(word)    -> result
     *
     * Return +true+ if the word is included in the dictionary,
     * +false+ otherwise.
     */ 
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
        // start and end are Fixnums, no need to mark
        rb_gc_mark(t->text);
    }
    static void tk_free(Token *t)
    {
        free(t);
    }

    /*
     * Get the text held by this token.
     *
     * call-seq:
     *   text()    -> text
     *   
     */
    static VALUE tk_text(VALUE self)
    {
        Token *tk = (Token *)DATA_PTR(self);
        return tk->text;
    }

    /*
     * Get the start position of this token.
     *
     * call-seq:
     *   start()    -> start_pos
     *
     */
    static VALUE tk_start(VALUE self)
    {
        Token *tk = (Token *)DATA_PTR(self);
        return tk->start;
    }

    /*
     * Get the end position of this token.
     *
     * call-seq:
     *   end()    -> end_pos
     *
     */
    static VALUE tk_end(VALUE self)
    {
        Token *tk = (Token *)DATA_PTR(self);
        return tk->end;
    }

    static VALUE cToken;
    static VALUE tk_create(const char* base, const rmmseg::Token &t)
    {
        Token *tk = ALLOC(Token);
        int start = t.text-base;

        // This is necessary, see
        // http://pluskid.lifegoo.com/?p=348
        volatile VALUE text = rb_str_new(t.text, t.length);
        tk->text = text;

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

    /*
     * Create an Algorithm object to do segmenting on +text+.
     *
     * call-seq:
     *   new(text)    -> algorithm
     *   
     */ 
    static VALUE algor_create(VALUE klass, VALUE text)
    {
        Algorithm *algor = ALLOC(Algorithm);
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

    /*
     * Get next token.
     *
     * call-seq:
     *   next_token()   -> token
     *
     * Return +nil+ if no more token available.
     */ 
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
        mRMMSeg = rb_define_module("RMMSeg");

        /* Manage dictionaries used by rmmseg. */
        mDictionary = rb_define_module_under(mRMMSeg, "Dictionary");
        rb_define_singleton_method(mDictionary, "load_chars", RUBY_METHOD_FUNC(dic_load_chars), 1);
        rb_define_singleton_method(mDictionary, "load_words", RUBY_METHOD_FUNC(dic_load_words), 1);
        rb_define_singleton_method(mDictionary, "add", RUBY_METHOD_FUNC(dic_add), 3);
        rb_define_singleton_method(mDictionary, "has_word?", RUBY_METHOD_FUNC(dic_has_word), 1);

        /* A Token hold the text and related position information. */
        cToken = rb_define_class_under(mRMMSeg, "Token", rb_cObject);
        rb_undef_method(rb_singleton_class(cToken), "new");
        rb_define_method(cToken, "text", RUBY_METHOD_FUNC(tk_text), 0);
        rb_define_method(cToken, "start", RUBY_METHOD_FUNC(tk_start), 0);
        rb_define_method(cToken, "end", RUBY_METHOD_FUNC(tk_end), 0);

        /* An Algorithm object use the MMSEG algorithm to do segmenting. */
        cAlgorithm = rb_define_class_under(mRMMSeg, "Algorithm", rb_cObject);
        rb_define_singleton_method(cAlgorithm, "new", RUBY_METHOD_FUNC(algor_create), 1);
        rb_define_method(cAlgorithm, "next_token", RUBY_METHOD_FUNC(algor_next_token), 0);
    }
}
